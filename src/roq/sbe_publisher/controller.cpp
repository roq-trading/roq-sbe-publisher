/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/sbe_publisher/controller.hpp"

#include "roq/logging.hpp"

using namespace std::literals;

namespace roq {
namespace sbe_publisher {

// === IMPLEMENTATION ===

Controller::Controller(client::Dispatcher &, Settings const &settings, Config const &, io::Context &context)
    : context_{context}, incremental_{settings, context, shared_}, snapshot_{settings, context, shared_} {
}

// client::Handler

void Controller::operator()(Event<Timer> const &event) {
  context_.drain();
  snapshot_(event);
}

void Controller::operator()(Event<Connected> const &event) {
  shared_(event);
}

void Controller::operator()(Event<Disconnected> const &event) {
  shared_(event);
}

void Controller::operator()(Event<DownloadBegin> const &) {
}

void Controller::operator()(Event<DownloadEnd> const &) {
}

void Controller::operator()(Event<Ready> const &event) {
  shared_(event);
}

void Controller::operator()(Event<ReferenceData> const &event) {
  dispatch(event);
}

void Controller::operator()(Event<MarketStatus> const &event) {
  dispatch(event);
}

void Controller::operator()(Event<TopOfBook> const &event) {
  dispatch(event);
}

void Controller::operator()(Event<MarketByPriceUpdate> const &event) {
  dispatch(event);
}

void Controller::operator()(Event<MarketByOrderUpdate> const &event) {
  dispatch(event);
}

void Controller::operator()(Event<TradeSummary> const &event) {
  dispatch(event);
}

void Controller::operator()(Event<StatisticsUpdate> const &event) {
  dispatch(event);
}

// utilities

template <typename T>
void Controller::dispatch(Event<T> const &event) {
  shared_(event.message_info);
  shared_.find_instrument_or_create(event.message_info.opaque, event.value.exchange, event.value.symbol, [&](auto &instrument) {
    if (shared_.ready()) {
      incremental_(instrument, event);
    }
    instrument(event, incremental_.get_sequence_number());  // note! *after* incremental
  });
}

}  // namespace sbe_publisher
}  // namespace roq
