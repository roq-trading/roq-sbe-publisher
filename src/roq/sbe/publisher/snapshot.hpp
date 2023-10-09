/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <absl/container/flat_hash_map.h>

#include <chrono>
#include <deque>
#include <vector>

#include "roq/api.hpp"

#include "roq/io/context.hpp"

#include "roq/codec/sbe/encoder.hpp"

#include "roq/sbe/publisher/base.hpp"
#include "roq/sbe/publisher/instrument.hpp"
#include "roq/sbe/publisher/settings.hpp"
#include "roq/sbe/publisher/shared.hpp"

namespace roq {
namespace sbe {
namespace publisher {

struct Snapshot final : public Base {
  Snapshot(Settings const &, io::Context &context, Shared &);

  void operator()(Event<Timer> const &);

 protected:
  void publish(Instrument const &);

 private:
  std::chrono::nanoseconds const publish_freq_;
  Shared &shared_;
  std::unique_ptr<codec::sbe::Encoder> encoder_;
  std::deque<uint16_t> publish_;
  std::chrono::nanoseconds next_publish_ = {};
  std::vector<MBPUpdate> bids_, asks_;
};

}  // namespace publisher
}  // namespace sbe
}  // namespace roq
