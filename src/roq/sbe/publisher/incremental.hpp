/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

#include "roq/io/context.hpp"

#include "roq/sbe/publisher/base.hpp"
#include "roq/sbe/publisher/settings.hpp"

namespace roq {
namespace sbe {
namespace publisher {

struct Incremental final : public Base {
  Incremental(Settings const &, io::Context &context);

  void operator()(Event<Timer> const &);
  void operator()(Event<Connected> const &);
  void operator()(Event<Disconnected> const &);
  void operator()(Event<Ready> const &);
  void operator()(Event<ReferenceData> const &);
  void operator()(Event<MarketStatus> const &);
  void operator()(Event<TopOfBook> const &);
  void operator()(Event<MarketByPriceUpdate> const &);
  void operator()(Event<MarketByOrderUpdate> const &);
  void operator()(Event<TradeSummary> const &);
  void operator()(Event<StatisticsUpdate> const &);

 private:
  bool ready_ = false;
};

}  // namespace publisher
}  // namespace sbe
}  // namespace roq
