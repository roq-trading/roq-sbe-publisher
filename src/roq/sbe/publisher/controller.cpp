/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/sbe/publisher/controller.hpp"

#include <absl/base/internal/endian.h>

#include <cassert>

#include "roq/logging.hpp"

#include "roq/utils/math.hpp"

#include "roq/debug/hex/message.hpp"

#include "roq/sbe/codec/encoder.hpp"

using namespace std::literals;

namespace roq {
namespace sbe {
namespace publisher {

// === CONSTANTS ===

namespace {
auto const TIMER_FREQUENCY = 1s;
constexpr auto const MAX_PAYLOAD_SIZE = size_t{1400};
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

auto create_session_id() {
  auto now_utc = clock::get_realtime<std::chrono::seconds>();
  return static_cast<uint16_t>(now_utc.count());
}
}  // namespace

// === IMPLEMENTATION ===

Controller::Controller(client::Dispatcher &dispatcher, Settings const &settings, Config const &, io::Context &context)
    : dispatcher_{dispatcher}, context_{context}, timer_{create_timer(*this, context_)},
      snapshot_{create_sender(
          *this, context_, settings, settings.multicast_address_snapshot, settings.multicast_port_snapshot)},
      incremental_{create_sender(
          *this, context_, settings, settings.multicast_address_incremental, settings.multicast_port_incremental)},
      buffer_(settings.encode_buffer_size), session_id_{create_session_id()} {
  log::info(
      "session_id={}"sv,
      debug::hex::Message{std::span{reinterpret_cast<std::byte const *>(&session_id_), sizeof(session_id_)}});
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
  auto message = codec::Encoder::encode(buffer_, market_status);
  send(message);
}

void Controller::operator()(Event<TopOfBook> const &event) {
  auto &[message_info, top_of_book] = event;
  auto message = codec::Encoder::encode(buffer_, top_of_book);
  send(message);
}

void Controller::operator()(Event<MarketByPriceUpdate> const &event) {
  auto &[message_info, market_by_price_update] = event;
  // XXX FIXME HACK !!!
  auto tmp = market_by_price_update;
  tmp.bids = {std::data(market_by_price_update.bids), std::min<size_t>(1024, std::size(market_by_price_update.bids))};
  tmp.asks = {std::data(market_by_price_update.asks), std::min<size_t>(1024, std::size(market_by_price_update.asks))};
  auto message = codec::Encoder::encode(buffer_, tmp);
  send(message);
}

void Controller::operator()(Event<MarketByOrderUpdate> const &event) {
  /*
  auto &[message_info, market_by_order_update] = event;
  auto message = codec::Encoder::encode(buffer_, market_by_order_update);
  send(message);
  */
}

void Controller::operator()(Event<TradeSummary> const &event) {
  auto &[message_info, trade_summary] = event;
  auto message = codec::Encoder::encode(buffer_, trade_summary);
  send(message);
}

void Controller::operator()(Event<StatisticsUpdate> const &event) {
  auto &[message_info, statistics_update] = event;
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
    auto bytes = (*incremental_).send(message);
    if (!bytes) {
      log::warn("DROP sequence_number={}, bytes={}"sv, sequence_number_, bytes);
    }
  }
}

}  // namespace publisher
}  // namespace sbe
}  // namespace roq
