/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/sbe/publisher/incremental.hpp"

#include "roq/logging.hpp"

#include "roq/codec/udp/header.hpp"

using namespace std::literals;

namespace roq {
namespace sbe {
namespace publisher {

// === CONSTANTS ===

namespace {
auto const CONTROL = codec::udp::pack(codec::udp::Encoding::SBE, codec::udp::Channel::INCREMENTAL);
auto const MAX_DEPTH = 1024 * 1024;
}  // namespace

// === IMPLEMENTATION ===

Incremental::Incremental(Settings const &settings, io::Context &context, Shared &shared)
    : Base{settings, context, shared, settings.multicast_address_incremental, settings.multicast_port_incremental},
      shared_{shared}, encoder_{codec::sbe::Encoder::create()}, max_depth_{settings.max_depth}, bids_(MAX_DEPTH),
      asks_(MAX_DEPTH) {
}

void Incremental::operator()(Instrument const &instrument, Event<ReferenceData> const &event) {
  assert(shared_.ready());
  if (event.value.discard)
    return;
  auto message = (*encoder_)(event);
  send(message, CONTROL, 0, instrument.object_id, instrument.last_sequence_number.reference_data);
}

void Incremental::operator()(Instrument const &instrument, Event<MarketStatus> const &event) {
  assert(shared_.ready());
  auto message = (*encoder_)(event);
  send(message, CONTROL, 0, instrument.object_id, instrument.last_sequence_number.market_status);
}

void Incremental::operator()(Instrument const &instrument, Event<TopOfBook> const &event) {
  assert(shared_.ready());
  auto message = (*encoder_)(event);
  send(message, CONTROL, 0, instrument.object_id, instrument.last_sequence_number.top_of_book);
}

void Incremental::operator()(Instrument const &instrument, Event<MarketByPriceUpdate> const &event) {
  assert(shared_.ready());
  auto &[message_info, market_by_price_update] = event;
  auto &market_by_price = instrument.get_market_by_price();
  auto result = market_by_price.create_depth_update(market_by_price_update, max_depth_, bids_, asks_);
  /*
  if (std::empty(result.first) && std::empty(result.second))
    return;
  */
  auto tmp = market_by_price_update;
  tmp.bids = result.first;
  tmp.asks = result.second;
  Event event_2{message_info, tmp};
  auto message = (*encoder_)(event_2);
  send(message, CONTROL, 0, instrument.object_id, instrument.last_sequence_number.market_by_price);
}

void Incremental::operator()(Instrument const &, Event<MarketByOrderUpdate> const &) {
  assert(shared_.ready());
  /*
  auto &[message_info, market_by_order_update] = event;
  auto message = (*encoder_)( market_by_order_update);
  send(message,CONTROL,0,instrument.object_id, instrument.last_sequence_number.market_by_order);
  */
}

void Incremental::operator()(Instrument const &instrument, Event<TradeSummary> const &event) {
  assert(shared_.ready());
  auto message = (*encoder_)(event);
  send(message, CONTROL, 0, instrument.object_id, instrument.last_sequence_number.trade_summary);
}

void Incremental::operator()(Instrument const &instrument, Event<StatisticsUpdate> const &event) {
  assert(shared_.ready());
  auto message = (*encoder_)(event);
  send(message, CONTROL, 0, instrument.object_id, instrument.last_sequence_number.statistics);
}

}  // namespace publisher
}  // namespace sbe
}  // namespace roq
