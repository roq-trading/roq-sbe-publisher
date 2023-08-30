/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/sbe/publisher/base.hpp"

#include <absl/base/internal/endian.h>

#include "roq/clock.hpp"

#include "roq/logging.hpp"

#include "roq/utils/math.hpp"

#include "roq/debug/hex/message.hpp"

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
auto create_session_id() {
  auto now_utc = clock::get_realtime<std::chrono::seconds>();
  return static_cast<uint16_t>(now_utc.count());
}

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
    Settings const &settings, io::Context &context, std::string_view const &multicast_address, uint16_t multicast_port)
    : encode_buffer_(settings.encode_buffer_size), session_id_{create_session_id()},
      sender_{create_sender(*this, settings, context, multicast_address, multicast_port)} {
  log::info(
      "session_id={}"sv,
      debug::hex::Message{std::span{reinterpret_cast<std::byte const *>(&session_id_), sizeof(session_id_)}});
}

// io::net::udp::Sender::Handler

void Base::operator()(io::net::udp::Sender::Error const &) {
  log::fatal("Unexpected"sv);
}

// utilities

void Base::send(std::span<std::byte const> const &payload) {
  auto total_fragments = utils::round_up<MAX_PAYLOAD_SIZE>(std::size(payload)) / MAX_PAYLOAD_SIZE;
  auto fragment_number_max = std::max<size_t>(1, total_fragments) - 1;
  for (size_t index = 0; index < total_fragments; ++index) {
    auto offset = index * MAX_PAYLOAD_SIZE;
    auto length = std::min(std::size(payload) - offset, MAX_PAYLOAD_SIZE);
    auto payload_2 = payload.subspan(offset, length);
    struct __attribute__((packed)) Header final {
      uint16_t session_id;
      uint32_t sequence_number;
      uint8_t fragment;
      uint8_t fragment_max;
    } header{
        .session_id = session_id_,  // note! random number => byte ordering not important
        .sequence_number = absl::little_endian::FromHost(++sequence_number_),
        .fragment = static_cast<uint8_t>(index),
        .fragment_max = static_cast<uint8_t>(fragment_number_max),
    };
    static_assert(sizeof(Header) == 8);
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
