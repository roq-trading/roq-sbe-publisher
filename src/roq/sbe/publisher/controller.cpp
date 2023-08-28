/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/sbe/publisher/controller.hpp"

#include <absl/base/internal/endian.h>

#include <cassert>

#include "roq/logging.hpp"

#include "roq/debug/hex/message.hpp"

#include "roq/sbe/codec/encoder.hpp"

using namespace std::literals;

namespace roq {
namespace sbe {
namespace publisher {

// === CONSTANTS ===

namespace {
auto const TIMER_FREQUENCY = 1s;
auto const MAX_BUFFER_SIZE = size_t{2048};
}  // namespace

// === HELPERS ===

namespace {
auto create_timer(auto &handler, auto &context) {
  return context.create_timer(handler, TIMER_FREQUENCY);
}

auto create_sender(auto &handler, auto &context, auto &settings, auto &multicast_address, auto &multicast_port) {
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

Controller::Controller(client::Dispatcher &dispatcher, Settings const &settings, Config const &, io::Context &context)
    : dispatcher_{dispatcher}, context_{context}, timer_{create_timer(*this, context_)},
      snapshot_{create_sender(
          *this, context_, settings, settings.multicast_address_snapshot, settings.multicast_port_snapshot)},
      incremental_{create_sender(
          *this, context_, settings, settings.multicast_address_incremental, settings.multicast_port_incremental)},
      buffer_(MAX_BUFFER_SIZE) {
  (*timer_).resume();
}

// client::Handler

void Controller::operator()(Event<Timer> const &) {
  context_.drain();
}

void Controller::operator()(Event<Connected> const &) {
}

void Controller::operator()(Event<Disconnected> const &) {
}

void Controller::operator()(Event<DownloadBegin> const &) {
}

void Controller::operator()(Event<DownloadEnd> const &) {
}

void Controller::operator()(Event<Ready> const &) {
}

void Controller::operator()(Event<ReferenceData> const &event) {
  auto &[message_info, reference_data] = event;
  if (reference_data.discard)
    return;
  auto message = codec::Encoder::encode(buffer_, reference_data);
  send(message);
}

void Controller::operator()(Event<MarketStatus> const &event) {
  auto &[message_info, market_status] = event;
  log::debug("market_status={}"sv, market_status);
  auto message = codec::Encoder::encode(buffer_, market_status);
  send(message);
}

void Controller::operator()(Event<TopOfBook> const &event) {
  auto &[message_info, top_of_book] = event;
  log::debug("top_of_book={}"sv, top_of_book);
  auto message = codec::Encoder::encode(buffer_, top_of_book);
  send(message);
}

// io::sys::Timer::Handler

void Controller::operator()(io::sys::Timer::Event const &) {
}

// io::net::udp::Sender::Handler

void Controller::operator()(io::net::udp::Sender::Error const &) {
  log::fatal("Unexpected"sv);
}

// utilities

void Controller::send(std::span<std::byte const> const &payload) {
  auto sequence_number = absl::little_endian::FromHost(++sequence_number_);
  log::debug("[{}] {}"sv, sequence_number, debug::hex::Message{payload});
  std::span header{reinterpret_cast<std::byte const *>(&sequence_number), sizeof(sequence_number)};
  std::array<std::span<std::byte const>, 2> message{{header, payload}};
  (*incremental_).send(message);
}

}  // namespace publisher
}  // namespace sbe
}  // namespace roq
