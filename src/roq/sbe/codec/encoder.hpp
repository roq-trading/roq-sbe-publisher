/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

namespace roq {
namespace sbe {
namespace codec {

struct Encoder final {
  static std::span<std::byte const> encode(std::span<std::byte> const &buffer, ReferenceData const &);
  static std::span<std::byte const> encode(std::span<std::byte> const &buffer, MarketStatus const &);
  static std::span<std::byte const> encode(std::span<std::byte> const &buffer, TopOfBook const &);
};

}  // namespace codec
}  // namespace sbe
}  // namespace roq
