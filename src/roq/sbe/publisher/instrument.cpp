/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/sbe/publisher/instrument.hpp"

#include "roq/client.hpp"

using namespace std::literals;

namespace roq {
namespace sbe {
namespace publisher {

// === HELPERS ===

namespace {
auto create_market_by_price(auto &exchange, auto &symbol) {
  auto options = client::MarketByPriceFactory::Options{
      .disable_checksum_validation = {},
      .allow_price_inversion = {},
  };
  return client::MarketByPriceFactory::create_new(exchange, symbol, options);
}
}  // namespace

// === IMPLEMENTATION ===

Instrument::Instrument(std::string_view const &exchange, std::string_view const &symbol)
    : exchange{exchange}, symbol{symbol}, market_by_price{create_market_by_price(exchange, symbol)} {
}

void Instrument::operator()(Event<ReferenceData> const &event) {
  if (reference_data(event)) {
    // updated
  }
}

void Instrument::operator()(Event<MarketStatus> const &event) {
  if (market_status(event)) {
    // updated
  }
}

void Instrument::operator()(Event<MarketByPriceUpdate> const &event) {
  (*market_by_price)(event);
}

void Instrument::operator()(Event<StatisticsUpdate> const &event) {
  if (statistics(event)) {
    // updated
  }
}

}  // namespace publisher
}  // namespace sbe
}  // namespace roq
