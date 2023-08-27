/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/client.hpp"

#include "roq/io/context.hpp"

#include "roq/io/sys/timer.hpp"

#include "roq/io/net/udp/sender.hpp"

#include "roq/sbe/publisher/config.hpp"
#include "roq/sbe/publisher/settings.hpp"

namespace roq {
namespace sbe {
namespace publisher {

struct Controller final : public client::Handler, public io::sys::Timer::Handler, public io::net::udp::Sender::Handler {
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
  void operator()(Event<TopOfBook> const &) override;

  // io::sys::Timer::Handler
  void operator()(io::sys::Timer::Event const &) override;

  // io::net::udp::Sender::Handler
  void operator()(io::net::udp::Sender::Error const &) override;

  // utilities

  void send(std::span<std::byte const> const &payload);

 private:
  client::Dispatcher &dispatcher_;
  io::Context &context_;
  std::unique_ptr<io::sys::Timer> timer_;
  std::unique_ptr<io::Sender> snapshot_;
  std::unique_ptr<io::Sender> incremental_;
  // EXPERIMENTAL
  uint32_t sequence_number_ = {};
};

}  // namespace publisher
}  // namespace sbe
}  // namespace roq
