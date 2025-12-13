/* Copyright (c) 2017-2026, Hans Erik Thrane */

#include "roq/sbe_publisher/application.hpp"

#include "roq/logging.hpp"

#include "roq/client/poller.hpp"

#include "roq/io/engine/context_factory.hpp"

#include "roq/sbe_publisher/config.hpp"
#include "roq/sbe_publisher/controller.hpp"
#include "roq/sbe_publisher/settings.hpp"

using namespace std::literals;

namespace roq {
namespace sbe_publisher {

// === IMPLEMENTATION ===

int Application::main(args::Parser const &args) {
  auto params = args.params();
  if (std::empty(params)) {
    log::fatal("Unexpected"sv);
  }
  Settings settings{args};
  auto config = Config::parse_file(settings.config_file);
  auto context = io::engine::ContextFactory::create();
  Controller{settings, config, *context, params}.dispatch();
  return EXIT_SUCCESS;
}

}  // namespace sbe_publisher
}  // namespace roq
