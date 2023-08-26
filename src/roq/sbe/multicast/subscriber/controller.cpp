/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/sbe/multicast/subscriber/controller.hpp"

#include <absl/base/internal/endian.h>

#include <cassert>

#include "roq/logging.hpp"

#include "roq/exceptions.hpp"
#include "roq/timer.hpp"

#include "roq/debug/hex/message.hpp"

#include "roq/io/reorder/factory.hpp"

using namespace std::literals;

namespace roq {
namespace sbe {
namespace multicast {
namespace subscriber {

// === CONSTANTS ===

namespace {
auto const TIMER_FREQUENCY = 100ms;
}  // namespace

// === HELPERS ===

namespace {
auto create_receiver(auto &handler, auto &settings, auto &context) {
  auto network_address = io::NetworkAddress{settings.multicast_port};
  auto socket_options = Mask{
      io::SocketOption::REUSE_ADDRESS,
  };
  auto receiver = context.create_udp_receiver(handler, network_address, socket_options);
  auto local_interface = io::NetworkAddress::create_blocking(settings.local_interface);
  auto multicast_address = io::NetworkAddress::create_blocking(settings.multicast_address);
  (*receiver).add_membership(multicast_address, local_interface);
  return receiver;
}

auto create_buffer(auto &handler, auto &settings) {
  auto options = io::reorder::Buffer::Options{
      .depth = settings.reorder_buffer_depth,
      .maximum_packet_size = settings.max_packet_size,
      .maximum_sequence_number = std::numeric_limits<uint32_t>::max(),
      .minimum_sequence_number = {},
  };
  return io::reorder::Factory::create(handler, options);
}
}  // namespace

// === IMPLEMENTATION ===

Controller::Controller(Settings const &settings, Config const &, io::Context &context)
    : context_{context}, terminate_{context.create_signal(*this, io::sys::Signal::Type::TERMINATE)},
      interrupt_{context.create_signal(*this, io::sys::Signal::Type::INTERRUPT)},
      timer_{context.create_timer(*this, TIMER_FREQUENCY)}, receiver_{create_receiver(*this, settings, context_)},
      buffer_{create_buffer(*this, settings)} {
}

void Controller::run() {
  log::info("Event loop is now running"sv);
  (*timer_).resume();
  context_.dispatch();
  log::info("Event loop has terminated"sv);
}

// io::sys::Signal::Handler

void Controller::operator()(io::sys::Signal::Event const &event) {
  log::warn("*** SIGNAL: {} ***"sv, magic_enum::enum_name(event.type));
  context_.stop();
}

// io::sys::Timer::Handler

void Controller::operator()(io::sys::Timer::Event const &) {
  // log::debug("HERE"sv);
}

// io::net::udp::Receiver::Handler

void Controller::operator()(io::net::udp::Receiver::Read const &read) {
  (*buffer_).drain(read.receiver);
}

void Controller::operator()(io::net::udp::Receiver::Error const &error) {
  log::warn("Error: what={}"sv, error.what);
}

// io::reorder::Buffer::Handler

uint64_t Controller::operator()(io::reorder::Buffer::GetSequenceNumber const &get_sequence_number) {
  auto &payload = get_sequence_number.payload;
  assert(!std::empty(payload));
  // note! parsing sequence number must be very cheap
  uint32_t tmp;
  if (std::size(payload) < sizeof(tmp))
    return {};  // XXX TODO or throw?
  std::memcpy(&tmp, std::data(payload), sizeof(tmp));
  auto sequence_number = absl::little_endian::FromHost(tmp);
  return sequence_number;
}

void Controller::operator()(io::reorder::Buffer::Parse const &parse) {
  log::info<1>("payload=[{}], sequence_number={}"sv, debug::hex::Message{parse.payload}, parse.sequence_number);
  // EXPERIMENTAL
  if (std::size(parse.payload) > 4) {
    std::string_view message{
        reinterpret_cast<char const *>(std::data(parse.payload) + 4), std::size(parse.payload) - 4};
    log::info(R"(message="{}")"sv, message);
  }
}

void Controller::operator()(io::reorder::Buffer::Reset const &) {
  log::warn("*** RESET ***"sv);
}

}  // namespace subscriber
}  // namespace multicast
}  // namespace sbe
}  // namespace roq
