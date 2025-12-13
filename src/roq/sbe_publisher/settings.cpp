/* Copyright (c) 2017-2026, Hans Erik Thrane */

#include "roq/sbe_publisher/settings.hpp"

#include "roq/logging.hpp"

using namespace std::literals;

namespace roq {
namespace sbe_publisher {

Settings::Settings(args::Parser const &args) : client::flags::Settings{args}, flags::Flags{flags::Flags::create()}, multicast{flags::Multicast::create()} {
  log::info("settings={}"sv, *this);
}

}  // namespace sbe_publisher
}  // namespace roq
