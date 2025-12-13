/* Copyright (c) 2017-2026, Hans Erik Thrane */

#pragma once

#include <vector>

#include "roq/api.hpp"

#include "roq/io/context.hpp"

#include "roq/codec/simple_binary_encoding/encoder.hpp"

#include "roq/sbe_publisher/base.hpp"
#include "roq/sbe_publisher/instrument.hpp"
#include "roq/sbe_publisher/settings.hpp"
#include "roq/sbe_publisher/shared.hpp"

namespace roq {
namespace sbe_publisher {

struct Incremental final : public Base {
  Incremental(Settings const &, io::Context &context, Shared &);

  void operator()(Instrument const &, Event<ReferenceData> const &);
  void operator()(Instrument const &, Event<MarketStatus> const &);
  void operator()(Instrument const &, Event<TopOfBook> const &);
  void operator()(Instrument const &, Event<MarketByPriceUpdate> const &);
  void operator()(Instrument const &, Event<MarketByOrderUpdate> const &);
  void operator()(Instrument const &, Event<TradeSummary> const &);
  void operator()(Instrument const &, Event<StatisticsUpdate> const &);

 private:
  std::unique_ptr<codec::simple_binary_encoding::Encoder> encoder_;
  size_t const max_depth_;
  std::vector<MBPUpdate> bids_, asks_;
};

}  // namespace sbe_publisher
}  // namespace roq
