/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/sbe_publisher/application.hpp"

#include "roq/logging.hpp"

#include "roq/client.hpp"

#include "roq/io/engine/context_factory.hpp"

#include "roq/sbe_publisher/config.hpp"
#include "roq/sbe_publisher/settings.hpp"

using namespace std::literals;

namespace roq {
namespace sbe_publisher {

// === IMPLEMENTATION ===

int Application::main(args::Parser const &args) {
  auto params = args.params();
  if (std::empty(params))
    log::fatal("Unexpected"sv);
  Settings settings{args};
  auto config = Config::parse_file(settings.config_file);
  auto context = io::engine::ContextFactory::create();
  client::Bridge{settings, config, params}.dispatch<value_type>(settings, config, *context);
  return EXIT_SUCCESS;
}

}  // namespace sbe_publisher
}  // namespace roq
