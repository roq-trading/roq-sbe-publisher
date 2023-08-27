/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include <catch2/catch_all.hpp>

#include "roq/debug/hex/message.hpp"

#include "roq_sbe/MessageHeader.h"
#include "roq_sbe/ReferenceData.h"

using namespace std::literals;

TEST_CASE("reference_data_encode", "[reference_data]") {
  using mesage_type = roq_sbe::ReferenceData;
  using message_header_type = roq_sbe::MessageHeader;

  std::vector<std::byte> buffer(4096);

  mesage_type value;

  auto &result = value.wrapAndApplyHeader(reinterpret_cast<char *>(std::data(buffer)), 0, std::size(buffer));

  result.putExchange("deribit"sv);
  result.putSymbol("BTC-PERPETUAL"sv);
  result.putDescription("AbcDef123"sv);
  result.security_type(roq_sbe::SecurityType::Value::Swap);  // XXX
  result.putBaseCurrency("BTC"sv);
  result.putQuoteCurrency("USD"sv);
  result.putMarginCurrency("USD"sv);
  result.putCommissionCurrency("USD"sv);
  result.tickSize(0.05);
  result.multiplier(10.0);
  result.minNotional(1.0);
  result.minTradeVol(1.0);
  result.maxTradeVol(999.0);
  result.tradeVolStepSize(1.0);
  result.optionType(roq_sbe::OptionType::Value::Call);
  result.putStrikeCurrency("USD"sv);
  result.strikePrice(123.4);
  result.putUnderlying("BTC-INDEX"sv);
  result.putTimeZone("GMT"sv);
  result.issueDate(1234);
  result.settlementDate(2345);
  result.expiryDateTime(3456);
  result.expiryDateTimeUTC(4567);

  auto length = message_header_type::encodedLength() + mesage_type::computeLength();
  CHECK(length == 538);  // see next test

  std::span message{std::data(buffer), length};

  fmt::print("{}\n"sv, roq::debug::hex::Message{message});
}

TEST_CASE("reference_data_decode", "[reference_data]") {
  using mesage_type = roq_sbe::ReferenceData;
  using message_header_type = roq_sbe::MessageHeader;

  auto const message =
      "\x12\x02"  // block length (530)
      "\x0a\x00"  // template id (10)
      "\x01\x00"  // schema id (1)
      "\x01\x00"  // version (1)
      "\x64\x65\x72\x69\x62\x69\x74\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"  // exchange
      "\x42\x54\x43\x2d\x50\x45\x52\x50\x45\x54\x55\x41\x4c\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x00\x00"  // symbol
      "\x41\x62\x63\x44\x65\x66\x31\x32\x33\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x00\x00"  // description
      "\x04"                              // security type (swap)
      "\x42\x54\x43\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"  // base currency
      "\x55\x53\x44\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"  // quote currency
      "\x55\x53\x44\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"  // margin currency
      "\x55\x53\x44\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"  // commission currency
      "\x9a\x99\x99\x99\x99\x99\xa9\x3f"                  // tick size
      "\x00\x00\x00\x00\x00\x00\x24\x40"                  // multiplier
      "\x00\x00\x00\x00\x00\x00\xf0\x3f"                  // min notional
      "\x00\x00\x00\x00\x00\x00\xf0\x3f"                  // min trade vol
      "\x00\x00\x00\x00\x00\x38\x8f\x40"                  // max trade vol
      "\x00\x00\x00\x00\x00\x00\xf0\x3f"                  // trade vol step size
      "\x01"                                              // option type (call)
      "\x55\x53\x44\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"  // strike currency
      "\x9a\x99\x99\x99\x99\xd9\x5e\x40"                  // strike price
      "\x42\x54\x43\x2d\x49\x4e\x44\x45\x58\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x00\x00"  // underlying
      "\x47\x4d\x54\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
      "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"  // time zone
      "\xd2\x04\x00\x00"                                  // issue date
      "\x29\x09\x00\x00"                                  // settlement date
      "\x80\x0d\x00\x00\x00\x00\x00\x00"                  // expiry datetime
      "\xd7\x11\x00\x00\x00\x00\x00\x00"sv;               // expiry datetime utc
  REQUIRE(std::size(message) == 538);

  std::span payload{reinterpret_cast<std::byte const *>(std::data(message)), std::size(message)};

  // note! SBE is *not* const-safe

  // - message header
  message_header_type message_header{
      reinterpret_cast<char *>(const_cast<std::byte *>(std::data(payload))), std::size(payload)};
  auto message_header_length = message_header_type::encodedLength();
  CHECK(message_header_length == 8);
  auto block_length = message_header.blockLength();
  CHECK(block_length == 530);
  auto template_id = message_header.templateId();
  REQUIRE(template_id == 10);
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
  CHECK(message_length == 530);
  CHECK((message_header_length + message_length) == std::size(message));
  value.sbeRewind();  // note! here it's not needed -- but with variable lists, it seems to be
  auto exchange = value.getExchangeAsStringView();
  CHECK(exchange == "deribit"sv);
  auto symbol = value.getSymbolAsStringView();
  CHECK(symbol == "BTC-PERPETUAL"sv);
}
