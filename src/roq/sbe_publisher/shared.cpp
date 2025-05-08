/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/sbe_publisher/shared.hpp"

#include "roq/logging.hpp"

#include "roq/clock.hpp"

#include "roq/utils/debug/hex/message.hpp"

#include "roq/core/routing/utility.hpp"

using namespace std::literals;

namespace roq {
namespace sbe_publisher {

// === HELPERS ===

namespace {
auto create_session_id() {
  auto now_utc = clock::get_realtime<std::chrono::seconds>();
  return static_cast<uint16_t>(now_utc.count());
}

auto get_instrument_id_from_opaque(auto opaque) {
  auto routing = core::routing::routing_from_opaque(opaque);
  return routing.id;
}
}  // namespace

// === IMPLEMENTATION ===

Shared::Shared() : session_id{create_session_id()} {
  log::info("session_id={}"sv, utils::debug::hex::Message{std::span{reinterpret_cast<std::byte const *>(&session_id), sizeof(session_id)}});
}

void Shared::operator()(Event<Disconnected> const &) {
  ready_ = false;
  source_name_.clear();
  source_session_id_ = {};
  source_seqno_ = {};
}

void Shared::operator()(Event<Connected> const &event) {
  source_name_ = event.message_info.source_name;
  source_session_id_ = event.message_info.source_session_id;
}

void Shared::operator()(Event<Ready> const &) {
  ready_ = true;
}

void Shared::operator()(MessageInfo const &message_info) {
  assert(message_info.source_seqno);
  source_seqno_ = message_info.source_seqno;
}

Instrument &Shared::get_instrument(uint64_t opaque, std::string_view const &exchange, std::string_view const &symbol) {
  auto instrument_id = get_instrument_id_from_opaque(opaque);
  auto iter = instruments_.find(instrument_id);
  if (iter == std::end(instruments_)) {
    if (next_object_id_ == std::numeric_limits<decltype(next_object_id_)>::max()) {
      log::fatal("Unexpected: too many objects"sv);
    }
    iter = instruments_.try_emplace(instrument_id, instrument_id, ++next_object_id_, exchange, symbol).first;
  }
  return (*iter).second;
}

}  // namespace sbe_publisher
}  // namespace roq
