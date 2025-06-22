/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/sbe_publisher/controller.hpp"

#include "roq/logging.hpp"

using namespace std::literals;

namespace roq {
namespace sbe_publisher {

// === CONSTANTS ===

namespace {
auto const DISPATCH_THIS_MANY_BEFORE_CHECKING_CLOCK = 1000uz;
auto const POLL_TIME = 100ns;
auto const SLEEP_TIME = 1ms;
auto const REFRESH_FREQ = 40ms;  // ~24 fps
}  // namespace

// === HELPERS ===

namespace {
auto create_dispatcher(auto &settings, auto &config, auto &context, auto &params) {
  return client::Poller::create(settings, config, context, params);
}
}  // namespace

// === IMPLEMENTATION ===

Controller::Controller(Settings const &settings, Config const &config, io::Context &context, std::span<std::string_view const> const &params)
    : context_{context}, terminate_{context.create_signal(*this, io::sys::Signal::Type::TERMINATE)},
      interrupt_{context.create_signal(*this, io::sys::Signal::Type::INTERRUPT)}, dispatcher_{create_dispatcher(settings, config, context, params)},
      incremental_{settings, context, shared_}, snapshot_{settings, context, shared_} {
}

void Controller::dispatch() {
  log::info("Starting the dispatch loop..."sv);
  (*dispatcher_).start();
  auto success = true;
  while (success) {
    auto now = clock::get_system();
    refresh(now);
    if (next_yield_ < now && POLL_TIME.count() > 0) {
      next_yield_ = now + POLL_TIME;
      std::this_thread::sleep_for(SLEEP_TIME);
    }
    for (size_t i = 0; success && i < DISPATCH_THIS_MANY_BEFORE_CHECKING_CLOCK; ++i) {
      success = (*dispatcher_).dispatch(*this);
    }
  }
  log::info("The dispatch loop has stopped!"sv);
}

void Controller::refresh(std::chrono::nanoseconds now) {
  MessageInfo message_info;
  auto timer = Timer{
      .now = now,
  };
  create_event_and_dispatch(snapshot_, message_info, timer);
}

// io::sys::Signal::Handler

void Controller::operator()(io::sys::Signal::Event const &event) {
  log::warn("*** SIGNAL: {} ***"sv, event.type);
  (*dispatcher_).stop();
}

// client::Poller::Handler

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
