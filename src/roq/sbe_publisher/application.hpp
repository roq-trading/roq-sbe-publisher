/* Copyright (c) 2017-2026, Hans Erik Thrane */

#pragma once

#include "roq/service.hpp"

namespace roq {
namespace sbe_publisher {

struct Application final : public Service {
  using Service::Service;

 protected:
  int main(args::Parser const &) override;
};

}  // namespace sbe_publisher
}  // namespace roq
