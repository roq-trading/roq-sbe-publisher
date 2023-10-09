/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

#include "roq/io/context.hpp"

#include "roq/codec/sbe/encoder.hpp"

#include "roq/sbe/publisher/base.hpp"
#include "roq/sbe/publisher/instrument.hpp"
#include "roq/sbe/publisher/settings.hpp"
#include "roq/sbe/publisher/shared.hpp"

namespace roq {
namespace sbe {
namespace publisher {

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
  Shared &shared_;
  std::unique_ptr<codec::sbe::Encoder> encoder_;
};

}  // namespace publisher
}  // namespace sbe
}  // namespace roq
