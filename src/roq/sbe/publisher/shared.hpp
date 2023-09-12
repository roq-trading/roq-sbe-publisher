/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

namespace roq {
namespace sbe {
namespace publisher {

struct Shared final {
  Shared();

  Shared(Shared const &) = delete;

  uint16_t const session_id;

  Source source_name;
  UUID source_session_id = {};
  uint64_t source_seqno = {};
};

}  // namespace publisher
}  // namespace sbe
}  // namespace roq
