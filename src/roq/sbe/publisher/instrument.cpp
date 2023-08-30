/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/sbe/publisher/instrument.hpp"

using namespace std::literals;

namespace roq {
namespace sbe {
namespace publisher {

// === IMPLEMENTATION ===

Instrument::Instrument(std::string_view const &exchange, std::string_view const &symbol)
    : exchange{exchange}, symbol{symbol} {
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

}  // namespace publisher
}  // namespace sbe
}  // namespace roq
