/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include <catch2/catch_all.hpp>

#include "roq/debug/hex/message.hpp"

#include "roq/sbe/codec/decoder.hpp"
#include "roq/sbe/codec/encoder.hpp"

using namespace std::literals;

using namespace roq;

// === CONSTANTS ===

namespace {
auto const EXCHANGE = "deribit"sv;
auto const SYMBOL = "BTC-PERPETUAL"sv;
}  // namespace

// === IMPLEMENTATION ===

TEST_CASE("top_of_book_encode", "[top_of_book]") {
  std::vector<std::byte> buffer(4096);
  auto top_of_book = TopOfBook{
      .exchange = EXCHANGE,
      .symbol = SYMBOL,
      .layer =
          {
              .bid_price = 123.4,
              .bid_quantity = 1.234,
              .ask_price = 234.5,
              .ask_quantity = 2.345,
          },
      .update_type = UpdateType::INCREMENTAL,
      .exchange_time_utc = 1234ns,
      .exchange_sequence = 2345,
      .sending_time_utc = 3456ns,
  };
  auto message_1 = sbe::codec::Encoder::encode(buffer, top_of_book);
  fmt::print("{}\n"sv, roq::debug::hex::Message{message_1});
  REQUIRE(std::size(message_1) == 145);
  std::string_view text_1{reinterpret_cast<char const *>(std::data(message_1)), std::size(message_1)};
  auto const text_2 =
      "\x89\x00"  // block length (137)
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
  REQUIRE(std::size(text_2) == 145);
  CHECK(text_1 == text_2);
  std::span message_2{reinterpret_cast<std::byte const *>(std::data(text_2)), std::size(text_2)};

  struct Handler final : public sbe::codec::Decoder::Handler {
    explicit Handler(TopOfBook const &source) : source_{source} {}
    void operator()(ReferenceData const &) { FAIL(); }
    void operator()(MarketStatus const &) { FAIL(); }
    void operator()(TopOfBook const &top_of_book) {
      found = true;
      CHECK(top_of_book.exchange == source_.exchange);
      CHECK(top_of_book.symbol == source_.symbol);
      CHECK(top_of_book.layer.bid_price == Catch::Approx{source_.layer.bid_price});
      CHECK(top_of_book.layer.bid_quantity == Catch::Approx{source_.layer.bid_quantity});
      CHECK(top_of_book.layer.ask_price == Catch::Approx{source_.layer.ask_price});
      CHECK(top_of_book.layer.ask_quantity == Catch::Approx{source_.layer.ask_quantity});
      CHECK(top_of_book.update_type == source_.update_type);
      CHECK(top_of_book.exchange_time_utc == source_.exchange_time_utc);
      CHECK(top_of_book.exchange_sequence == source_.exchange_sequence);
      CHECK(top_of_book.sending_time_utc == source_.sending_time_utc);
    }
    bool found = false;

   private:
    TopOfBook const &source_;
  } handler{top_of_book};
  auto bytes = sbe::codec::Decoder::decode(handler, message_2);
  CHECK(handler.found == true);
  CHECK(bytes == std::size(message_1));
}
