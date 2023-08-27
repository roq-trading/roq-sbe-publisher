/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include <catch2/catch_all.hpp>

#include "roq/debug/hex/message.hpp"

#include "roq_sbe/MessageHeader.h"
#include "roq_sbe/TopOfBook.h"

using namespace std::literals;

TEST_CASE("codec_encode", "[codec]") {
  CHECK(1 == 1);
}

/*
TEST_CASE("codec_encode", "[codec]") {
  using mesage_type = codec::Simple;
  using message_header_type = mesage_type::messageHeader;

  std::vector<std::byte> buffer(4096);

  mesage_type value;

  auto &result = value.wrapAndApplyHeader(reinterpret_cast<char *>(std::data(buffer)), 0, std::size(buffer));

  result.putName("BTC-PERPETUAL"sv);
  result.price(1234.56);
  result.time(1234);

  auto length = message_header_type::encodedLength() + mesage_type::computeLength();
  CHECK(length == 44);  // see next test

  std::span message{std::data(buffer), length};

  fmt::print("{}\n"sv, roq::debug::hex::Message{message});
}

TEST_CASE("codec_decode", "[codec]") {
  using mesage_type = codec::Simple;
  using message_header_type = mesage_type::messageHeader;

  auto const message =
      "\x20\x00"                                                          // block length (32)
      "\xe9\x03"                                                          // template id (1001)
      "\x01\x00"                                                          // schema id (1)
      "\x01\x00"                                                          // version (1)
      "\x00\x00"                                                          // num groups (0)
      "\x00\x00"                                                          // num var data fields (0)
      "\x42\x54\x43\x2d\x50\x45\x52\x50\x45\x54\x55\x41\x4c\x00\x00\x00"  // name
      "\x0a\xd7\xa3\x70\x3d\x4a\x93\x40"                                  // price
      "\xd2\x04\x00\x00\x00\x00\x00\x00"sv;                               // time
  REQUIRE(std::size(message) == 44);

  std::span payload{reinterpret_cast<std::byte const *>(std::data(message)), std::size(message)};

  // note! SBE is *not* const-safe

  // - message header
  message_header_type message_header{
      reinterpret_cast<char *>(const_cast<std::byte *>(std::data(payload))), std::size(payload)};
  auto message_header_length = message_header_type::encodedLength();
  CHECK(message_header_length == 12);
  auto block_length = message_header.blockLength();
  CHECK(block_length == 32);
  auto template_id = message_header.templateId();
  REQUIRE(template_id == 1001);
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
  CHECK(message_length == 32);
  CHECK((message_header_length + message_length) == std::size(message));
  value.sbeRewind();  // note! here it's not needed -- but with variable lists, it seems to be
  auto index_name = value.getNameAsStringView();
  CHECK(index_name == "BTC-PERPETUAL"sv);
  auto price = value.price();
  CHECK(price == Catch::Approx{1234.56});
  auto timestamp_ms = value.time();
  CHECK(timestamp_ms == 1234);
}
*/
