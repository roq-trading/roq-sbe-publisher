/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/sbe_publisher/incremental.hpp"

#include "roq/logging.hpp"

#include "roq/codec/udp/header.hpp"

using namespace std::literals;

namespace roq {
namespace sbe_publisher {

// === CONSTANTS ===

namespace {
auto const CONTROL = codec::udp::pack(codec::udp::Encoding::SBE, codec::udp::Channel::INCREMENTAL);
}  // namespace

// === IMPLEMENTATION ===

Incremental::Incremental(Settings const &settings, io::Context &context, Shared &shared)
    : Base{settings, context, shared, settings.multicast.multicast_address_incremental, settings.multicast.multicast_port_incremental},
      encoder_{codec::sbe::Encoder::create()}, max_depth_{settings.max_depth} {
}

void Incremental::operator()(Instrument const &instrument, Event<ReferenceData> const &event) {
  assert(ready());
  if (event.value.discard) {
    return;
  }
  auto message = (*encoder_)(event);
  send(message, CONTROL, 0, instrument.object_id, instrument.last_sequence_number.reference_data);
}

void Incremental::operator()(Instrument const &instrument, Event<MarketStatus> const &event) {
  assert(ready());
  auto message = (*encoder_)(event);
  send(message, CONTROL, 0, instrument.object_id, instrument.last_sequence_number.market_status);
}

void Incremental::operator()(Instrument const &instrument, Event<TopOfBook> const &event) {
  assert(ready());
  auto message = (*encoder_)(event);
  send(message, CONTROL, 0, instrument.object_id, instrument.last_sequence_number.top_of_book);
}

void Incremental::operator()(Instrument const &instrument, Event<MarketByPriceUpdate> const &event) {
  assert(ready());
  auto &[message_info, market_by_price_update] = event;
  auto &market_by_price = instrument.get_market_by_price();
  market_by_price.create_depth_update(market_by_price_update, max_depth_, bids_, asks_);
  /*
  if (std::empty(bids_) && std::empty(asks_))
    return;
  */
  auto tmp = market_by_price_update;
  tmp.bids = bids_;
  tmp.asks = asks_;
  Event event_2{message_info, tmp};
  auto message = (*encoder_)(event_2);
  send(message, CONTROL, 0, instrument.object_id, instrument.last_sequence_number.market_by_price);
}

void Incremental::operator()(Instrument const &, Event<MarketByOrderUpdate> const &) {
  assert(ready());
  /*
  auto &[message_info, market_by_order_update] = event;
  auto message = (*encoder_)( market_by_order_update);
  send(message,CONTROL,0,instrument.object_id, instrument.last_sequence_number.market_by_order);
  */
}

void Incremental::operator()(Instrument const &instrument, Event<TradeSummary> const &event) {
  assert(ready());
  auto message = (*encoder_)(event);
  send(message, CONTROL, 0, instrument.object_id, instrument.last_sequence_number.trade_summary);
}

void Incremental::operator()(Instrument const &instrument, Event<StatisticsUpdate> const &event) {
  assert(ready());
  auto message = (*encoder_)(event);
  send(message, CONTROL, 0, instrument.object_id, instrument.last_sequence_number.statistics);
}

}  // namespace sbe_publisher
}  // namespace roq
