/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/io/context.hpp"

#include "roq/io/net/udp/sender.hpp"

#include "roq/sbe/publisher/settings.hpp"
#include "roq/sbe/publisher/shared.hpp"

namespace roq {
namespace sbe {
namespace publisher {

struct Base : public io::net::udp::Sender::Handler {
  Base(Base &&) = default;
  Base(Base const &) = delete;

 protected:
  Base(Settings const &, io::Context &, Shared &, std::string_view const &multicast_address, uint16_t multicast_port);

  // io::net::udp::Sender::Handler
  void operator()(io::net::udp::Sender::Error const &) override;

  // utilities

  void send(std::span<std::byte const> const &payload);

  std::vector<std::byte> encode_buffer_;

 private:
  Shared const &shared_;
  std::unique_ptr<io::Sender> const sender_;
  uint32_t sequence_number_ = {};
};

}  // namespace publisher
}  // namespace sbe
}  // namespace roq
