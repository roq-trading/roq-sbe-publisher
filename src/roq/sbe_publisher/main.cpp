/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include "roq/api.hpp"

#include "roq/flags/args.hpp"
#include "roq/logging/flags/settings.hpp"

#include "roq/sbe_publisher/application.hpp"

using namespace std::literals;

// === CONSTANTS ===

namespace {
auto const INFO = roq::Service::Info{
    .description = "SBE Publisher"sv,
    .package_name = ROQ_PACKAGE_NAME,
    .build_version = ROQ_VERSION,
};
}  // namespace

// === IMPLEMENTATION ===

int main(int argc, char **argv) {
  roq::flags::Args args{argc, argv, INFO.description, INFO.build_version};
  roq::logging::flags::Settings settings{args};
  return roq::sbe_publisher::Application{args, settings, INFO}.run();
}
