/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/client/config.hpp"

#include "roq/sbe/multicast/publisher/settings.hpp"

namespace roq {
namespace sbe {
namespace multicast {
namespace publisher {

struct Config final : public client::Config {
  explicit Config(Settings const &);

 protected:
  void dispatch(Handler &) const override;

 private:
  Settings const &settings_;
};

}  // namespace publisher
}  // namespace multicast
}  // namespace sbe
}  // namespace roq
