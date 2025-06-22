/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include <chrono>
#include <deque>
#include <vector>

#include "roq/api.hpp"

#include "roq/io/context.hpp"

#include "roq/codec/sbe/encoder.hpp"

#include "roq/sbe_publisher/base.hpp"
#include "roq/sbe_publisher/instrument.hpp"
#include "roq/sbe_publisher/settings.hpp"
#include "roq/sbe_publisher/shared.hpp"

namespace roq {
namespace sbe_publisher {

struct Snapshot final : public Base {
  Snapshot(Settings const &, io::Context &context, Shared &);

  void refresh(std::chrono::nanoseconds now);

 protected:
  void publish(Instrument const &);

 private:
  std::chrono::nanoseconds const publish_freq_;
  Shared &shared_;
  std::unique_ptr<codec::sbe::Encoder> encoder_;
  std::deque<uint16_t> publish_;
  std::chrono::nanoseconds next_publish_ = {};
  size_t const max_depth_;
  std::vector<MBPUpdate> bids_, asks_;
};

}  // namespace sbe_publisher
}  // namespace roq
