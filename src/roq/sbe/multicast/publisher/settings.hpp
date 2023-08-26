/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <fmt/compile.h>
#include <fmt/format.h>

#include "roq/args/parser.hpp"

#include "roq/client/flags/settings.hpp"

#include "roq/sbe/multicast/publisher/flags/flags.hpp"

namespace roq {
namespace sbe {
namespace multicast {
namespace publisher {

struct Settings final : public client::flags::Settings, public flags::Flags {
  explicit Settings(args::Parser const &);
};

}  // namespace publisher
}  // namespace multicast
}  // namespace sbe
}  // namespace roq

template <>
struct fmt::formatter<roq::sbe::multicast::publisher::Settings> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::sbe::multicast::publisher::Settings const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(client={}, )"
        R"(flags={})"
        R"(}})"_cf,
        static_cast<roq::client::Settings2 const &>(value),
        static_cast<roq::sbe::multicast::publisher::flags::Flags const &>(value));
  }
};
