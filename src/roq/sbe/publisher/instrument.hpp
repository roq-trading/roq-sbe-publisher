/* Copyright (c) 2017-2024, Hans Erik Thrane */

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
  Instrument(uint32_t instrument_id, uint16_t object_id, std::string_view const &exchange, std::string_view const &symbol);

  void operator()(Event<ReferenceData> const &, uint32_t sequence_number);
  void operator()(Event<MarketStatus> const &, uint32_t sequence_number);
  void operator()(Event<TopOfBook> const &, uint32_t sequence_number);
  void operator()(Event<MarketByPriceUpdate> const &, uint32_t sequence_number);
  void operator()(Event<MarketByOrderUpdate> const &, uint32_t sequence_number);
  void operator()(Event<TradeSummary> const &, uint32_t sequence_number);
  void operator()(Event<StatisticsUpdate> const &, uint32_t sequence_number);

  operator ReferenceData() const;
  operator MarketStatus() const;
  template <typename Callback>
  void create_market_by_price_snapshot(auto &bids, auto &asks, Callback callback) const {
    (*market_by_price_).create_snapshot(bids, asks, callback);
  }
  operator StatisticsUpdate() const;

  auto &get_market_by_price() const { return *market_by_price_; }

  uint32_t const instrument_id;
  uint16_t const object_id;

  Exchange const exchange;
  Symbol const symbol;

  struct {
    uint32_t reference_data = {};
    uint32_t market_status = {};
    uint32_t top_of_book = {};
    uint32_t market_by_price = {};
    uint32_t market_by_order = {};
    uint32_t trade_summary = {};
    uint32_t statistics = {};
  } last_sequence_number = {};

 private:
  cache::ReferenceData reference_data_;
  cache::MarketStatus market_status_;
  std::unique_ptr<cache::MarketByPrice> market_by_price_;
  cache::Statistics statistics_;
};

}  // namespace publisher
}  // namespace sbe
}  // namespace roq
