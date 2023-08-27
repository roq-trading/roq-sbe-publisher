/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/sbe/publisher/config.hpp"

#include <toml++/toml.h>

#include "roq/logging.hpp"

using namespace std::literals;

namespace roq {
namespace sbe {
namespace publisher {

// === HELPERS ===

namespace {
void check_empty(auto &node) {
  if (!node.is_table())
    return;
  auto &table = *node.as_table();
  auto error = false;
  for (auto &[key, value] : table) {
    log::warn(R"(key="{}")"sv, static_cast<std::string_view>(key));
    error = true;
  }
  if (error)
    log::fatal("Unexpected"sv);
}

template <typename Callback>
bool find_and_remove(auto &node, std::string_view const &key, Callback callback) {
  if (!node.is_table()) {
    log::warn("Unexpected: node is not a table"sv);
    return false;
  }
  auto &table = *node.as_table();
  auto iter = table.find(key);
  if (iter == table.end())
    return false;
  callback((*iter).second);
  table.erase(iter);
  assert(table.find(key) == std::end(table));
  return true;
}

template <typename R>
R parse_symbols(auto &node) {
  using result_type = std::remove_cvref<R>::type;
  result_type result;
  auto parse_helper = [&](auto &node) {
    using value_type = typename R::value_type;
    if (node.is_value()) {
      result.emplace(*node.template value<value_type>());
    } else if (node.is_array()) {
      auto &arr = *node.as_array();
      for (auto &node_2 : arr) {
        result.emplace(*node_2.template value<value_type>());
      }
    } else {
      log::fatal("Unexpected"sv);
    }
  };
  if (find_and_remove(node, "symbols"sv, parse_helper)) {
  } else {
    log::fatal(R"(Unexpected: did not find the "symbols" table)"sv);
  }
  return result;
}
}  // namespace

// === IMPLEMENTATION ===

Config::Config(auto &node) : symbols{parse_symbols<decltype(symbols)>(node)} {
  check_empty(node);
}

void Config::dispatch(Handler &handler) const {
  for (auto &item : symbols) {
    auto symbol = client::Symbol{
        .regex = item,
        .exchange = ".*",
    };
    handler(symbol);
  }
}

Config Config::parse_file(std::string_view const &path) {
  auto root = toml::parse_file(path);
  return Config{root};
}

Config Config::parse_text(std::string_view const &text) {
  auto root = toml::parse(text);
  return Config{root};
}

}  // namespace publisher
}  // namespace sbe
}  // namespace roq
