/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/sbe/publisher/controller.hpp"

#include "roq/logging.hpp"

using namespace std::literals;

namespace roq {
namespace sbe {
namespace publisher {

// === IMPLEMENTATION ===

Controller::Controller(client::Dispatcher &dispatcher, Settings const &settings, Config const &, io::Context &context)
    : dispatcher_{dispatcher}, context_{context}, incremental_{settings, context, shared_},
      snapshot_{settings, context, shared_} {
}

// client::Handler

void Controller::operator()(Event<Timer> const &event) {
  context_.drain();
  dispatch(event, true);
}

void Controller::operator()(Event<Connected> const &event) {
  shared_.source_name = event.message_info.source_name;
  shared_.source_session_id = event.message_info.source_session_id;
  dispatch(event, true);
}

void Controller::operator()(Event<Disconnected> const &event) {
  ready_ = false;
  shared_.source_name.clear();
  shared_.source_session_id = {};
  dispatch(event, true);
}

void Controller::operator()(Event<DownloadBegin> const &) {
}

void Controller::operator()(Event<DownloadEnd> const &) {
}

void Controller::operator()(Event<Ready> const &event) {
  ready_ = true;
  dispatch(event, true);
}

void Controller::operator()(Event<ReferenceData> const &event) {
  dispatch(event, ready_);
}

void Controller::operator()(Event<MarketStatus> const &event) {
  dispatch(event, ready_);
}

void Controller::operator()(Event<TopOfBook> const &event) {
  dispatch(event, ready_);
}

void Controller::operator()(Event<MarketByPriceUpdate> const &event) {
  dispatch(event, ready_);
}

void Controller::operator()(Event<MarketByOrderUpdate> const &event) {
  dispatch(event, ready_);
}

void Controller::operator()(Event<TradeSummary> const &event) {
  dispatch(event, ready_);
}

void Controller::operator()(Event<StatisticsUpdate> const &event) {
  dispatch(event, ready_);
}

// utilities

template <typename T>
void Controller::dispatch(Event<T> const &event, bool ready) {
  if (event.message_info.source_seqno)  // note! timer shouldn't update
    shared_.source_seqno = event.message_info.source_seqno;
  if (ready)
    incremental_(event);
  snapshot_(event);
}

}  // namespace publisher
}  // namespace sbe
}  // namespace roq
