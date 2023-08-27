/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include <catch2/catch_all.hpp>

#include "roq/debug/hex/message.hpp"

#include "roq_sbe/MarketStatus.h"
#include "roq_sbe/MessageHeader.h"

using namespace std::literals;

TEST_CASE("market_status_encode", "[market_status]") {
  using mesage_type = roq_sbe::MarketStatus;
  using message_header_type = roq_sbe::MessageHeader;

  std::vector<std::byte> buffer(4096);

  mesage_type value;

  auto &result = value.wrapAndApplyHeader(reinterpret_cast<char *>(std::data(buffer)), 0, std::size(buffer));

  result.putExchange("deribit"sv);
  result.putSymbol("BTC-PERPETUAL"sv);
  result.tradingStatus(roq_sbe::TradingStatus::Value::Open);

  auto length = message_header_type::encodedLength() + mesage_type::computeLength();
  CHECK(length == 89);  // see next test

  std::span message{std::data(buffer), length};

  fmt::print("{}\n"sv, roq::debug::hex::Message{message});
}

TEST_CASE("market_status_decode", "[market_status]") {
  using mesage_type = roq_sbe::MarketStatus;
  using message_header_type = roq_sbe::MessageHeader;

  auto const message =
      "\x51\x00"  // block length (81)
      "\x0b\x00"  // template id (11)
      "\x01\x00"  // schema id (1)
      "\x01\x00"  // version (1)
      "\x64\x65\x72\x69\x62\x69\x74\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"  // exchange
      "\x42\x54\x43\x2d\x50\x45\x52\x50\x45\x54\x55\x41\x4c\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x00\x00"  // symbol
      "\x05"sv;                           // trading status (open)
  REQUIRE(std::size(message) == 89);

  std::span payload{reinterpret_cast<std::byte const *>(std::data(message)), std::size(message)};

  // note! SBE is *not* const-safe

  // - message header
  message_header_type message_header{
      reinterpret_cast<char *>(const_cast<std::byte *>(std::data(payload))), std::size(payload)};
  auto message_header_length = message_header_type::encodedLength();
  CHECK(message_header_length == 8);
  auto block_length = message_header.blockLength();
  CHECK(block_length == 81);
  auto template_id = message_header.templateId();
  REQUIRE(template_id == 11);
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
  CHECK(message_length == 81);
  CHECK((message_header_length + message_length) == std::size(message));
  value.sbeRewind();  // note! here it's not needed -- but with variable lists, it seems to be
  auto exchange = value.getExchangeAsStringView();
  CHECK(exchange == "deribit"sv);
  auto symbol = value.getSymbolAsStringView();
  CHECK(symbol == "BTC-PERPETUAL"sv);
  auto trading_status = value.tradingStatus();
  CHECK(trading_status == roq_sbe::TradingStatus::Value::Open);
}
