/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include "roq/args/parser.hpp"

#include "roq/client/flags/settings.hpp"

#include "roq/sbe_publisher/flags/flags.hpp"
#include "roq/sbe_publisher/flags/multicast.hpp"

namespace roq {
namespace sbe_publisher {

struct Settings final : public client::flags::Settings, public flags::Flags {
  explicit Settings(args::Parser const &);

  flags::Multicast multicast;
};

}  // namespace sbe_publisher
}  // namespace roq

template <>
struct fmt::formatter<roq::sbe_publisher::Settings> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::sbe_publisher::Settings const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(client={}, )"
        R"(flags={}, )"
        R"(multicast={})"
        R"(}})"sv,
        static_cast<roq::client::Settings2 const &>(value),
        static_cast<roq::sbe_publisher::flags::Flags const &>(value),
        value.multicast);
  }
};
