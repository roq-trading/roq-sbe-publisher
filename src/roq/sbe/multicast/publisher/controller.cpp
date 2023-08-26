/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/sbe/multicast/publisher/controller.hpp"

#include <absl/base/internal/endian.h>

#include <cassert>

#include "roq/logging.hpp"

#include "roq/json/top_of_book.hpp"

using namespace std::literals;

namespace roq {
namespace sbe {
namespace multicast {
namespace publisher {

// === CONSTANTS ===

namespace {
auto const TIMER_FREQUENCY = 1s;
}

// === HELPERS ===

namespace {
auto create_timer(auto &handler, auto &context) {
  return context.create_timer(handler, TIMER_FREQUENCY);
}

auto create_sender(auto &handler, auto &context, auto &settings) {
  if (std::empty(settings.multicast_address))
    log::fatal("Unexpected: address is missing"sv);
  if (!settings.multicast_port)
    log::fatal("Unexpected: port is missing"sv);
  auto network_address = io::NetworkAddress::create_blocking(settings.multicast_address, settings.multicast_port);
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
      sender_{create_sender(*this, context_, settings)} {
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

void Controller::operator()(Event<ReferenceData> const &) {
}

void Controller::operator()(Event<TopOfBook> const &event) {
  auto &[message_info, top_of_book] = event;
  // XXX TODO SBE encoding
  auto context = json::Context{
      .price_decimals = {},
      .quantity_decimals = {},
  };
  auto message = fmt::format("{}"sv, json::TopOfBook(context, top_of_book));
  log::info(R"(message="{}")"sv, message);
  std::span payload{reinterpret_cast<std::byte const *>(std::data(message)), std::size(message)};
  send(payload);
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
  std::span header{reinterpret_cast<std::byte const *>(&sequence_number), sizeof(sequence_number)};
  std::array<std::span<std::byte const>, 2> message{{header, payload}};
  (*sender_).send(message);
}

}  // namespace publisher
}  // namespace multicast
}  // namespace sbe
}  // namespace roq
