/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include <catch2/catch_all.hpp>

#include "roq/debug/hex/message.hpp"

#include "roq_sbe/MessageHeader.h"
#include "roq_sbe/TopOfBook.h"

using namespace std::literals;

TEST_CASE("top_of_book_encode", "[top_of_book]") {
  using mesage_type = roq_sbe::TopOfBook;
  using message_header_type = roq_sbe::MessageHeader;

  std::vector<std::byte> buffer(4096);

  mesage_type value;

  auto &result = value.wrapAndApplyHeader(reinterpret_cast<char *>(std::data(buffer)), 0, std::size(buffer));

  result.putExchange("deribit"sv);
  result.putSymbol("BTC-PERPETUAL"sv);
  auto &layer = result.layer();
  layer.bidPrice(123.4);
  layer.bidQuantity(1.234);
  layer.askPrice(234.5);
  layer.askQuantity(2.345);
  result.updateType(roq_sbe::UpdateType::Value::Incremental);
  result.exchangeTimeUTC(1234);
  result.exchangeSequence(2345);
  result.sendingTimeUTC(3456);

  auto length = message_header_type::encodedLength() + mesage_type::computeLength();
  CHECK(length == 145);  // see next test

  std::span message{std::data(buffer), length};

  fmt::print("{}\n"sv, roq::debug::hex::Message{message});
}

TEST_CASE("top_of_book_decode", "[top_of_book]") {
  using mesage_type = roq_sbe::TopOfBook;
  using message_header_type = roq_sbe::MessageHeader;

  auto const message =
      "\x89\x00"  // block length (???)
      "\x0c\x00"  // template id (12)
      "\x01\x00"  // schema id (1)
      "\x01\x00"  // version (1)
      "\x64\x65\x72\x69\x62\x69\x74\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"  // exchange
      "\x42\x54\x43\x2d\x50\x45\x52\x50\x45\x54\x55\x41\x4c\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x00\x00"     // symbol
      "\x9a\x99\x99\x99\x99\xd9\x5e\x40"     // bid price
      "\x58\x39\xb4\xc8\x76\xbe\xf3\x3f"     // bid quantity
      "\x00\x00\x00\x00\x00\x50\x6d\x40"     // ask price
      "\xc3\xf5\x28\x5c\x8f\xc2\x02\x40"     // ask quantity
      "\x02"                                 // update type (incremental)
      "\xd2\x04\x00\x00\x00\x00\x00\x00"     // exchange time utc
      "\x29\x09\x00\x00\x00\x00\x00\x00"     // exchange sequence
      "\x80\x0d\x00\x00\x00\x00\x00\x00"sv;  // sending time utc
  REQUIRE(std::size(message) == 145);

  std::span payload{reinterpret_cast<std::byte const *>(std::data(message)), std::size(message)};

  // note! SBE is *not* const-safe

  // - message header
  message_header_type message_header{
      reinterpret_cast<char *>(const_cast<std::byte *>(std::data(payload))), std::size(payload)};
  auto message_header_length = message_header_type::encodedLength();
  CHECK(message_header_length == 8);
  auto block_length = message_header.blockLength();
  CHECK(block_length == 137);
  auto template_id = message_header.templateId();
  REQUIRE(template_id == 12);
  auto schema_id = message_header.schemaId();
  CHECK(schema_id == 1);
  auto version = message_header.version();
  CHECK(version == 1);

  // - message
  auto payload_2 = payload.subspan(message_header_length);
  mesage_type value{
      reinterpret_cast<char *>(const_cast<std::byte *>(std::data(payload_2))),
      std::size(payload_2),
      block_length,
      version};
  auto message_length = value.computeLength();
  CHECK(message_length == 137);
  CHECK((message_header_length + message_length) == std::size(message));
  value.sbeRewind();  // note! here it's not needed -- but with variable lists, it seems to be
  auto exchange = value.getExchangeAsStringView();
  CHECK(exchange == "deribit"sv);
  auto symbol = value.getSymbolAsStringView();
  CHECK(symbol == "BTC-PERPETUAL"sv);
  auto &layer = value.layer();
  CHECK(layer.bidPrice() == Catch::Approx{123.4});
  CHECK(layer.bidQuantity() == Catch::Approx{1.234});
  CHECK(layer.askPrice() == Catch::Approx{234.5});
  CHECK(layer.askQuantity() == Catch::Approx{2.345});
  auto update_type = value.updateType();
  CHECK(update_type == roq_sbe::UpdateType::Value::Incremental);
  auto exchange_time_utc = value.exchangeTimeUTC();
  CHECK(exchange_time_utc == 1234);
  auto exchange_sequence = value.exchangeSequence();
  CHECK(exchange_sequence == 2345);
  auto sending_time_utc = value.sendingTimeUTC();
  CHECK(sending_time_utc == 3456);
}
