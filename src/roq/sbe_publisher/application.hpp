/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include <span>
#include <string_view>

#include "roq/service.hpp"

#include "roq/sbe_publisher/controller.hpp"

namespace roq {
namespace sbe_publisher {

struct Application final : public Service {
  using Service::Service;  // inherit constructors

 protected:
  int main(args::Parser const &) override;

 private:
  using value_type = Controller;  // note!
};

}  // namespace sbe_publisher
}  // namespace roq
