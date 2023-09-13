/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/sbe/publisher/base.hpp"

#include "roq/logging.hpp"

#include "roq/utils/math.hpp"

#include "roq/debug/hex/message.hpp"

#include "roq/codec/sbe/encoder.hpp"
#include "roq/codec/sbe/header.hpp"

using namespace std::literals;

namespace roq {
namespace sbe {
namespace publisher {

// === CONSTANTS ===

namespace {
constexpr auto const MAX_PAYLOAD_SIZE = size_t{1400};
}  // namespace

// === HELPERS ===

namespace {
auto create_sender(auto &handler, auto &settings, auto &context, auto &multicast_address, auto multicast_port) {
  if (std::empty(multicast_address))
    log::fatal("Unexpected: address is missing"sv);
  if (!multicast_port)
    log::fatal("Unexpected: port is missing"sv);
  auto network_address = io::NetworkAddress::create_blocking(multicast_address, multicast_port);
  auto socket_options = Mask{
      io::SocketOption::REUSE_ADDRESS,
  };
  return context.create_multicast_sender(
      handler,
      network_address,
      socket_options,
      settings.local_interface,
      settings.multicast_ttl,
      settings.multicast_loop);
}
}  // namespace

// === IMPLEMENTATION ===

Base::Base(
    Settings const &settings,
    io::Context &context,
    Shared &shared,
    std::string_view const &multicast_address,
    uint16_t multicast_port)
    : encode_buffer_(settings.encode_buffer_size), shared_{shared},
      sender_{create_sender(*this, settings, context, multicast_address, multicast_port)} {
}

// io::net::udp::Sender::Handler

void Base::operator()(io::net::udp::Sender::Error const &) {
  log::fatal("Unexpected"sv);
}

// utilities

void Base::send(
    std::span<std::byte const> const &payload,
    uint8_t control,
    uint8_t object_type,
    uint16_t object_id,
    uint32_t last_sequence_number) {
  auto total_fragments = utils::round_up<MAX_PAYLOAD_SIZE>(std::size(payload)) / MAX_PAYLOAD_SIZE;
  auto fragment_number_max = std::max<size_t>(1, total_fragments) - 1;
  for (size_t index = 0; index < total_fragments; ++index) {
    auto offset = index * MAX_PAYLOAD_SIZE;
    auto length = std::min(std::size(payload) - offset, MAX_PAYLOAD_SIZE);
    auto payload_2 = payload.subspan(offset, length);
    auto header = codec::sbe::Header{
        .control = control,
        .object_type = object_type,
        .session_id = shared_.session_id,  // note! random number => byte ordering not important
        .sequence_number = ++sequence_number_,
        .fragment = static_cast<uint8_t>(index),
        .fragment_max = static_cast<uint8_t>(fragment_number_max),
        .object_id = object_id,
        .last_sequence_number = last_sequence_number,
    };
    codec::sbe::Encoder::encode(header);
    log::info<1>(
        "[{}:{}:{}] {}"sv, sequence_number_, header.fragment, header.fragment_max, debug::hex::Message{payload_2});
    std::span header_2{reinterpret_cast<std::byte const *>(&header), sizeof(header)};
    std::array<std::span<std::byte const>, 2> message{{header_2, payload_2}};
    auto bytes = (*sender_).send(message);
    if (!bytes) {
      log::warn("DROP sequence_number={}, bytes={}"sv, sequence_number_, bytes);
    }
  }
}

}  // namespace publisher
}  // namespace sbe
}  // namespace roq
