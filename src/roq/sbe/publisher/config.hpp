/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include "roq/utils/container.hpp"

#include "roq/client/config.hpp"

namespace roq {
namespace sbe {
namespace publisher {

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

}  // namespace publisher
}  // namespace sbe
}  // namespace roq
