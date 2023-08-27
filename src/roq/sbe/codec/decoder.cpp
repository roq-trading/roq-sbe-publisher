/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include "roq/sbe/codec/decoder.hpp"

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
std::chrono::nanoseconds map_helper(int64_t value) {
  return std::chrono::nanoseconds{value};
}

template <>
std::chrono::seconds map_helper(int32_t value) {
  return std::chrono::seconds{value};
}

template <>
std::chrono::days map_helper(int32_t value) {
  return std::chrono::days{value};
}

template <>
OptionType map_helper(roq_sbe::OptionType::Value value) {
  switch (value) {
    using enum roq_sbe::OptionType::Value;
    case Undefined:
      break;
    case Call:
      return OptionType::CALL;
    case Put:
      return OptionType::PUT;
  }
  return OptionType::UNDEFINED;
}

template <>
SecurityType map_helper(roq_sbe::SecurityType::Value value) {
  switch (value) {
    using enum roq_sbe::SecurityType::Value;
    case Undefined:
      break;
    case Spot:
      return SecurityType::SPOT;
    case Futures:
      return SecurityType::FUTURES;
    case Option:
      return SecurityType::OPTION;
    case Swap:
      return SecurityType::SWAP;
  }
  return SecurityType::UNDEFINED;
}

template <>
TradingStatus map_helper(roq_sbe::TradingStatus::Value value) {
  switch (value) {
    using enum roq_sbe::TradingStatus::Value;
    case Undefined:
      break;
    case StartOfDay:
      return TradingStatus::START_OF_DAY;
    case PreOpen:
      return TradingStatus::PRE_OPEN;
    case PreOpenNoCancel:
      return TradingStatus::PRE_OPEN_NO_CANCEL;
    case PreOpenFreeze:
      return TradingStatus::PRE_OPEN_FREEZE;
    case Open:
      return TradingStatus::OPEN;
    case FastMarket:
      return TradingStatus::FAST_MARKET;
    case Halt:
      return TradingStatus::HALT;
    case CloseNotFinal:
      return TradingStatus::CLOSE_NOT_FINAL;
    case PreClose:
      return TradingStatus::PRE_CLOSE;
    case PreCloseNoCancel:
      return TradingStatus::PRE_CLOSE_NO_CANCEL;
    case PreCloseFreeze:
      return TradingStatus::PRE_CLOSE_FREEZE;
    case Close:
      return TradingStatus::CLOSE;
    case PostClose:
      return TradingStatus::POST_CLOSE;
    case EndOfDay:
      return TradingStatus::END_OF_DAY;
  }
  return TradingStatus::UNDEFINED;
}

template <>
UpdateType map_helper(roq_sbe::UpdateType::Value value) {
  switch (value) {
    using enum roq_sbe::UpdateType::Value;
    case Undefined:
      break;
    case Snapshot:
      return UpdateType::SNAPSHOT;
    case Incremental:
      return UpdateType::INCREMENTAL;
    case Stale:
      return UpdateType::STALE;
  }
  return UpdateType::UNDEFINED;
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

template <typename T>
auto get_encoded_length(T const &) {
  using value_type = std::remove_cvref<T>::type;
  return value_type::encodedLength();
}

struct Dispatcher {
  Dispatcher(
      Decoder::Handler &handler, std::span<std::byte const> const &buffer, roq_sbe::MessageHeader const &message_header)
      : handler_{handler}, buffer_{buffer.subspan(get_encoded_length(message_header))}, message_header_{message_header},
        block_length_{message_header.blockLength()}, version_{message_header.version()} {}

  template <typename T>
  size_t dispatch() const {
    return get_encoded_length(message_header_) + dispatch_helper<T>();
  }

 protected:
  template <typename T>
  size_t dispatch_helper() const;

 private:
  Decoder::Handler &handler_;
  std::span<std::byte const> const buffer_;
  roq_sbe::MessageHeader const &message_header_;
  uint16_t const block_length_;
  uint16_t const version_;
};

template <>
size_t Dispatcher::dispatch_helper<ReferenceData>() const {
  using mesage_type = roq_sbe::ReferenceData;
  mesage_type value{
      reinterpret_cast<char *>(const_cast<std::byte *>(std::data(buffer_))),
      std::size(buffer_),
      block_length_,
      version_};
  auto message_length = value.computeLength();
  auto reference_data = ReferenceData{
      .exchange = value.getExchangeAsStringView(),
      .symbol = value.getSymbolAsStringView(),
      .description = value.getDescriptionAsStringView(),
      .security_type = map(value.securityType()),
      .base_currency = value.getBaseCurrencyAsStringView(),
      .quote_currency = value.getQuoteCurrencyAsStringView(),
      .margin_currency = value.getMarginCurrencyAsStringView(),
      .commission_currency = value.getCommissionCurrencyAsStringView(),
      .tick_size = value.tickSize(),
      .multiplier = value.multiplier(),
      .min_notional = value.minNotional(),
      .min_trade_vol = value.minTradeVol(),
      .max_trade_vol = value.maxTradeVol(),
      .trade_vol_step_size = value.tradeVolStepSize(),
      .option_type = map(value.optionType()),
      .strike_currency = value.getStrikeCurrencyAsStringView(),
      .strike_price = value.strikePrice(),
      .underlying = value.getUnderlyingAsStringView(),
      .time_zone = value.getTimeZoneAsStringView(),
      .issue_date = map(value.issueDate()),
      .settlement_date = map(value.settlementDate()),
      .expiry_datetime = map(value.expiryDateTime()),
      .expiry_datetime_utc = map(value.expiryDateTimeUTC()),
  };
  handler_(reference_data);
  return message_length;
}

template <>
size_t Dispatcher::dispatch_helper<MarketStatus>() const {
  using mesage_type = roq_sbe::MarketStatus;
  mesage_type value{
      reinterpret_cast<char *>(const_cast<std::byte *>(std::data(buffer_))),
      std::size(buffer_),
      block_length_,
      version_};
  auto message_length = value.computeLength();
  auto market_status = MarketStatus{
      .exchange = value.getExchangeAsStringView(),
      .symbol = value.getSymbolAsStringView(),
      .trading_status = map(value.tradingStatus()),
  };
  handler_(market_status);
  return message_length;
}

template <>
size_t Dispatcher::dispatch_helper<TopOfBook>() const {
  using mesage_type = roq_sbe::TopOfBook;
  mesage_type value{
      reinterpret_cast<char *>(const_cast<std::byte *>(std::data(buffer_))),
      std::size(buffer_),
      block_length_,
      version_};
  auto message_length = value.computeLength();
  auto &layer = value.layer();
  auto top_of_book = TopOfBook{
      .exchange = value.getExchangeAsStringView(),
      .symbol = value.getSymbolAsStringView(),
      .layer =
          {
              .bid_price = layer.bidPrice(),
              .bid_quantity = layer.bidQuantity(),
              .ask_price = layer.askPrice(),
              .ask_quantity = layer.askQuantity(),
          },
      .update_type = map(value.updateType()),
      .exchange_time_utc = map(value.exchangeTimeUTC()),
      .exchange_sequence = value.exchangeSequence(),
      .sending_time_utc = map(value.sendingTimeUTC()),
  };
  handler_(top_of_book);
  return message_length;
}
}  // namespace

// === IMPLEMENTATION ===

size_t Decoder::decode(Handler &handler, std::span<std::byte const> const &buffer) {
  using message_header_type = roq_sbe::MessageHeader;
  message_header_type message_header{
      reinterpret_cast<char *>(const_cast<std::byte *>(std::data(buffer))), std::size(buffer)};
  // XXX TODO check version
  switch (message_header.templateId()) {
    case roq_sbe::ReferenceData::SBE_TEMPLATE_ID:
      return Dispatcher{handler, buffer, message_header}.dispatch<ReferenceData>();
    case roq_sbe::MarketStatus::SBE_TEMPLATE_ID:
      return Dispatcher{handler, buffer, message_header}.dispatch<MarketStatus>();
    case roq_sbe::TopOfBook::SBE_TEMPLATE_ID:
      return Dispatcher{handler, buffer, message_header}.dispatch<TopOfBook>();
  }
  // can't parse
  return 0;
}

}  // namespace codec
}  // namespace sbe
}  // namespace roq
