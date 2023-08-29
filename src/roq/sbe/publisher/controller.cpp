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
// auto const MAX_BUFFER_SIZE = size_t{2048};
auto const MAX_BUFFER_SIZE = size_t{1024 * 1024};
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
  log::debug("reference_data={}"sv, reference_data);
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

void Controller::operator()(Event<MarketByPriceUpdate> const &event) {
  auto &[message_info, market_by_price_update] = event;
  log::debug("market_by_price_update={}"sv, market_by_price_update);
  // XXX FIXME HACK !!!
  auto tmp = market_by_price_update;
  tmp.bids = {std::data(market_by_price_update.bids), std::min<size_t>(1024, std::size(market_by_price_update.bids))};
  tmp.asks = {std::data(market_by_price_update.asks), std::min<size_t>(1024, std::size(market_by_price_update.asks))};
  auto message = codec::Encoder::encode(buffer_, tmp);
  send(message);
}

void Controller::operator()(Event<MarketByOrderUpdate> const &event) {
  auto &[message_info, market_by_order_update] = event;
  log::debug("market_by_order_update={}"sv, market_by_order_update);
  /*
  auto message = codec::Encoder::encode(buffer_, market_by_order_update);
  send(message);
  */
}

void Controller::operator()(Event<TradeSummary> const &event) {
  auto &[message_info, trade_summary] = event;
  log::debug("trade_summary={}"sv, trade_summary);
  auto message = codec::Encoder::encode(buffer_, trade_summary);
  send(message);
}

void Controller::operator()(Event<StatisticsUpdate> const &event) {
  auto &[message_info, statistics_update] = event;
  log::debug("statistics_update={}"sv, statistics_update);
  auto message = codec::Encoder::encode(buffer_, statistics_update);
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
  auto length = std::size(message[0]) + std::size(message[1]);
  auto bytes = (*incremental_).send(message);
  if (!bytes) {
    log::warn("DROP sequence_number={}, bytes={}, length={}"sv, sequence_number_, bytes, length);
  } else if (bytes > 1400) {
    log::warn("DROP sequence_number={}, bytes={}, length={} (message too large)"sv, sequence_number_, bytes, length);
  }
}

}  // namespace publisher
}  // namespace sbe
}  // namespace roq
