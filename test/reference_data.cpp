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
auto const DESCRIPTION = "AbcDef123"sv;
}  // namespace

// === IMPLEMENTATION ===

TEST_CASE("reference_data_encode", "[reference_data]") {
  std::vector<std::byte> buffer(4096);
  auto reference_data = ReferenceData{
      .exchange = EXCHANGE,
      .symbol = SYMBOL,
      .description = DESCRIPTION,
      .security_type = SecurityType::SWAP,
      .base_currency = "BTC"sv,
      .quote_currency = "USD"sv,
      .margin_currency = "USD"sv,
      .commission_currency = "USD"sv,
      .tick_size = 0.05,
      .multiplier = 10.0,
      .min_notional = 1.0,
      .min_trade_vol = 1.0,
      .max_trade_vol = 999.0,
      .trade_vol_step_size = 1.0,
      .option_type = OptionType::CALL,
      .strike_currency = "USD"sv,
      .strike_price = 123.4,
      .underlying = "BTC-INDEX"sv,
      .time_zone = "GMT"sv,
      .issue_date = std::chrono::days{1234},
      .settlement_date = std::chrono::days{2345},
      .expiry_datetime = 3456s,
      .expiry_datetime_utc = 4567s,
  };
  auto message_1 = sbe::codec::Encoder::encode(buffer, reference_data);
  fmt::print("{}\n"sv, roq::debug::hex::Message{message_1});
  REQUIRE(std::size(message_1) == 530);
  std::string_view text_1{reinterpret_cast<char const *>(std::data(message_1)), std::size(message_1)};
  auto const text_2 =
      "\x0a\x02"  // block length (522)
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
      "\x80\x0d\x00\x00"                                  // expiry datetime
      "\xd7\x11\x00\x00"sv;                               // expiry datatime utc
  REQUIRE(std::size(text_2) == 530);
  CHECK(text_1 == text_2);
  std::span message_2{reinterpret_cast<std::byte const *>(std::data(text_2)), std::size(text_2)};

  struct Handler final : public sbe::codec::Decoder::Handler {
    explicit Handler(ReferenceData const &source) : source_{source} {}
    void operator()(ReferenceData const &reference_data) {
      found = true;
      CHECK(reference_data.exchange == source_.exchange);
      CHECK(reference_data.symbol == source_.symbol);
      CHECK(reference_data.description == source_.description);
      CHECK(reference_data.security_type == source_.security_type);
      CHECK(reference_data.base_currency == source_.base_currency);
      CHECK(reference_data.quote_currency == source_.quote_currency);
      CHECK(reference_data.margin_currency == source_.margin_currency);
      CHECK(reference_data.commission_currency == source_.commission_currency);
      CHECK(reference_data.tick_size == source_.tick_size);
      CHECK(reference_data.multiplier == source_.multiplier);
      CHECK(reference_data.min_notional == source_.min_notional);
      CHECK(reference_data.min_trade_vol == source_.min_trade_vol);
      CHECK(reference_data.max_trade_vol == source_.max_trade_vol);
      CHECK(reference_data.trade_vol_step_size == source_.trade_vol_step_size);
      CHECK(reference_data.option_type == source_.option_type);
      CHECK(reference_data.strike_currency == source_.strike_currency);
      CHECK(reference_data.strike_price == source_.strike_price);
      CHECK(reference_data.underlying == source_.underlying);
      CHECK(reference_data.time_zone == source_.time_zone);
      CHECK(reference_data.issue_date == source_.issue_date);
      CHECK(reference_data.settlement_date == source_.settlement_date);
      CHECK(reference_data.expiry_datetime == source_.expiry_datetime);
      CHECK(reference_data.expiry_datetime_utc == source_.expiry_datetime_utc);
    }
    void operator()(MarketStatus const &) { FAIL(); }
    void operator()(TopOfBook const &) { FAIL(); }
    bool found = false;

   private:
    ReferenceData const &source_;
  } handler{reference_data};
  auto bytes = sbe::codec::Decoder::decode(handler, message_2);
  CHECK(handler.found == true);
  CHECK(bytes == std::size(message_1));
}
