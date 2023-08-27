/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/sbe/codec/encoder.hpp"

#include "roq_sbe/MessageHeader.h"

#include "roq_sbe/MarketStatus.h"
#include "roq_sbe/ReferenceData.h"
#include "roq_sbe/TopOfBook.h"

using namespace std::literals;

namespace roq {
namespace sbe {
namespace codec {

// === HELPERS ===

namespace {
template <typename R, typename T>
R map_helper(T value);

template <>
int64_t map_helper(std::chrono::nanoseconds value) {
  return value.count();
}

template <>
int32_t map_helper(std::chrono::seconds value) {
  return value.count();
}

template <>
int32_t map_helper(std::chrono::days value) {
  return value.count();
}

template <>
roq_sbe::OptionType::Value map_helper(OptionType value) {
  switch (value) {
    using enum OptionType;
    case UNDEFINED:
      break;
    case CALL:
      return roq_sbe::OptionType::Value::Call;
    case PUT:
      return roq_sbe::OptionType::Value::Put;
  }
  return roq_sbe::OptionType::Value::Undefined;
}

template <>
roq_sbe::SecurityType::Value map_helper(SecurityType value) {
  switch (value) {
    using enum SecurityType;
    case UNDEFINED:
      break;
    case SPOT:
      return roq_sbe::SecurityType::Value::Spot;
    case FUTURES:
      return roq_sbe::SecurityType::Value::Futures;
    case OPTION:
      return roq_sbe::SecurityType::Value::Option;
    case SWAP:
      return roq_sbe::SecurityType::Value::Swap;
  }
  return roq_sbe::SecurityType::Value::Undefined;
}

template <>
roq_sbe::TradingStatus::Value map_helper(TradingStatus value) {
  switch (value) {
    using enum TradingStatus;
    case UNDEFINED:
      break;
    case START_OF_DAY:
      return roq_sbe::TradingStatus::Value::StartOfDay;
    case PRE_OPEN:
      return roq_sbe::TradingStatus::Value::PreOpen;
    case PRE_OPEN_NO_CANCEL:
      return roq_sbe::TradingStatus::Value::PreOpenNoCancel;
    case PRE_OPEN_FREEZE:
      return roq_sbe::TradingStatus::Value::PreOpenFreeze;
    case OPEN:
      return roq_sbe::TradingStatus::Value::Open;
    case FAST_MARKET:
      return roq_sbe::TradingStatus::Value::FastMarket;
    case HALT:
      return roq_sbe::TradingStatus::Value::Halt;
    case CLOSE_NOT_FINAL:
      return roq_sbe::TradingStatus::Value::CloseNotFinal;
    case PRE_CLOSE:
      return roq_sbe::TradingStatus::Value::PreClose;
    case PRE_CLOSE_NO_CANCEL:
      return roq_sbe::TradingStatus::Value::PreCloseNoCancel;
    case PRE_CLOSE_FREEZE:
      return roq_sbe::TradingStatus::Value::PreCloseFreeze;
    case CLOSE:
      return roq_sbe::TradingStatus::Value::Close;
    case POST_CLOSE:
      return roq_sbe::TradingStatus::Value::PostClose;
    case END_OF_DAY:
      return roq_sbe::TradingStatus::Value::EndOfDay;
  }
  return roq_sbe::TradingStatus::Value::Undefined;
}

template <>
roq_sbe::UpdateType::Value map_helper(UpdateType value) {
  switch (value) {
    using enum UpdateType;
    case UNDEFINED:
      break;
    case SNAPSHOT:
      return roq_sbe::UpdateType::Value::Snapshot;
    case INCREMENTAL:
      return roq_sbe::UpdateType::Value::Incremental;
    case STALE:
      return roq_sbe::UpdateType::Value::Stale;
  }
  return roq_sbe::UpdateType::Value::Undefined;
}

template <typename T>
struct map final {
  explicit map(T const &value) : value_{value} {}
  template <typename R>
  operator R() const {
    return map_helper<R>(value_);
  }

 private:
  T const value_;
};
}  // namespace

// === IMPLEMENTATION ===

std::span<std::byte const> Encoder::encode(std::span<std::byte> const &buffer, ReferenceData const &reference_data) {
  using message_header_type = roq_sbe::MessageHeader;
  using message_type = roq_sbe::ReferenceData;
  message_type value;
  auto &result = value.wrapAndApplyHeader(reinterpret_cast<char *>(std::data(buffer)), 0, std::size(buffer));
  result.putExchange(reference_data.exchange);
  result.putSymbol(reference_data.symbol);
  result.putDescription(reference_data.description);
  result.securityType(map(reference_data.security_type));
  result.putBaseCurrency(reference_data.base_currency);
  result.putQuoteCurrency(reference_data.quote_currency);
  result.putMarginCurrency(reference_data.margin_currency);
  result.putCommissionCurrency(reference_data.commission_currency);
  result.tickSize(reference_data.tick_size);
  result.multiplier(reference_data.multiplier);
  result.minNotional(reference_data.min_notional);
  result.minTradeVol(reference_data.min_trade_vol);
  result.maxTradeVol(reference_data.max_trade_vol);
  result.tradeVolStepSize(reference_data.trade_vol_step_size);
  result.optionType(map(reference_data.option_type));
  result.putStrikeCurrency(reference_data.strike_currency);
  result.strikePrice(reference_data.strike_price);
  result.putUnderlying(reference_data.underlying);
  result.putTimeZone(reference_data.time_zone);
  result.issueDate(map(reference_data.issue_date));
  result.settlementDate(map(reference_data.settlement_date));
  result.expiryDateTime(map(reference_data.expiry_datetime));
  result.expiryDateTimeUTC(map(reference_data.expiry_datetime_utc));
  auto length = message_header_type::encodedLength() + message_type::computeLength();
  return {std::data(buffer), length};
}

std::span<std::byte const> Encoder::encode(std::span<std::byte> const &buffer, MarketStatus const &market_status) {
  using message_header_type = roq_sbe::MessageHeader;
  using message_type = roq_sbe::MarketStatus;
  message_type value;
  auto &result = value.wrapAndApplyHeader(reinterpret_cast<char *>(std::data(buffer)), 0, std::size(buffer));
  result.putExchange(market_status.exchange);
  result.putSymbol(market_status.symbol);
  result.tradingStatus(map(market_status.trading_status));
  auto length = message_header_type::encodedLength() + message_type::computeLength();
  return {std::data(buffer), length};
}

std::span<std::byte const> Encoder::encode(std::span<std::byte> const &buffer, TopOfBook const &top_of_book) {
  using message_header_type = roq_sbe::MessageHeader;
  using message_type = roq_sbe::TopOfBook;
  message_type value;
  auto &result = value.wrapAndApplyHeader(reinterpret_cast<char *>(std::data(buffer)), 0, std::size(buffer));
  result.putExchange(top_of_book.exchange);
  result.putSymbol(top_of_book.symbol);
  auto &layer = result.layer();
  layer.bidPrice(top_of_book.layer.bid_price);
  layer.bidQuantity(top_of_book.layer.bid_quantity);
  layer.askPrice(top_of_book.layer.ask_price);
  layer.askQuantity(top_of_book.layer.ask_quantity);
  result.updateType(map(top_of_book.update_type));
  result.exchangeTimeUTC(map(top_of_book.exchange_time_utc));
  result.exchangeSequence(top_of_book.exchange_sequence);
  result.sendingTimeUTC(map(top_of_book.sending_time_utc));
  auto length = message_header_type::encodedLength() + message_type::computeLength();
  return {std::data(buffer), length};
}

}  // namespace codec
}  // namespace sbe
}  // namespace roq
