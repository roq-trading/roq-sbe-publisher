/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/sbe/publisher/shared.hpp"

#include "roq/logging.hpp"

#include "roq/clock.hpp"

#include "roq/debug/hex/message.hpp"

using namespace std::literals;

namespace roq {
namespace sbe {
namespace publisher {

// === HELPERS ===

namespace {
auto create_session_id() {
  auto now_utc = clock::get_realtime<std::chrono::seconds>();
  return static_cast<uint16_t>(now_utc.count());
}
}  // namespace

// === IMPLEMENTATION ===

Shared::Shared() : session_id{create_session_id()} {
  log::info(
      "session_id={}"sv,
      debug::hex::Message{std::span{reinterpret_cast<std::byte const *>(&session_id), sizeof(session_id)}});
}

}  // namespace publisher
}  // namespace sbe
}  // namespace roq
