/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <span>

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

  bool ready() const { return shared_.ready(); }

  uint32_t get_sequence_number() const { return sequence_number_; }

 protected:
  Base(
      Settings const &,
      io::Context &,
      Shared &,
      std::span<std::string const> const &multicast_address,
      uint16_t multicast_port);

  // io::net::udp::Sender::Handler
  void operator()(io::net::udp::Sender::Error const &) override;

  // utilities

  void send(
      std::span<std::byte const> const &payload,
      uint8_t control,
      uint8_t object_type,
      uint16_t object_id,
      uint32_t last_sequence_number);

  std::vector<std::byte> encode_buffer_;

 private:
  Shared const &shared_;
  std::vector<std::unique_ptr<io::Sender>> sender_;
  uint32_t sequence_number_ = {};
};

}  // namespace publisher
}  // namespace sbe
}  // namespace roq
