/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/sbe/publisher/snapshot.hpp"

#include "roq/logging.hpp"

#include "roq/codec/sbe/encoder.hpp"

#include "roq/core/routing/utility.hpp"

using namespace std::literals;

namespace roq {
namespace sbe {
namespace publisher {

// === HELPERS ===

namespace {
auto get_instrument_id_from_opaque(auto opaque) {
  auto routing = core::routing::routing_from_opaque(opaque);
  return routing.id;
}
}  // namespace

// === IMPLEMENTATION ===

Snapshot::Snapshot(Settings const &settings, io::Context &context, Shared &shared)
    : Base{settings, context, shared, settings.multicast_address_snapshot, settings.multicast_port_snapshot},
      publish_freq_{settings.snapshot_publish_freq}, shared_{shared} {
}

void Snapshot::operator()(Event<Timer> const &event) {
  if (!ready_)
    return;
  if (std::empty(instruments_))
    return;
  auto now = event.value.now;
  if (now < next_publish_)
    return;
  next_publish_ = now + publish_freq_;
  while (true) {
    if (std::empty(publish_)) {
      for (auto &[instrument_id, _] : instruments_)
        publish_.emplace_back(instrument_id);
    }
    assert(!std::empty(publish_));
    auto instrument_id = publish_.front();
    publish_.pop_front();
    auto iter = instruments_.find(instrument_id);
    if (iter != std::end(instruments_)) {
      auto &instrument = (*iter).second;
      /*
      log::debug(
          R"(publish instrument_id={}, exchange="{}", symbol="{}")"sv,
          instrument_id,
          instrument.exchange,
          instrument.symbol);
      */
      publish(instrument);  // XXX TODO cache encoded blob
      break;
    }
  }
}

void Snapshot::operator()(Event<Connected> const &) {
}

void Snapshot::operator()(Event<Disconnected> const &) {
  ready_ = false;
  instruments_.clear();
  publish_.clear();
}

void Snapshot::operator()(Event<Ready> const &) {
  ready_ = true;
}

void Snapshot::operator()(Event<ReferenceData> const &event) {
  if (event.value.discard)
    return;
  dispatch(event);
}

void Snapshot::operator()(Event<MarketStatus> const &event) {
  dispatch(event);
}

void Snapshot::operator()(Event<TopOfBook> const &) {
  // note! we don't cache
}

void Snapshot::operator()(Event<MarketByPriceUpdate> const &event) {
  dispatch(event);
}

void Snapshot::operator()(Event<MarketByOrderUpdate> const &) {
  // note! we don't cache
}

void Snapshot::operator()(Event<TradeSummary> const &) {
  // note! we don't cache
}

void Snapshot::operator()(Event<StatisticsUpdate> const &event) {
  dispatch(event);
}

// utilities

template <typename T>
void Snapshot::dispatch(Event<T> const &event) {
  auto instrument_id = get_instrument_id_from_opaque(event.message_info.opaque);
  auto iter = instruments_.find(instrument_id);
  if (iter == std::end(instruments_))
    iter = instruments_.try_emplace(instrument_id, event.value.exchange, event.value.symbol).first;
  (*iter).second(event);
}

void Snapshot::publish(Instrument const &instrument) {
  auto message_info = MessageInfo{
      .source = SOURCE_NONE,
      .source_name = shared_.source_name,
      .source_session_id = shared_.source_session_id,
      .source_seqno = shared_.source_seqno,
      .receive_time_utc = {},
      .receive_time = {},
      .source_send_time = {},
      .source_receive_time = {},
      .origin_create_time = {},
      .origin_create_time_utc = {},  // XXX TODO ???
      .is_last = true,
      .opaque = {},
  };
  // reference data
  auto reference_data = instrument.reference_data.convert(instrument);
  // log::debug("reference_data={}"sv, reference_data);
  Event event_1{message_info, reference_data};
  auto message_1 = codec::sbe::Encoder::encode(encode_buffer_, event_1);
  send(message_1);
  // market status
  auto market_status = instrument.market_status.convert(instrument);
  // log::debug("market_status={}"sv, market_status);
  Event event_2{message_info, market_status};
  auto message_2 = codec::sbe::Encoder::encode(encode_buffer_, event_2);
  send(message_2);
  // market by price
  (*instrument.market_by_price).create_snapshot(bids_, asks_, [&](auto &market_by_price_update) {
    // log::debug("market_by_price_update={}"sv, market_by_price_update);
    // XXX FIXME HACK !!!
    auto tmp = market_by_price_update;
    tmp.bids = {std::data(market_by_price_update.bids), std::min<size_t>(1024, std::size(market_by_price_update.bids))};
    tmp.asks = {std::data(market_by_price_update.asks), std::min<size_t>(1024, std::size(market_by_price_update.asks))};
    Event event_3{message_info, tmp};
    auto message_3 = codec::sbe::Encoder::encode(encode_buffer_, event_3);
    send(message_3);
  });
  // statistics
  auto statistics_update = instrument.statistics.convert(instrument);
  // log::debug("statistics_update={}"sv, statistics_update);
  Event event_4{message_info, statistics_update};
  auto message_4 = codec::sbe::Encoder::encode(encode_buffer_, event_4);
  send(message_4);
}

}  // namespace publisher
}  // namespace sbe
}  // namespace roq
