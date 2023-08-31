/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <memory>

#include "roq/api.hpp"

#include "roq/cache/market_by_price.hpp"
#include "roq/cache/market_status.hpp"
#include "roq/cache/reference_data.hpp"
#include "roq/cache/statistics.hpp"

namespace roq {
namespace sbe {
namespace publisher {

struct Instrument final {
  Instrument(std::string_view const &exchange, std::string_view const &symbol);

  void operator()(Event<ReferenceData> const &);
  void operator()(Event<MarketStatus> const &);
  void operator()(Event<MarketByPriceUpdate> const &);
  void operator()(Event<StatisticsUpdate> const &);

  Exchange const exchange;
  Symbol const symbol;

  cache::ReferenceData reference_data;
  cache::MarketStatus market_status;
  std::unique_ptr<cache::MarketByPrice> market_by_price;
  cache::Statistics statistics;
};

}  // namespace publisher
}  // namespace sbe
}  // namespace roq
