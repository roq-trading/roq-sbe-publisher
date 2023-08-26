/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <span>
#include <string_view>

#include "roq/service.hpp"

#include "roq/sbe/multicast/publisher/controller.hpp"

namespace roq {
namespace sbe {
namespace multicast {
namespace publisher {

struct Application final : public Service {
  using Service::Service;  // inherit constructors

 protected:
  int main(args::Parser const &) override;

 private:
  using value_type = Controller;  // note!
};

}  // namespace publisher
}  // namespace multicast
}  // namespace sbe
}  // namespace roq
