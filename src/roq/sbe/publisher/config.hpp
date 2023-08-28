/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <absl/container/flat_hash_map.h>
#include <absl/container/flat_hash_set.h>

#include "roq/client/config.hpp"

namespace roq {
namespace sbe {
namespace publisher {

struct Config final : public client::Config {
  static Config parse_file(std::string_view const &);
  static Config parse_text(std::string_view const &);

 protected:
  void dispatch(Handler &) const override;

 private:
  explicit Config(auto &node);

  absl::flat_hash_map<std::string, absl::flat_hash_set<std::string>> const symbols_;
};

}  // namespace publisher
}  // namespace sbe
}  // namespace roq
