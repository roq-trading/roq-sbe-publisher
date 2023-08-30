/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/client.hpp"

#include "roq/io/context.hpp"

#include "roq/sbe/publisher/config.hpp"
#include "roq/sbe/publisher/incremental.hpp"
#include "roq/sbe/publisher/settings.hpp"
#include "roq/sbe/publisher/shared.hpp"
#include "roq/sbe/publisher/snapshot.hpp"

namespace roq {
namespace sbe {
namespace publisher {

struct Controller final : public client::Handler {
  Controller(client::Dispatcher &, Settings const &, Config const &, io::Context &context);

  Controller(Controller &&) = default;
  Controller(Controller const &) = delete;

 protected:
  // client::Handler
  void operator()(Event<Timer> const &) override;
  void operator()(Event<Connected> const &) override;
  void operator()(Event<Disconnected> const &) override;
  void operator()(Event<DownloadBegin> const &) override;
  void operator()(Event<DownloadEnd> const &) override;
  void operator()(Event<Ready> const &) override;
  void operator()(Event<ReferenceData> const &) override;
  void operator()(Event<MarketStatus> const &) override;
  void operator()(Event<TopOfBook> const &) override;
  void operator()(Event<MarketByPriceUpdate> const &) override;
  void operator()(Event<MarketByOrderUpdate> const &) override;
  void operator()(Event<TradeSummary> const &) override;
  void operator()(Event<StatisticsUpdate> const &) override;

  template <typename T>
  void dispatch(Event<T> const &, bool ready);

 private:
  client::Dispatcher &dispatcher_;
  io::Context &context_;
  Shared shared_;
  Incremental incremental_;
  Snapshot snapshot_;
  bool ready_ = false;
};

}  // namespace publisher
}  // namespace sbe
}  // namespace roq
