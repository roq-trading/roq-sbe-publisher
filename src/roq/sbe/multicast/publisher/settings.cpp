/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/sbe/multicast/publisher/settings.hpp"

#include "roq/logging.hpp"

using namespace std::literals;

namespace roq {
namespace sbe {
namespace multicast {
namespace publisher {

Settings::Settings(args::Parser const &args) : client::flags::Settings{args}, flags::Flags{flags::Flags::create()} {
  log::debug("settings={}"sv, *this);
}

}  // namespace publisher
}  // namespace multicast
}  // namespace sbe
}  // namespace roq
