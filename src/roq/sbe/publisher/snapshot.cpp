/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/sbe/publisher/snapshot.hpp"

#include "roq/logging.hpp"

#include "roq/sbe/codec/encoder.hpp"

using namespace std::literals;

namespace roq {
namespace sbe {
namespace publisher {

// === IMPLEMENTATION ===

Snapshot::Snapshot(Settings const &settings, io::Context &context)
    : Base{settings, context, settings.multicast_address_snapshot, settings.multicast_port_snapshot} {
}

void Snapshot::operator()(Event<Timer> const &) {
  // XXX TODO
  // - maintain a list of opaque to publish (remember lookup due to disconnect)
  // - cache sbe encoded blob
}

void Snapshot::operator()(Event<Connected> const &) {
}

void Snapshot::operator()(Event<Disconnected> const &) {
  ready_ = false;
  reference_data_.clear();
}

void Snapshot::operator()(Event<Ready> const &) {
  ready_ = true;
}

void Snapshot::operator()(Event<ReferenceData> const &event) {
  auto &[message_info, reference_data] = event;
  if (reference_data.discard)
    return;
  if (reference_data_[message_info.opaque](reference_data)) {
    // updated
  }
}

void Snapshot::operator()(Event<MarketStatus> const &event) {
  auto &[message_info, market_status] = event;
  if (market_status_[message_info.opaque](market_status)) {
    // updated
  }
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

// publish

void Snapshot::operator()(cache::ReferenceData const &cache) {
  struct Context final {  // XXX FIXME
    std::string exchange;
    std::string symbol;
  } context;
  auto reference_data = cache.convert(context);
  auto message = codec::Encoder::encode(encode_buffer_, reference_data);
  send(message);
}

void Snapshot::operator()(cache::MarketStatus const &cache) {
  struct Context final {  // XXX FIXME
    std::string exchange;
    std::string symbol;
  } context;
  auto market_status = cache.convert(context);
  auto message = codec::Encoder::encode(encode_buffer_, market_status);
  send(message);
}

}  // namespace publisher
}  // namespace sbe
}  // namespace roq
