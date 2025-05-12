/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/sbe_publisher/base.hpp"

#include "roq/logging.hpp"

#include "roq/utils/math.hpp"

#include "roq/utils/debug/hex/message.hpp"

#include "roq/codec/udp/encoder.hpp"

#include "roq/codec/sbe/encoder.hpp"

using namespace std::literals;

namespace roq {
namespace sbe_publisher {

// === CONSTANTS ===

namespace {
constexpr auto const MAX_PAYLOAD_SIZE = 1400UZ;
}  // namespace

// === HELPERS ===

namespace {
template <typename R>
R create_sender(auto &handler, auto &settings, auto &context, auto &multicast_address, auto multicast_port) {
  using result_type = std::remove_cvref_t<R>;
  result_type result;
  if (std::empty(settings.multicast.local_interface)) {
    log::fatal("Unexpected: local_interface is missing"sv);
  }
  if (!multicast_port) {
    log::fatal("Unexpected: port is missing"sv);
  }
  auto socket_options = Mask{
      io::SocketOption::REUSE_ADDRESS,
      io::SocketOption::REUSE_PORT,
  };
  if (std::empty(multicast_address)) {
    for (auto &item : settings.multicast.local_interface) {
      log::warn(R"(Using UDP (local_interface="{}", port={}))"sv, item, multicast_port);
      auto network_address = io::NetworkAddress::create_blocking(item, multicast_port);
      auto sender = context.create_udp_sender(handler, network_address, socket_options);
      result.emplace_back(std::move(sender));
    }
  } else {
    if (std::size(settings.multicast.local_interface) > 1 && std::size(settings.multicast.local_interface) != std::size(multicast_address)) {
      log::fatal("Unexpected: len(local_interface) != len(multicast_address)"sv);
    }
    for (size_t i = 0; i < std::size(multicast_address); ++i) {
      auto &local_interface_2 = settings.multicast.local_interface[i % std::size(settings.multicast.local_interface)];
      auto &multicast_address_2 = multicast_address[i];
      log::warn(R"(Using multicast (local_interface="{}", port={}, multicast_address="{}"))"sv, local_interface_2, multicast_port, multicast_address_2);
      auto network_address = io::NetworkAddress::create_blocking(multicast_address_2, multicast_port);
      auto sender = context.create_multicast_sender(
          handler, network_address, socket_options, local_interface_2, settings.multicast.multicast_ttl, settings.multicast.multicast_loop);
      result.emplace_back(std::move(sender));
    }
  }
  return result;
}
}  // namespace

// === IMPLEMENTATION ===

Base::Base(Settings const &settings, io::Context &context, Shared &shared, std::span<std::string const> const &multicast_address, uint16_t multicast_port)
    : encode_buffer_(settings.encode_buffer_size), shared_{shared},
      sender_{create_sender<decltype(sender_)>(*this, settings, context, multicast_address, multicast_port)} {
}

void Base::operator()(io::net::udp::Sender::Error const &) {
  log::fatal("Unexpected"sv);
}

void Base::operator()(io::net::udp::Sender::Write const &) {
}

// utilities

void Base::send(std::span<std::byte const> const &payload, uint8_t control, uint8_t object_type, uint16_t object_id, uint32_t last_sequence_number) {
  auto total_fragments = utils::round_up<MAX_PAYLOAD_SIZE>(std::size(payload)) / MAX_PAYLOAD_SIZE;
  auto fragment_number_max = std::max<size_t>(1, total_fragments) - 1;
  for (size_t index = 0; index < total_fragments; ++index) {
    auto offset = index * MAX_PAYLOAD_SIZE;
    auto length = std::min(std::size(payload) - offset, MAX_PAYLOAD_SIZE);
    auto payload_2 = payload.subspan(offset, length);
    auto header = codec::udp::Header{
        .control = control,
        .object_type = object_type,
        .session_id = shared_.session_id,  // note! random number => byte ordering not important
        .sequence_number = ++sequence_number_,
        .fragment = static_cast<uint8_t>(index),
        .fragment_max = static_cast<uint8_t>(fragment_number_max),
        .object_id = object_id,
        .last_sequence_number = last_sequence_number,
    };
    codec::udp::Encoder::encode(header);
    log::info<1>("[{}:{}:{}] {}"sv, sequence_number_, header.fragment, header.fragment_max, utils::debug::hex::Message{payload_2});
    std::span header_2{reinterpret_cast<std::byte const *>(&header), sizeof(header)};
    std::array<std::span<std::byte const>, 2> message{{header_2, payload_2}};
    for (auto &sender : sender_) {
      if ((*sender).send([&](auto &buffer) {
            auto length = std::size(message[0]) + std::size(message[1]);
            if (std::size(buffer) < length) [[unlikely]] {
              log::fatal("Unexpected: {} < {}"sv, std::size(buffer), length);
            }
            std::memcpy(std::data(buffer), std::data(message[0]), std::size(message[0]));
            std::memcpy(std::data(buffer) + std::size(message[0]), std::data(message[1]), std::size(message[1]));
            return length;
          })) {
      } else {
        log::warn("HERE"sv);
      }
    }
  }
}

}  // namespace sbe_publisher
}  // namespace roq
