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

TEST_CASE("market_status_encode", "[market_status]") {
  std::vector<std::byte> buffer(4096);
  auto market_status = MarketStatus{
      .exchange = EXCHANGE,
      .symbol = SYMBOL,
      .trading_status = TradingStatus::OPEN,
  };
  auto message_1 = sbe::codec::Encoder::encode(buffer, market_status);
  fmt::print("{}\n"sv, roq::debug::hex::Message{message_1});
  REQUIRE(std::size(message_1) == 89);
  std::string_view text_1{reinterpret_cast<char const *>(std::data(message_1)), std::size(message_1)};
  auto const text_2 =
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
  REQUIRE(std::size(text_2) == 89);
  CHECK(text_1 == text_2);
  std::span message_2{reinterpret_cast<std::byte const *>(std::data(text_2)), std::size(text_2)};

  struct Handler final : public sbe::codec::Decoder::Handler {
    explicit Handler(MarketStatus const &source) : source_{source} {}
    void operator()(ReferenceData const &) { FAIL(); }
    void operator()(MarketStatus const &market_status) {
      found = true;
      CHECK(market_status.exchange == source_.exchange);
      CHECK(market_status.symbol == source_.symbol);
      CHECK(market_status.trading_status == source_.trading_status);
    }
    void operator()(TopOfBook const &) { FAIL(); }
    bool found = false;

   private:
    MarketStatus const &source_;
  } handler{market_status};
  auto bytes = sbe::codec::Decoder::decode(handler, message_2);
  CHECK(handler.found == true);
  CHECK(bytes == std::size(message_1));
}
