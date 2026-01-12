/* Copyright (c) 2017-2026, Hans Erik Thrane */

#pragma once

#include <memory>

#include "roq/client/poller.hpp"

#include "roq/io/context.hpp"

#include "roq/service/dispatcher.hpp"

#include "roq/sbe_publisher/config.hpp"
#include "roq/sbe_publisher/incremental.hpp"
#include "roq/sbe_publisher/settings.hpp"
#include "roq/sbe_publisher/shared.hpp"
#include "roq/sbe_publisher/snapshot.hpp"

namespace roq {
namespace sbe_publisher {

struct Controller final : public io::sys::Signal::Handler, public service::Dispatcher::Handler, public client::Poller::Handler {
  Controller(Settings const &, Config const &, io::Context &context, std::span<std::string_view const> const &params);

  Controller(Controller const &) = delete;

  void dispatch();

 protected:
  void refresh(std::chrono::nanoseconds now);

  // io::sys::Signal::Handler
  void operator()(io::sys::Signal::Event const &) override;

  // service::Dispatcher::Handler
  void operator()(metrics::Writer &) const override;
  void operator()(service::Disconnected const &) override;
  void operator()(service::Response &, service::Request const &) override;
  void operator()(Control const &, uint8_t user_id) override;

  // client::Poller::Handler
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
  std::unique_ptr<io::sys::Signal> terminate_;
  std::unique_ptr<io::sys::Signal> interrupt_;
  std::unique_ptr<service::Dispatcher> service_;
  std::unique_ptr<client::Poller> dispatcher_;
  Shared shared_;
  Incremental incremental_;
  Snapshot snapshot_;
  std::chrono::nanoseconds next_yield_ = {};
};

}  // namespace sbe_publisher
}  // namespace roq
