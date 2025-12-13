/* Copyright (c) 2017-2026, Hans Erik Thrane */

#include "roq/sbe_publisher/instrument.hpp"

#include "roq/client.hpp"

using namespace std::literals;

namespace roq {
namespace sbe_publisher {

// === HELPERS ===

namespace {
auto create_market_by_price(auto &exchange, auto &symbol) {
  auto options = client::MarketByPriceFactory::Options{
      .disable_checksum_validation = {},
      .allow_price_inversion = {},
  };
  return client::MarketByPriceFactory::create_new(exchange, symbol, options);
}

struct Context final {
  std::string_view exchange;
  std::string_view symbol;
};
}  // namespace

// === IMPLEMENTATION ===

Instrument::Instrument(uint32_t instrument_id, uint16_t object_id, std::string_view const &exchange, std::string_view const &symbol)
    : instrument_id{instrument_id}, object_id{object_id}, exchange{exchange}, symbol{symbol}, market_by_price_{create_market_by_price(exchange, symbol)} {
}

void Instrument::operator()(Event<ReferenceData> const &event, uint32_t sequence_number) {
  last_sequence_number.reference_data = sequence_number;
  if (reference_data_(event)) {
    // updated
  }
}

void Instrument::operator()(Event<MarketStatus> const &event, uint32_t sequence_number) {
  last_sequence_number.market_status = sequence_number;
  if (market_status_(event)) {
    // updated
  }
}

void Instrument::operator()(Event<TopOfBook> const &, uint32_t sequence_number) {
  last_sequence_number.top_of_book = sequence_number;
  // not cached
}

void Instrument::operator()(Event<MarketByPriceUpdate> const &event, uint32_t sequence_number) {
  last_sequence_number.market_by_price = sequence_number;
  (*market_by_price_)(event);
}

void Instrument::operator()(Event<MarketByOrderUpdate> const &, uint32_t sequence_number) {
  last_sequence_number.market_by_order = sequence_number;
  // XXX TODO
}

void Instrument::operator()(Event<TradeSummary> const &, uint32_t sequence_number) {
  last_sequence_number.trade_summary = sequence_number;
  // not cached
}

void Instrument::operator()(Event<StatisticsUpdate> const &event, uint32_t sequence_number) {
  last_sequence_number.statistics = sequence_number;
  if (statistics_(event)) {
    // updated
  }
}

// ...

Instrument::operator ReferenceData() const {
  auto context = Context{
      .exchange = exchange,
      .symbol = symbol,
  };
  return reference_data_.convert(context);
}

Instrument::operator MarketStatus() const {
  auto context = Context{
      .exchange = exchange,
      .symbol = symbol,
  };
  return market_status_.convert(context);
}

Instrument::operator StatisticsUpdate() const {
  auto context = Context{
      .exchange = exchange,
      .symbol = symbol,
  };
  return statistics_.convert(context);
}

}  // namespace sbe_publisher
}  // namespace roq
