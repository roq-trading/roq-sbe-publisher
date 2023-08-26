/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/sbe/multicast/subscriber/application.hpp"

#include <vector>

#include "roq/exceptions.hpp"

#include "roq/logging.hpp"

#include "roq/io/engine/context_factory.hpp"

#include "roq/sbe/multicast/subscriber/config.hpp"
#include "roq/sbe/multicast/subscriber/controller.hpp"
#include "roq/sbe/multicast/subscriber/settings.hpp"

using namespace std::literals;

namespace roq {
namespace sbe {
namespace multicast {
namespace subscriber {

// === IMPLEMENTATION ===

int Application::main(args::Parser const &args) {
  auto params = args.params();
  Settings settings{args};
  Config config;
  auto context = io::engine::ContextFactory::create_libevent();
  try {
    Controller{settings, config, *context}.run();
    return EXIT_SUCCESS;
  } catch (...) {
    try {
      throw;
    } catch (Exception &e) {
      log::error("Unhandled exception: {}"sv, e);
    } catch (std::exception &e) {
      log::error(R"(Unhandled exception: type="{}", what="{}")"sv, typeid(e).name(), e.what());
    } catch (...) {
      auto e = std::current_exception();
      log::error(R"(Unhandled exception: type="{}")"sv, typeid(e).name());
    }
  }
  return EXIT_FAILURE;
}

}  // namespace subscriber
}  // namespace multicast
}  // namespace sbe
}  // namespace roq
