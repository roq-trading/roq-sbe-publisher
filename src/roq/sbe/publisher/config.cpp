/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/sbe/publisher/config.hpp"

#include "roq/logging.hpp"

using namespace std::literals;

namespace roq {
namespace sbe {
namespace publisher {

// === IMPLEMENTATION ===

Config::Config(Settings const &settings) : settings_{settings} {
}

void Config::dispatch(Handler &handler) const {
  auto symbol = client::Symbol{
      .regex = settings_.symbols,
      .exchange = settings_.exchange,
  };
  handler(symbol);
}

}  // namespace publisher
}  // namespace sbe
}  // namespace roq
