/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include "roq/sbe/publisher/settings.hpp"

#include "roq/logging.hpp"

using namespace std::literals;

namespace roq {
namespace sbe {
namespace publisher {

Settings::Settings(args::Parser const &args)
    : client::flags::Settings{args}, flags::Flags{flags::Flags::create()}, multicast{flags::Multicast::create()} {
  log::info("settings={}"sv, *this);
}

}  // namespace publisher
}  // namespace sbe
}  // namespace roq
