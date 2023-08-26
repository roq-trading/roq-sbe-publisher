/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <absl/container/flat_hash_map.h>

#include <chrono>
#include <memory>

#include "roq/io/context.hpp"

#include "roq/io/sys/signal.hpp"
#include "roq/io/sys/timer.hpp"

#include "roq/io/net/udp/receiver.hpp"

#include "roq/io/reorder/buffer.hpp"

#include "roq/sbe/multicast/subscriber/config.hpp"
#include "roq/sbe/multicast/subscriber/settings.hpp"

namespace roq {
namespace sbe {
namespace multicast {
namespace subscriber {

struct Controller final : public io::sys::Signal::Handler,
                          public io::sys::Timer::Handler,
                          public io::net::udp::Receiver::Handler,
                          public io::reorder::Buffer::Handler {
  Controller(Settings const &, Config const &, io::Context &);

  void run();

 protected:
  // io::sys::Signal::Handler
  void operator()(io::sys::Signal::Event const &) override;

  // io::sys::Timer::Handler
  void operator()(io::sys::Timer::Event const &) override;

  // io::net::udp::Receiver
  void operator()(io::net::udp::Receiver::Read const &) override;
  void operator()(io::net::udp::Receiver::Error const &) override;

  // io::reorder::Buffer::Handler
  uint64_t operator()(io::reorder::Buffer::GetSequenceNumber const &) override;
  void operator()(io::reorder::Buffer::Parse const &) override;
  void operator()(io::reorder::Buffer::Reset const &) override;

 private:
  io::Context &context_;
  std::unique_ptr<io::sys::Signal> const terminate_;
  std::unique_ptr<io::sys::Signal> const interrupt_;
  std::unique_ptr<io::sys::Timer> const timer_;
  std::unique_ptr<io::net::udp::Receiver> const receiver_;
  std::unique_ptr<io::reorder::Buffer> const buffer_;
};

}  // namespace subscriber
}  // namespace multicast
}  // namespace sbe
}  // namespace roq
