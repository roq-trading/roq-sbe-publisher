/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/sbe/publisher/application.hpp"

#include <cassert>

#include "roq/client.hpp"

#include "roq/io/engine/context_factory.hpp"

#include "roq/sbe/publisher/config.hpp"
#include "roq/sbe/publisher/settings.hpp"

using namespace std::literals;

namespace roq {
namespace sbe {
namespace publisher {

// === IMPLEMENTATION ===

int Application::main(args::Parser const &args) {
  auto params = args.params();
  if (std::empty(params))
    log::fatal("Unexpected"sv);
  Settings settings{args};
  auto config = Config::parse_file(settings.config_file);
  auto context = io::engine::ContextFactory::create_libevent();
  client::Bridge{settings, config, params}.dispatch<value_type>(settings, config, *context);
  return EXIT_SUCCESS;
}

}  // namespace publisher
}  // namespace sbe
}  // namespace roq
