/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/sbe/multicast/subscriber/settings.hpp"

using namespace std::chrono_literals;

namespace roq {
namespace sbe {
namespace multicast {
namespace subscriber {

// === IMPLEMENTATION ===

Settings::Settings(args::Parser const &) : flags::Flags{flags::Flags::create()} {
}

}  // namespace subscriber
}  // namespace multicast
}  // namespace sbe
}  // namespace roq
