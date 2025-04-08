/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include "roq/utils/container.hpp"

#include "roq/client/config.hpp"

namespace roq {
namespace sbe_publisher {

struct Config final : public client::Config {
  Config(Config const &) = delete;

  static Config parse_file(std::string_view const &);
  static Config parse_text(std::string_view const &);

 protected:
  void dispatch(Handler &) const override;

 private:
  explicit Config(auto &node);

  utils::unordered_map<std::string, utils::unordered_set<std::string>> const symbols_;
};

}  // namespace sbe_publisher
}  // namespace roq
