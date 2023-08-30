/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/sbe/publisher/incremental.hpp"

#include "roq/logging.hpp"

#include "roq/sbe/codec/encoder.hpp"

using namespace std::literals;

namespace roq {
namespace sbe {
namespace publisher {

// === IMPLEMENTATION ===

Incremental::Incremental(Settings const &settings, io::Context &context, Shared &shared)
    : Base{settings, context, shared, settings.multicast_address_incremental, settings.multicast_port_incremental} {
}

void Incremental::operator()(Event<Timer> const &) {
}

void Incremental::operator()(Event<Connected> const &) {
}

void Incremental::operator()(Event<Disconnected> const &) {
  ready_ = false;
}

void Incremental::operator()(Event<Ready> const &) {
  ready_ = true;
}

void Incremental::operator()(Event<ReferenceData> const &event) {
  assert(ready_);
  auto &[message_info, reference_data] = event;
  if (reference_data.discard)
    return;
  auto message = codec::Encoder::encode(encode_buffer_, reference_data);
  send(message);
}

void Incremental::operator()(Event<MarketStatus> const &event) {
  assert(ready_);
  auto &[message_info, market_status] = event;
  auto message = codec::Encoder::encode(encode_buffer_, market_status);
  send(message);
}

void Incremental::operator()(Event<TopOfBook> const &event) {
  assert(ready_);
  auto &[message_info, top_of_book] = event;
  auto message = codec::Encoder::encode(encode_buffer_, top_of_book);
  send(message);
}

void Incremental::operator()(Event<MarketByPriceUpdate> const &event) {
  assert(ready_);
  auto &[message_info, market_by_price_update] = event;
  // XXX FIXME HACK !!!
  auto tmp = market_by_price_update;
  tmp.bids = {std::data(market_by_price_update.bids), std::min<size_t>(1024, std::size(market_by_price_update.bids))};
  tmp.asks = {std::data(market_by_price_update.asks), std::min<size_t>(1024, std::size(market_by_price_update.asks))};
  auto message = codec::Encoder::encode(encode_buffer_, tmp);
  send(message);
}

void Incremental::operator()(Event<MarketByOrderUpdate> const &event) {
  assert(ready_);
  /*
  auto &[message_info, market_by_order_update] = event;
  auto message = codec::Encoder::encode(encode_buffer_, market_by_order_update);
  send(message);
  */
}

void Incremental::operator()(Event<TradeSummary> const &event) {
  assert(ready_);
  auto &[message_info, trade_summary] = event;
  auto message = codec::Encoder::encode(encode_buffer_, trade_summary);
  send(message);
}

void Incremental::operator()(Event<StatisticsUpdate> const &event) {
  assert(ready_);
  auto &[message_info, statistics_update] = event;
  auto message = codec::Encoder::encode(encode_buffer_, statistics_update);
  send(message);
}

}  // namespace publisher
}  // namespace sbe
}  // namespace roq
