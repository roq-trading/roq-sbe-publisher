/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/sbe/publisher/incremental.hpp"

#include "roq/logging.hpp"

#include "roq/codec/sbe/encoder.hpp"
#include "roq/codec/sbe/header.hpp"

using namespace std::literals;

namespace roq {
namespace sbe {
namespace publisher {

// === CONSTANTS ===

namespace {
auto const CONTROL = codec::sbe::ENCODING | codec::sbe::INCREMENTAL;
}

// === IMPLEMENTATION ===

Incremental::Incremental(Settings const &settings, io::Context &context, Shared &shared)
    : Base{settings, context, shared, settings.multicast_address_incremental, settings.multicast_port_incremental},
      shared_{shared} {
}

void Incremental::operator()(Instrument const &instrument, Event<ReferenceData> const &event) {
  assert(shared_.ready());
  if (event.value.discard)
    return;
  auto message = codec::sbe::Encoder::encode(encode_buffer_, event);
  send(message, CONTROL, 0, instrument.object_id, instrument.last_sequence_number.reference_data);
}

void Incremental::operator()(Instrument const &instrument, Event<MarketStatus> const &event) {
  assert(shared_.ready());
  auto message = codec::sbe::Encoder::encode(encode_buffer_, event);
  send(message, CONTROL, 0, instrument.object_id, instrument.last_sequence_number.market_status);
}

void Incremental::operator()(Instrument const &instrument, Event<TopOfBook> const &event) {
  assert(shared_.ready());
  auto message = codec::sbe::Encoder::encode(encode_buffer_, event);
  send(message, CONTROL, 0, instrument.object_id, instrument.last_sequence_number.top_of_book);
}

void Incremental::operator()(Instrument const &instrument, Event<MarketByPriceUpdate> const &event) {
  assert(shared_.ready());
  auto &[message_info, market_by_price_update] = event;
  // XXX FIXME HACK !!!
  auto tmp = market_by_price_update;
  tmp.bids = {std::data(market_by_price_update.bids), std::min<size_t>(1024, std::size(market_by_price_update.bids))};
  tmp.asks = {std::data(market_by_price_update.asks), std::min<size_t>(1024, std::size(market_by_price_update.asks))};
  Event event_2{message_info, tmp};
  auto message = codec::sbe::Encoder::encode(encode_buffer_, event_2);
  send(message, CONTROL, 0, instrument.object_id, instrument.last_sequence_number.market_by_price);
}

void Incremental::operator()(Instrument const &instrument, Event<MarketByOrderUpdate> const &event) {
  assert(shared_.ready());
  /*
  auto &[message_info, market_by_order_update] = event;
  auto message = codec::sbe::Encoder::encode(encode_buffer_, market_by_order_update);
  send(message,CONTROL,0,instrument.object_id, instrument.last_sequence_number.market_by_order);
  */
}

void Incremental::operator()(Instrument const &instrument, Event<TradeSummary> const &event) {
  assert(shared_.ready());
  auto message = codec::sbe::Encoder::encode(encode_buffer_, event);
  send(message, CONTROL, 0, instrument.object_id, instrument.last_sequence_number.trade_summary);
}

void Incremental::operator()(Instrument const &instrument, Event<StatisticsUpdate> const &event) {
  assert(shared_.ready());
  auto message = codec::sbe::Encoder::encode(encode_buffer_, event);
  send(message, CONTROL, 0, instrument.object_id, instrument.last_sequence_number.statistics);
}

}  // namespace publisher
}  // namespace sbe
}  // namespace roq
