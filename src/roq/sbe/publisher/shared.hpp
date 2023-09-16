/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <absl/container/flat_hash_map.h>

#include "roq/api.hpp"

#include "roq/sbe/publisher/instrument.hpp"

namespace roq {
namespace sbe {
namespace publisher {

struct Shared final {
  Shared();

  Shared(Shared &&) = default;
  Shared(Shared const &) = delete;

  uint16_t const session_id;

  Source const &get_source_name() const { return source_name_; }
  UUID const &get_source_session_id() const { return source_session_id_; }
  uint64_t get_source_seqno() const { return source_seqno_; }

  bool ready() const { return ready_; }

  void operator()(Event<Disconnected> const &);
  void operator()(Event<Connected> const &);
  void operator()(Event<Ready> const &);

  void operator()(MessageInfo const &);

  template <typename Callback>
  void find_instrument_or_create(
      uint64_t opaque, std::string_view const &exchange, std::string_view const &symbol, Callback callback) {
    auto &instrument = get_instrument(opaque, exchange, symbol);
    callback(instrument);
  }

  template <typename Callback>
  bool find_instrument(uint32_t instrument_id, Callback callback) {
    auto iter = instruments_.find(instrument_id);
    if (iter != std::end(instruments_)) {
      callback((*iter).second);
      return true;
    }
    return false;
  }

  template <typename Callback>
  bool get_all_instruments(Callback callback) {
    if (std::empty(instruments_))
      return false;
    for (auto &[_, instrument] : instruments_)
      callback(instrument);
    return true;
  }

  Instrument &get_instrument(uint64_t opaque, std::string_view const &exchange, std::string_view const &symbol);

 private:
  Source source_name_;
  UUID source_session_id_ = {};
  uint64_t source_seqno_ = {};

  bool ready_ = {};

  uint16_t next_object_id_ = {};
  absl::flat_hash_map<uint32_t, Instrument> instruments_;
};

}  // namespace publisher
}  // namespace sbe
}  // namespace roq
