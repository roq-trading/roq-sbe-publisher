/* Copyright (c) 2017-2025, Hans Erik Thrane */

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
void parse_symbols_helper(R &result, auto &node) {
  assert(node.is_table());
  for (auto &[key, item] : node) {
    if (item.is_array()) {
      auto &symbols = result[""sv];
      auto array = *item.as_array();
      for (auto &item : array)
        symbols.emplace(*item.template value<std::string_view>());
    } else if (item.is_table()) {
      auto &symbols = result[key];
      auto table = *item.as_table();
      for (auto &[key_2, item_2] : table) {
        if (key_2 == "regex"sv) {
          if (item_2.is_value()) {
            symbols.emplace(*item_2.template value<std::string_view>());
          } else if (item_2.is_array()) {
            auto array = *item_2.as_array();
            for (auto &item_3 : array)
              symbols.emplace(*item_3.template value<std::string_view>());
          } else {
            log::fatal("Unexpected: regex must be value or array"sv);
          }
        } else {
          log::fatal("Unexpected: regex"sv);
        }
      }
    } else {
      log::fatal("Unexpected: regex"sv);
    }
  }
}

template <typename R>
R parse_symbols(auto &node) {
  using result_type = std::remove_cvref<R>::type;
  result_type result;
  auto parse_helper = [&](auto &node) {
    if (node.is_table()) {
      parse_symbols_helper(result, *node.as_table());
    } else {
      log::fatal("Unexpected: symbols must be list of exchanges against symbol regex"sv);
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

Config::Config(auto &node) : symbols_{parse_symbols<decltype(symbols_)>(node)} {
  check_empty(node);
}

void Config::dispatch(Handler &handler) const {
  for (auto &[exchange, symbols] : symbols_)
    for (auto &item : symbols) {
      auto symbol = client::Symbol{
          .regex = item,
          .exchange = exchange,
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
