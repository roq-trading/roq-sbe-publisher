/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <cstdint>

namespace roq {
namespace sbe {
namespace publisher {

struct Shared final {
  Shared();

  uint16_t const session_id;
};

}  // namespace publisher
}  // namespace sbe
}  // namespace roq
