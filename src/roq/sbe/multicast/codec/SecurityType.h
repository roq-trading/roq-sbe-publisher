/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _CODEC_SECURITYTYPE_CXX_H_
#define _CODEC_SECURITYTYPE_CXX_H_

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

class SecurityType
{
public:
    enum Value
    {
        Undefined = static_cast<std::uint8_t>(0),
        Spot = static_cast<std::uint8_t>(1),
        Futures = static_cast<std::uint8_t>(2),
        Option = static_cast<std::uint8_t>(3),
        Swap = static_cast<std::uint8_t>(4),
        SBE_UNKNOWN = static_cast<std::uint8_t>(255),
        NULL_VALUE = static_cast<std::uint8_t>(255)
    };

    static SecurityType::Value get(const std::uint8_t value)
    {
        switch (value)
        {
            case static_cast<std::uint8_t>(0): return Undefined;
            case static_cast<std::uint8_t>(1): return Spot;
            case static_cast<std::uint8_t>(2): return Futures;
            case static_cast<std::uint8_t>(3): return Option;
            case static_cast<std::uint8_t>(4): return Swap;
            case static_cast<std::uint8_t>(255): return NULL_VALUE;
        }

        return SBE_UNKNOWN;
    }

    static const char *c_str(const SecurityType::Value value)
    {
        switch (value)
        {
            case Undefined: return "Undefined";
            case Spot: return "Spot";
            case Futures: return "Futures";
            case Option: return "Option";
            case Swap: return "Swap";
            case NULL_VALUE: return "NULL_VALUE";
        }

        return "SBE_UNKNOWN";
    }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> & operator << (
        std::basic_ostream<CharT, Traits> &os, SecurityType::Value m)
    {
        return os << SecurityType::c_str(m);
    }
};

}

#endif
