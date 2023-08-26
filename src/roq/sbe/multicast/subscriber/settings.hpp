/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/args/parser.hpp"

#include "roq/sbe/multicast/subscriber/flags/flags.hpp"

namespace roq {
namespace sbe {
namespace multicast {
namespace subscriber {

struct Settings final : public flags::Flags {
  explicit Settings(args::Parser const &);
};

}  // namespace subscriber
}  // namespace multicast
}  // namespace sbe
}  // namespace roq
