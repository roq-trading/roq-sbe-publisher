/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

#include "roq/cache/market_status.hpp"
#include "roq/cache/reference_data.hpp"

namespace roq {
namespace sbe {
namespace publisher {

struct Instrument final {
  Instrument(std::string_view const &exchange, std::string_view const &symbol);

  void operator()(Event<ReferenceData> const &);
  void operator()(Event<MarketStatus> const &);

  Exchange const exchange;
  Symbol const symbol;

  cache::ReferenceData reference_data;
  cache::MarketStatus market_status;
};

}  // namespace publisher
}  // namespace sbe
}  // namespace roq
