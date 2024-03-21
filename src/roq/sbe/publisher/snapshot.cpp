/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include "roq/sbe/publisher/snapshot.hpp"

#include "roq/logging.hpp"

#include "roq/codec/udp/header.hpp"

#include "roq/sbe/publisher/shared.hpp"

using namespace std::literals;

namespace roq {
namespace sbe {
namespace publisher {

// === CONSTANTS ===

namespace {
auto const CONTROL = codec::udp::pack(codec::udp::Encoding::SBE, codec::udp::Channel::SNAPSHOT);
}

// === IMPLEMENTATION ===

Snapshot::Snapshot(Settings const &settings, io::Context &context, Shared &shared)
    : Base{settings, context, shared, settings.multicast.multicast_address_snapshot, settings.multicast.multicast_port_snapshot},
      publish_freq_{settings.snapshot_publish_freq}, shared_{shared}, encoder_{codec::sbe::Encoder::create()},
      max_depth_{settings.max_depth} {
}

void Snapshot::operator()(Event<Timer> const &event) {
  if (!shared_.ready())  // XXX TODO maybe a latch instead so subsequent disconnects don't stop publishing?
    return;
  auto now = event.value.now;
  if (now < next_publish_)
    return;
  next_publish_ = now + publish_freq_;
  while (true) {
    if (std::empty(publish_)) {
      shared_.get_all_instruments([&](auto &instrument) { publish_.emplace_back(instrument.instrument_id); });
      if (std::empty(publish_))
        return;
    }
    assert(!std::empty(publish_));
    auto instrument_id = publish_.front();
    publish_.pop_front();
    if (shared_.find_instrument(instrument_id, [&](auto &instrument) { publish(instrument); })) {
      break;
    } else {
      assert(false);
    }
  }
}

// utilities

void Snapshot::publish(Instrument const &instrument) {
  auto message_info = MessageInfo{
      .source = SOURCE_NONE,
      .source_name = shared_.get_source_name(),
      .source_session_id = shared_.get_source_session_id(),
      .source_seqno = shared_.get_source_seqno(),
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
  auto reference_data = static_cast<ReferenceData>(instrument);
  Event event_1{message_info, reference_data};
  auto message_1 = (*encoder_)(event_1);
  send(message_1, CONTROL, 0, instrument.object_id, instrument.last_sequence_number.reference_data);
  // market status
  auto market_status = static_cast<MarketStatus>(instrument);
  Event event_2{message_info, market_status};
  auto message_2 = (*encoder_)(event_2);
  send(message_2, CONTROL, 0, instrument.object_id, instrument.last_sequence_number.market_status);
  // market by price
  instrument.create_market_by_price_snapshot(bids_, asks_, [&](auto &market_by_price_update) {
    auto tmp = market_by_price_update;
    tmp.bids = {std::data(market_by_price_update.bids), std::min(max_depth_, std::size(market_by_price_update.bids))};
    tmp.asks = {std::data(market_by_price_update.asks), std::min(max_depth_, std::size(market_by_price_update.asks))};
    Event event_3{message_info, tmp};
    auto message_3 = (*encoder_)(event_3);
    send(message_3, CONTROL, 0, instrument.object_id, instrument.last_sequence_number.market_by_price);
  });
  // statistics
  auto statistics_update = static_cast<StatisticsUpdate>(instrument);
  Event event_4{message_info, statistics_update};
  auto message_4 = (*encoder_)(event_4);
  send(message_4, CONTROL, 0, instrument.object_id, instrument.last_sequence_number.statistics);
}

}  // namespace publisher
}  // namespace sbe
}  // namespace roq
