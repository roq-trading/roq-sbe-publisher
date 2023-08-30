/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/sbe/publisher/snapshot.hpp"

#include "roq/logging.hpp"

#include "roq/sbe/codec/encoder.hpp"

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
      publish_freq_{settings.snapshot_publish_freq} {
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
      log::debug(
          R"(publish instrument_id={}, exchange="{}", symbol="{}")"sv,
          instrument_id,
          instrument.exchange,
          instrument.symbol);
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
}

void Snapshot::operator()(Event<Ready> const &) {
  ready_ = true;
}

void Snapshot::operator()(Event<ReferenceData> const &event) {
  auto &[message_info, reference_data] = event;
  if (reference_data.discard)
    return;
  dispatch(event);
}

void Snapshot::operator()(Event<MarketStatus> const &event) {
  dispatch(event);
}

void Snapshot::operator()(Event<TopOfBook> const &) {
}

void Snapshot::operator()(Event<MarketByPriceUpdate> const &) {
}

void Snapshot::operator()(Event<MarketByOrderUpdate> const &) {
}

void Snapshot::operator()(Event<TradeSummary> const &) {
}

void Snapshot::operator()(Event<StatisticsUpdate> const &) {
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
  auto reference_data = instrument.reference_data.convert(instrument);
  auto message_1 = codec::Encoder::encode(encode_buffer_, reference_data);
  send(message_1);
  auto market_status = instrument.market_status.convert(instrument);
  auto message_2 = codec::Encoder::encode(encode_buffer_, market_status);
  send(message_2);
}

}  // namespace publisher
}  // namespace sbe
}  // namespace roq
