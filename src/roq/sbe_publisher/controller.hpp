/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include "roq/client.hpp"

#include "roq/io/context.hpp"

#include "roq/sbe_publisher/config.hpp"
#include "roq/sbe_publisher/incremental.hpp"
#include "roq/sbe_publisher/settings.hpp"
#include "roq/sbe_publisher/shared.hpp"
#include "roq/sbe_publisher/snapshot.hpp"

namespace roq {
namespace sbe_publisher {

struct Controller final : public client::Handler {
  Controller(client::Dispatcher &, Settings const &, Config const &, io::Context &context);

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
  void dispatch(Event<T> const &);

 private:
  io::Context &context_;
  Shared shared_;
  Incremental incremental_;
  Snapshot snapshot_;
};

}  // namespace sbe_publisher
}  // namespace roq
