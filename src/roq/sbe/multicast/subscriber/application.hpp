/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <span>
#include <string_view>

#include "roq/service.hpp"

namespace roq {
namespace sbe {
namespace multicast {
namespace subscriber {

struct Application final : public Service {
  using Service::Service;  // inherit constructors

 protected:
  int main(args::Parser const &) override;
};

}  // namespace subscriber
}  // namespace multicast
}  // namespace sbe
}  // namespace roq
