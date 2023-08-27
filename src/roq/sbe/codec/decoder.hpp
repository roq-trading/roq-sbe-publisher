/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

namespace roq {
namespace sbe {
namespace codec {

struct Decoder final {
  struct Handler {
    virtual void operator()(ReferenceData const &) = 0;
    virtual void operator()(MarketStatus const &) = 0;
    virtual void operator()(TopOfBook const &) = 0;
  };
  static size_t decode(Handler &, std::span<std::byte const> const &buffer);
};

}  // namespace codec
}  // namespace sbe
}  // namespace roq
