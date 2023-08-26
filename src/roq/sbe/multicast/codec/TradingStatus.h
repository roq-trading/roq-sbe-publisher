/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _CODEC_TRADINGSTATUS_CXX_H_
#define _CODEC_TRADINGSTATUS_CXX_H_

#if !defined(__STDC_LIMIT_MACROS)
#  define __STDC_LIMIT_MACROS 1
#endif

#include <cstdint>
#include <iomanip>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <sstream>
#include <string>

#define SBE_NULLVALUE_INT8 (std::numeric_limits<std::int8_t>::min)()
#define SBE_NULLVALUE_INT16 (std::numeric_limits<std::int16_t>::min)()
#define SBE_NULLVALUE_INT32 (std::numeric_limits<std::int32_t>::min)()
#define SBE_NULLVALUE_INT64 (std::numeric_limits<std::int64_t>::min)()
#define SBE_NULLVALUE_UINT8 (std::numeric_limits<std::uint8_t>::max)()
#define SBE_NULLVALUE_UINT16 (std::numeric_limits<std::uint16_t>::max)()
#define SBE_NULLVALUE_UINT32 (std::numeric_limits<std::uint32_t>::max)()
#define SBE_NULLVALUE_UINT64 (std::numeric_limits<std::uint64_t>::max)()

namespace codec {

class TradingStatus
{
public:
    enum Value
    {
        Undefined = static_cast<std::uint8_t>(0),
        StartOfDay = static_cast<std::uint8_t>(1),
        PreOpen = static_cast<std::uint8_t>(2),
        PreOpenNoCancel = static_cast<std::uint8_t>(3),
        PreOpenFreeze = static_cast<std::uint8_t>(4),
        Open = static_cast<std::uint8_t>(5),
        FastMarket = static_cast<std::uint8_t>(6),
        Halt = static_cast<std::uint8_t>(7),
        CloseNotFinal = static_cast<std::uint8_t>(8),
        PreClose = static_cast<std::uint8_t>(9),
        PreCloseNoCancel = static_cast<std::uint8_t>(10),
        PreCloseFreeze = static_cast<std::uint8_t>(11),
        Close = static_cast<std::uint8_t>(12),
        PostClose = static_cast<std::uint8_t>(13),
        EndOfDay = static_cast<std::uint8_t>(14),
        SBE_UNKNOWN = static_cast<std::uint8_t>(255),
        NULL_VALUE = static_cast<std::uint8_t>(255)
    };

    static TradingStatus::Value get(const std::uint8_t value)
    {
        switch (value)
        {
            case static_cast<std::uint8_t>(0): return Undefined;
            case static_cast<std::uint8_t>(1): return StartOfDay;
            case static_cast<std::uint8_t>(2): return PreOpen;
            case static_cast<std::uint8_t>(3): return PreOpenNoCancel;
            case static_cast<std::uint8_t>(4): return PreOpenFreeze;
            case static_cast<std::uint8_t>(5): return Open;
            case static_cast<std::uint8_t>(6): return FastMarket;
            case static_cast<std::uint8_t>(7): return Halt;
            case static_cast<std::uint8_t>(8): return CloseNotFinal;
            case static_cast<std::uint8_t>(9): return PreClose;
            case static_cast<std::uint8_t>(10): return PreCloseNoCancel;
            case static_cast<std::uint8_t>(11): return PreCloseFreeze;
            case static_cast<std::uint8_t>(12): return Close;
            case static_cast<std::uint8_t>(13): return PostClose;
            case static_cast<std::uint8_t>(14): return EndOfDay;
            case static_cast<std::uint8_t>(255): return NULL_VALUE;
        }

        return SBE_UNKNOWN;
    }

    static const char *c_str(const TradingStatus::Value value)
    {
        switch (value)
        {
            case Undefined: return "Undefined";
            case StartOfDay: return "StartOfDay";
            case PreOpen: return "PreOpen";
            case PreOpenNoCancel: return "PreOpenNoCancel";
            case PreOpenFreeze: return "PreOpenFreeze";
            case Open: return "Open";
            case FastMarket: return "FastMarket";
            case Halt: return "Halt";
            case CloseNotFinal: return "CloseNotFinal";
            case PreClose: return "PreClose";
            case PreCloseNoCancel: return "PreCloseNoCancel";
            case PreCloseFreeze: return "PreCloseFreeze";
            case Close: return "Close";
            case PostClose: return "PostClose";
            case EndOfDay: return "EndOfDay";
            case NULL_VALUE: return "NULL_VALUE";
        }

        return "SBE_UNKNOWN";
    }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> & operator << (
        std::basic_ostream<CharT, Traits> &os, TradingStatus::Value m)
    {
        return os << TradingStatus::c_str(m);
    }
};

}

#endif
