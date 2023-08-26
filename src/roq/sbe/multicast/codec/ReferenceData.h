/* Generated SBE (Simple Binary Encoding) message codec */
#ifndef _CODEC_REFERENCEDATA_CXX_H_
#define _CODEC_REFERENCEDATA_CXX_H_

#if defined(SBE_HAVE_CMATH)
/* cmath needed for std::numeric_limits<double>::quiet_NaN() */
#  include <cmath>
#  define SBE_FLOAT_NAN std::numeric_limits<float>::quiet_NaN()
#  define SBE_DOUBLE_NAN std::numeric_limits<double>::quiet_NaN()
#else
/* math.h needed for NAN */
#  include <math.h>
#  define SBE_FLOAT_NAN NAN
#  define SBE_DOUBLE_NAN NAN
#endif

#if __cplusplus >= 201103L
#  define SBE_CONSTEXPR constexpr
#  define SBE_NOEXCEPT noexcept
#else
#  define SBE_CONSTEXPR
#  define SBE_NOEXCEPT
#endif

#if __cplusplus >= 201703L
#  include <string_view>
#  define SBE_NODISCARD [[nodiscard]]
#else
#  define SBE_NODISCARD
#endif

#if !defined(__STDC_LIMIT_MACROS)
#  define __STDC_LIMIT_MACROS 1
#endif

#include <cstdint>
#include <cstring>
#include <iomanip>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>
#include <tuple>

#if defined(WIN32) || defined(_WIN32)
#  define SBE_BIG_ENDIAN_ENCODE_16(v) _byteswap_ushort(v)
#  define SBE_BIG_ENDIAN_ENCODE_32(v) _byteswap_ulong(v)
#  define SBE_BIG_ENDIAN_ENCODE_64(v) _byteswap_uint64(v)
#  define SBE_LITTLE_ENDIAN_ENCODE_16(v) (v)
#  define SBE_LITTLE_ENDIAN_ENCODE_32(v) (v)
#  define SBE_LITTLE_ENDIAN_ENCODE_64(v) (v)
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#  define SBE_BIG_ENDIAN_ENCODE_16(v) __builtin_bswap16(v)
#  define SBE_BIG_ENDIAN_ENCODE_32(v) __builtin_bswap32(v)
#  define SBE_BIG_ENDIAN_ENCODE_64(v) __builtin_bswap64(v)
#  define SBE_LITTLE_ENDIAN_ENCODE_16(v) (v)
#  define SBE_LITTLE_ENDIAN_ENCODE_32(v) (v)
#  define SBE_LITTLE_ENDIAN_ENCODE_64(v) (v)
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#  define SBE_LITTLE_ENDIAN_ENCODE_16(v) __builtin_bswap16(v)
#  define SBE_LITTLE_ENDIAN_ENCODE_32(v) __builtin_bswap32(v)
#  define SBE_LITTLE_ENDIAN_ENCODE_64(v) __builtin_bswap64(v)
#  define SBE_BIG_ENDIAN_ENCODE_16(v) (v)
#  define SBE_BIG_ENDIAN_ENCODE_32(v) (v)
#  define SBE_BIG_ENDIAN_ENCODE_64(v) (v)
#else
#  error "Byte Ordering of platform not determined. Set __BYTE_ORDER__ manually before including this file."
#endif

#if !defined(SBE_BOUNDS_CHECK_EXPECT)
#  if defined(SBE_NO_BOUNDS_CHECK)
#    define SBE_BOUNDS_CHECK_EXPECT(exp, c) (false)
#  elif defined(_MSC_VER)
#    define SBE_BOUNDS_CHECK_EXPECT(exp, c) (exp)
#  else 
#    define SBE_BOUNDS_CHECK_EXPECT(exp, c) (__builtin_expect(exp, c))
#  endif

#endif

#define SBE_NULLVALUE_INT8 (std::numeric_limits<std::int8_t>::min)()
#define SBE_NULLVALUE_INT16 (std::numeric_limits<std::int16_t>::min)()
#define SBE_NULLVALUE_INT32 (std::numeric_limits<std::int32_t>::min)()
#define SBE_NULLVALUE_INT64 (std::numeric_limits<std::int64_t>::min)()
#define SBE_NULLVALUE_UINT8 (std::numeric_limits<std::uint8_t>::max)()
#define SBE_NULLVALUE_UINT16 (std::numeric_limits<std::uint16_t>::max)()
#define SBE_NULLVALUE_UINT32 (std::numeric_limits<std::uint32_t>::max)()
#define SBE_NULLVALUE_UINT64 (std::numeric_limits<std::uint64_t>::max)()


#include "SecurityType.h"
#include "MessageHeader.h"
#include "TradingStatus.h"
#include "GroupSize.h"
#include "Layer.h"
#include "UpdateType.h"
#include "OptionType.h"

namespace codec {

class ReferenceData
{
private:
    char *m_buffer = nullptr;
    std::uint64_t m_bufferLength = 0;
    std::uint64_t m_offset = 0;
    std::uint64_t m_position = 0;
    std::uint64_t m_actingBlockLength = 0;
    std::uint64_t m_actingVersion = 0;

    inline std::uint64_t *sbePositionPtr() SBE_NOEXCEPT
    {
        return &m_position;
    }

public:
    static const std::uint16_t SBE_BLOCK_LENGTH = static_cast<std::uint16_t>(530);
    static const std::uint16_t SBE_TEMPLATE_ID = static_cast<std::uint16_t>(10);
    static const std::uint16_t SBE_SCHEMA_ID = static_cast<std::uint16_t>(1);
    static const std::uint16_t SBE_SCHEMA_VERSION = static_cast<std::uint16_t>(1);
    static constexpr const char* SBE_SEMANTIC_VERSION = "";

    enum MetaAttribute
    {
        EPOCH, TIME_UNIT, SEMANTIC_TYPE, PRESENCE
    };

    union sbe_float_as_uint_u
    {
        float fp_value;
        std::uint32_t uint_value;
    };

    union sbe_double_as_uint_u
    {
        double fp_value;
        std::uint64_t uint_value;
    };

    using messageHeader = MessageHeader;

    ReferenceData() = default;

    ReferenceData(
        char *buffer,
        const std::uint64_t offset,
        const std::uint64_t bufferLength,
        const std::uint64_t actingBlockLength,
        const std::uint64_t actingVersion) :
        m_buffer(buffer),
        m_bufferLength(bufferLength),
        m_offset(offset),
        m_position(sbeCheckPosition(offset + actingBlockLength)),
        m_actingBlockLength(actingBlockLength),
        m_actingVersion(actingVersion)
    {
    }

    ReferenceData(char *buffer, const std::uint64_t bufferLength) :
        ReferenceData(buffer, 0, bufferLength, sbeBlockLength(), sbeSchemaVersion())
    {
    }

    ReferenceData(
        char *buffer,
        const std::uint64_t bufferLength,
        const std::uint64_t actingBlockLength,
        const std::uint64_t actingVersion) :
        ReferenceData(buffer, 0, bufferLength, actingBlockLength, actingVersion)
    {
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::uint16_t sbeBlockLength() SBE_NOEXCEPT
    {
        return static_cast<std::uint16_t>(530);
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::uint64_t sbeBlockAndHeaderLength() SBE_NOEXCEPT
    {
        return messageHeader::encodedLength() + sbeBlockLength();
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::uint16_t sbeTemplateId() SBE_NOEXCEPT
    {
        return static_cast<std::uint16_t>(10);
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::uint16_t sbeSchemaId() SBE_NOEXCEPT
    {
        return static_cast<std::uint16_t>(1);
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::uint16_t sbeSchemaVersion() SBE_NOEXCEPT
    {
        return static_cast<std::uint16_t>(1);
    }

    SBE_NODISCARD static const char *sbeSemanticVersion() SBE_NOEXCEPT
    {
        return "";
    }

    SBE_NODISCARD static SBE_CONSTEXPR const char *sbeSemanticType() SBE_NOEXCEPT
    {
        return "";
    }

    SBE_NODISCARD std::uint64_t offset() const SBE_NOEXCEPT
    {
        return m_offset;
    }

    ReferenceData &wrapForEncode(char *buffer, const std::uint64_t offset, const std::uint64_t bufferLength)
    {
        return *this = ReferenceData(buffer, offset, bufferLength, sbeBlockLength(), sbeSchemaVersion());
    }

    ReferenceData &wrapAndApplyHeader(char *buffer, const std::uint64_t offset, const std::uint64_t bufferLength)
    {
        messageHeader hdr(buffer, offset, bufferLength, sbeSchemaVersion());

        hdr
            .blockLength(sbeBlockLength())
            .templateId(sbeTemplateId())
            .schemaId(sbeSchemaId())
            .version(sbeSchemaVersion());

        return *this = ReferenceData(
            buffer,
            offset + messageHeader::encodedLength(),
            bufferLength,
            sbeBlockLength(),
            sbeSchemaVersion());
    }

    ReferenceData &wrapForDecode(
        char *buffer,
        const std::uint64_t offset,
        const std::uint64_t actingBlockLength,
        const std::uint64_t actingVersion,
        const std::uint64_t bufferLength)
    {
        return *this = ReferenceData(buffer, offset, bufferLength, actingBlockLength, actingVersion);
    }

    ReferenceData &sbeRewind()
    {
        return wrapForDecode(m_buffer, m_offset, m_actingBlockLength, m_actingVersion, m_bufferLength);
    }

    SBE_NODISCARD std::uint64_t sbePosition() const SBE_NOEXCEPT
    {
        return m_position;
    }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    std::uint64_t sbeCheckPosition(const std::uint64_t position)
    {
        if (SBE_BOUNDS_CHECK_EXPECT((position > m_bufferLength), false))
        {
            throw std::runtime_error("buffer too short [E100]");
        }
        return position;
    }

    void sbePosition(const std::uint64_t position)
    {
        m_position = sbeCheckPosition(position);
    }

    SBE_NODISCARD std::uint64_t encodedLength() const SBE_NOEXCEPT
    {
        return sbePosition() - m_offset;
    }

    SBE_NODISCARD std::uint64_t decodeLength() const
    {
        ReferenceData skipper(m_buffer, m_offset, m_bufferLength, sbeBlockLength(), m_actingVersion);
        skipper.skip();
        return skipper.encodedLength();
    }

    SBE_NODISCARD const char *buffer() const SBE_NOEXCEPT
    {
        return m_buffer;
    }

    SBE_NODISCARD char *buffer() SBE_NOEXCEPT
    {
        return m_buffer;
    }

    SBE_NODISCARD std::uint64_t bufferLength() const SBE_NOEXCEPT
    {
        return m_bufferLength;
    }

    SBE_NODISCARD std::uint64_t actingVersion() const SBE_NOEXCEPT
    {
        return m_actingVersion;
    }

    SBE_NODISCARD static const char *exchangeMetaAttribute(const MetaAttribute metaAttribute) SBE_NOEXCEPT
    {
        switch (metaAttribute)
        {
            case MetaAttribute::PRESENCE: return "required";
            default: return "";
        }
    }

    static SBE_CONSTEXPR std::uint16_t exchangeId() SBE_NOEXCEPT
    {
        return 1;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::uint64_t exchangeSinceVersion() SBE_NOEXCEPT
    {
        return 0;
    }

    SBE_NODISCARD bool exchangeInActingVersion() SBE_NOEXCEPT
    {
        return true;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::size_t exchangeEncodingOffset() SBE_NOEXCEPT
    {
        return 0;
    }

    static SBE_CONSTEXPR char exchangeNullValue() SBE_NOEXCEPT
    {
        return static_cast<char>(0);
    }

    static SBE_CONSTEXPR char exchangeMinValue() SBE_NOEXCEPT
    {
        return static_cast<char>(32);
    }

    static SBE_CONSTEXPR char exchangeMaxValue() SBE_NOEXCEPT
    {
        return static_cast<char>(126);
    }

    static SBE_CONSTEXPR std::size_t exchangeEncodingLength() SBE_NOEXCEPT
    {
        return 32;
    }

    static SBE_CONSTEXPR std::uint64_t exchangeLength() SBE_NOEXCEPT
    {
        return 32;
    }

    SBE_NODISCARD const char *exchange() const SBE_NOEXCEPT
    {
        return m_buffer + m_offset + 0;
    }

    SBE_NODISCARD char *exchange() SBE_NOEXCEPT
    {
        return m_buffer + m_offset + 0;
    }

    SBE_NODISCARD char exchange(const std::uint64_t index) const
    {
        if (index >= 32)
        {
            throw std::runtime_error("index out of range for exchange [E104]");
        }

        char val;
        std::memcpy(&val, m_buffer + m_offset + 0 + (index * 1), sizeof(char));
        return (val);
    }

    ReferenceData &exchange(const std::uint64_t index, const char value)
    {
        if (index >= 32)
        {
            throw std::runtime_error("index out of range for exchange [E105]");
        }

        char val = (value);
        std::memcpy(m_buffer + m_offset + 0 + (index * 1), &val, sizeof(char));
        return *this;
    }

    std::uint64_t getExchange(char *const dst, const std::uint64_t length) const
    {
        if (length > 32)
        {
            throw std::runtime_error("length too large for getExchange [E106]");
        }

        std::memcpy(dst, m_buffer + m_offset + 0, sizeof(char) * static_cast<std::size_t>(length));
        return length;
    }

    ReferenceData &putExchange(const char *const src) SBE_NOEXCEPT
    {
        std::memcpy(m_buffer + m_offset + 0, src, sizeof(char) * 32);
        return *this;
    }

    SBE_NODISCARD std::string getExchangeAsString() const
    {
        const char *buffer = m_buffer + m_offset + 0;
        std::size_t length = 0;

        for (; length < 32 && *(buffer + length) != '\0'; ++length);
        std::string result(buffer, length);

        return result;
    }

    std::string getExchangeAsJsonEscapedString()
    {
        std::ostringstream oss;
        std::string s = getExchangeAsString();

        for (const auto c : s)
        {
            switch (c)
            {
                case '"': oss << "\\\""; break;
                case '\\': oss << "\\\\"; break;
                case '\b': oss << "\\b"; break;
                case '\f': oss << "\\f"; break;
                case '\n': oss << "\\n"; break;
                case '\r': oss << "\\r"; break;
                case '\t': oss << "\\t"; break;

                default:
                    if ('\x00' <= c && c <= '\x1f')
                    {
                        oss << "\\u" << std::hex << std::setw(4)
                            << std::setfill('0') << (int)(c);
                    }
                    else
                    {
                        oss << c;
                    }
            }
        }

        return oss.str();
    }

    #if __cplusplus >= 201703L
    SBE_NODISCARD std::string_view getExchangeAsStringView() const SBE_NOEXCEPT
    {
        const char *buffer = m_buffer + m_offset + 0;
        std::size_t length = 0;

        for (; length < 32 && *(buffer + length) != '\0'; ++length);
        std::string_view result(buffer, length);

        return result;
    }
    #endif

    #if __cplusplus >= 201703L
    ReferenceData &putExchange(const std::string_view str)
    {
        const std::size_t srcLength = str.length();
        if (srcLength > 32)
        {
            throw std::runtime_error("string too large for putExchange [E106]");
        }

        std::memcpy(m_buffer + m_offset + 0, str.data(), srcLength);
        for (std::size_t start = srcLength; start < 32; ++start)
        {
            m_buffer[m_offset + 0 + start] = 0;
        }

        return *this;
    }
    #else
    ReferenceData &putExchange(const std::string &str)
    {
        const std::size_t srcLength = str.length();
        if (srcLength > 32)
        {
            throw std::runtime_error("string too large for putExchange [E106]");
        }

        std::memcpy(m_buffer + m_offset + 0, str.c_str(), srcLength);
        for (std::size_t start = srcLength; start < 32; ++start)
        {
            m_buffer[m_offset + 0 + start] = 0;
        }

        return *this;
    }
    #endif

    SBE_NODISCARD static const char *symbolMetaAttribute(const MetaAttribute metaAttribute) SBE_NOEXCEPT
    {
        switch (metaAttribute)
        {
            case MetaAttribute::PRESENCE: return "required";
            default: return "";
        }
    }

    static SBE_CONSTEXPR std::uint16_t symbolId() SBE_NOEXCEPT
    {
        return 2;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::uint64_t symbolSinceVersion() SBE_NOEXCEPT
    {
        return 0;
    }

    SBE_NODISCARD bool symbolInActingVersion() SBE_NOEXCEPT
    {
        return true;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::size_t symbolEncodingOffset() SBE_NOEXCEPT
    {
        return 32;
    }

    static SBE_CONSTEXPR char symbolNullValue() SBE_NOEXCEPT
    {
        return static_cast<char>(0);
    }

    static SBE_CONSTEXPR char symbolMinValue() SBE_NOEXCEPT
    {
        return static_cast<char>(32);
    }

    static SBE_CONSTEXPR char symbolMaxValue() SBE_NOEXCEPT
    {
        return static_cast<char>(126);
    }

    static SBE_CONSTEXPR std::size_t symbolEncodingLength() SBE_NOEXCEPT
    {
        return 48;
    }

    static SBE_CONSTEXPR std::uint64_t symbolLength() SBE_NOEXCEPT
    {
        return 48;
    }

    SBE_NODISCARD const char *symbol() const SBE_NOEXCEPT
    {
        return m_buffer + m_offset + 32;
    }

    SBE_NODISCARD char *symbol() SBE_NOEXCEPT
    {
        return m_buffer + m_offset + 32;
    }

    SBE_NODISCARD char symbol(const std::uint64_t index) const
    {
        if (index >= 48)
        {
            throw std::runtime_error("index out of range for symbol [E104]");
        }

        char val;
        std::memcpy(&val, m_buffer + m_offset + 32 + (index * 1), sizeof(char));
        return (val);
    }

    ReferenceData &symbol(const std::uint64_t index, const char value)
    {
        if (index >= 48)
        {
            throw std::runtime_error("index out of range for symbol [E105]");
        }

        char val = (value);
        std::memcpy(m_buffer + m_offset + 32 + (index * 1), &val, sizeof(char));
        return *this;
    }

    std::uint64_t getSymbol(char *const dst, const std::uint64_t length) const
    {
        if (length > 48)
        {
            throw std::runtime_error("length too large for getSymbol [E106]");
        }

        std::memcpy(dst, m_buffer + m_offset + 32, sizeof(char) * static_cast<std::size_t>(length));
        return length;
    }

    ReferenceData &putSymbol(const char *const src) SBE_NOEXCEPT
    {
        std::memcpy(m_buffer + m_offset + 32, src, sizeof(char) * 48);
        return *this;
    }

    SBE_NODISCARD std::string getSymbolAsString() const
    {
        const char *buffer = m_buffer + m_offset + 32;
        std::size_t length = 0;

        for (; length < 48 && *(buffer + length) != '\0'; ++length);
        std::string result(buffer, length);

        return result;
    }

    std::string getSymbolAsJsonEscapedString()
    {
        std::ostringstream oss;
        std::string s = getSymbolAsString();

        for (const auto c : s)
        {
            switch (c)
            {
                case '"': oss << "\\\""; break;
                case '\\': oss << "\\\\"; break;
                case '\b': oss << "\\b"; break;
                case '\f': oss << "\\f"; break;
                case '\n': oss << "\\n"; break;
                case '\r': oss << "\\r"; break;
                case '\t': oss << "\\t"; break;

                default:
                    if ('\x00' <= c && c <= '\x1f')
                    {
                        oss << "\\u" << std::hex << std::setw(4)
                            << std::setfill('0') << (int)(c);
                    }
                    else
                    {
                        oss << c;
                    }
            }
        }

        return oss.str();
    }

    #if __cplusplus >= 201703L
    SBE_NODISCARD std::string_view getSymbolAsStringView() const SBE_NOEXCEPT
    {
        const char *buffer = m_buffer + m_offset + 32;
        std::size_t length = 0;

        for (; length < 48 && *(buffer + length) != '\0'; ++length);
        std::string_view result(buffer, length);

        return result;
    }
    #endif

    #if __cplusplus >= 201703L
    ReferenceData &putSymbol(const std::string_view str)
    {
        const std::size_t srcLength = str.length();
        if (srcLength > 48)
        {
            throw std::runtime_error("string too large for putSymbol [E106]");
        }

        std::memcpy(m_buffer + m_offset + 32, str.data(), srcLength);
        for (std::size_t start = srcLength; start < 48; ++start)
        {
            m_buffer[m_offset + 32 + start] = 0;
        }

        return *this;
    }
    #else
    ReferenceData &putSymbol(const std::string &str)
    {
        const std::size_t srcLength = str.length();
        if (srcLength > 48)
        {
            throw std::runtime_error("string too large for putSymbol [E106]");
        }

        std::memcpy(m_buffer + m_offset + 32, str.c_str(), srcLength);
        for (std::size_t start = srcLength; start < 48; ++start)
        {
            m_buffer[m_offset + 32 + start] = 0;
        }

        return *this;
    }
    #endif

    SBE_NODISCARD static const char *descriptionMetaAttribute(const MetaAttribute metaAttribute) SBE_NOEXCEPT
    {
        switch (metaAttribute)
        {
            case MetaAttribute::PRESENCE: return "required";
            default: return "";
        }
    }

    static SBE_CONSTEXPR std::uint16_t descriptionId() SBE_NOEXCEPT
    {
        return 3;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::uint64_t descriptionSinceVersion() SBE_NOEXCEPT
    {
        return 0;
    }

    SBE_NODISCARD bool descriptionInActingVersion() SBE_NOEXCEPT
    {
        return true;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::size_t descriptionEncodingOffset() SBE_NOEXCEPT
    {
        return 80;
    }

    static SBE_CONSTEXPR char descriptionNullValue() SBE_NOEXCEPT
    {
        return static_cast<char>(0);
    }

    static SBE_CONSTEXPR char descriptionMinValue() SBE_NOEXCEPT
    {
        return static_cast<char>(32);
    }

    static SBE_CONSTEXPR char descriptionMaxValue() SBE_NOEXCEPT
    {
        return static_cast<char>(126);
    }

    static SBE_CONSTEXPR std::size_t descriptionEncodingLength() SBE_NOEXCEPT
    {
        return 128;
    }

    static SBE_CONSTEXPR std::uint64_t descriptionLength() SBE_NOEXCEPT
    {
        return 128;
    }

    SBE_NODISCARD const char *description() const SBE_NOEXCEPT
    {
        return m_buffer + m_offset + 80;
    }

    SBE_NODISCARD char *description() SBE_NOEXCEPT
    {
        return m_buffer + m_offset + 80;
    }

    SBE_NODISCARD char description(const std::uint64_t index) const
    {
        if (index >= 128)
        {
            throw std::runtime_error("index out of range for description [E104]");
        }

        char val;
        std::memcpy(&val, m_buffer + m_offset + 80 + (index * 1), sizeof(char));
        return (val);
    }

    ReferenceData &description(const std::uint64_t index, const char value)
    {
        if (index >= 128)
        {
            throw std::runtime_error("index out of range for description [E105]");
        }

        char val = (value);
        std::memcpy(m_buffer + m_offset + 80 + (index * 1), &val, sizeof(char));
        return *this;
    }

    std::uint64_t getDescription(char *const dst, const std::uint64_t length) const
    {
        if (length > 128)
        {
            throw std::runtime_error("length too large for getDescription [E106]");
        }

        std::memcpy(dst, m_buffer + m_offset + 80, sizeof(char) * static_cast<std::size_t>(length));
        return length;
    }

    ReferenceData &putDescription(const char *const src) SBE_NOEXCEPT
    {
        std::memcpy(m_buffer + m_offset + 80, src, sizeof(char) * 128);
        return *this;
    }

    SBE_NODISCARD std::string getDescriptionAsString() const
    {
        const char *buffer = m_buffer + m_offset + 80;
        std::size_t length = 0;

        for (; length < 128 && *(buffer + length) != '\0'; ++length);
        std::string result(buffer, length);

        return result;
    }

    std::string getDescriptionAsJsonEscapedString()
    {
        std::ostringstream oss;
        std::string s = getDescriptionAsString();

        for (const auto c : s)
        {
            switch (c)
            {
                case '"': oss << "\\\""; break;
                case '\\': oss << "\\\\"; break;
                case '\b': oss << "\\b"; break;
                case '\f': oss << "\\f"; break;
                case '\n': oss << "\\n"; break;
                case '\r': oss << "\\r"; break;
                case '\t': oss << "\\t"; break;

                default:
                    if ('\x00' <= c && c <= '\x1f')
                    {
                        oss << "\\u" << std::hex << std::setw(4)
                            << std::setfill('0') << (int)(c);
                    }
                    else
                    {
                        oss << c;
                    }
            }
        }

        return oss.str();
    }

    #if __cplusplus >= 201703L
    SBE_NODISCARD std::string_view getDescriptionAsStringView() const SBE_NOEXCEPT
    {
        const char *buffer = m_buffer + m_offset + 80;
        std::size_t length = 0;

        for (; length < 128 && *(buffer + length) != '\0'; ++length);
        std::string_view result(buffer, length);

        return result;
    }
    #endif

    #if __cplusplus >= 201703L
    ReferenceData &putDescription(const std::string_view str)
    {
        const std::size_t srcLength = str.length();
        if (srcLength > 128)
        {
            throw std::runtime_error("string too large for putDescription [E106]");
        }

        std::memcpy(m_buffer + m_offset + 80, str.data(), srcLength);
        for (std::size_t start = srcLength; start < 128; ++start)
        {
            m_buffer[m_offset + 80 + start] = 0;
        }

        return *this;
    }
    #else
    ReferenceData &putDescription(const std::string &str)
    {
        const std::size_t srcLength = str.length();
        if (srcLength > 128)
        {
            throw std::runtime_error("string too large for putDescription [E106]");
        }

        std::memcpy(m_buffer + m_offset + 80, str.c_str(), srcLength);
        for (std::size_t start = srcLength; start < 128; ++start)
        {
            m_buffer[m_offset + 80 + start] = 0;
        }

        return *this;
    }
    #endif

    SBE_NODISCARD static const char *security_typeMetaAttribute(const MetaAttribute metaAttribute) SBE_NOEXCEPT
    {
        switch (metaAttribute)
        {
            case MetaAttribute::PRESENCE: return "required";
            default: return "";
        }
    }

    static SBE_CONSTEXPR std::uint16_t security_typeId() SBE_NOEXCEPT
    {
        return 4;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::uint64_t security_typeSinceVersion() SBE_NOEXCEPT
    {
        return 0;
    }

    SBE_NODISCARD bool security_typeInActingVersion() SBE_NOEXCEPT
    {
        return true;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::size_t security_typeEncodingOffset() SBE_NOEXCEPT
    {
        return 208;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::size_t security_typeEncodingLength() SBE_NOEXCEPT
    {
        return 1;
    }

    SBE_NODISCARD std::uint8_t security_typeRaw() const SBE_NOEXCEPT
    {
        std::uint8_t val;
        std::memcpy(&val, m_buffer + m_offset + 208, sizeof(std::uint8_t));
        return (val);
    }

    SBE_NODISCARD SecurityType::Value security_type() const
    {
        std::uint8_t val;
        std::memcpy(&val, m_buffer + m_offset + 208, sizeof(std::uint8_t));
        return SecurityType::get((val));
    }

    ReferenceData &security_type(const SecurityType::Value value) SBE_NOEXCEPT
    {
        std::uint8_t val = (value);
        std::memcpy(m_buffer + m_offset + 208, &val, sizeof(std::uint8_t));
        return *this;
    }

    SBE_NODISCARD static const char *baseCurrencyMetaAttribute(const MetaAttribute metaAttribute) SBE_NOEXCEPT
    {
        switch (metaAttribute)
        {
            case MetaAttribute::PRESENCE: return "required";
            default: return "";
        }
    }

    static SBE_CONSTEXPR std::uint16_t baseCurrencyId() SBE_NOEXCEPT
    {
        return 5;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::uint64_t baseCurrencySinceVersion() SBE_NOEXCEPT
    {
        return 0;
    }

    SBE_NODISCARD bool baseCurrencyInActingVersion() SBE_NOEXCEPT
    {
        return true;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::size_t baseCurrencyEncodingOffset() SBE_NOEXCEPT
    {
        return 209;
    }

    static SBE_CONSTEXPR char baseCurrencyNullValue() SBE_NOEXCEPT
    {
        return static_cast<char>(0);
    }

    static SBE_CONSTEXPR char baseCurrencyMinValue() SBE_NOEXCEPT
    {
        return static_cast<char>(32);
    }

    static SBE_CONSTEXPR char baseCurrencyMaxValue() SBE_NOEXCEPT
    {
        return static_cast<char>(126);
    }

    static SBE_CONSTEXPR std::size_t baseCurrencyEncodingLength() SBE_NOEXCEPT
    {
        return 32;
    }

    static SBE_CONSTEXPR std::uint64_t baseCurrencyLength() SBE_NOEXCEPT
    {
        return 32;
    }

    SBE_NODISCARD const char *baseCurrency() const SBE_NOEXCEPT
    {
        return m_buffer + m_offset + 209;
    }

    SBE_NODISCARD char *baseCurrency() SBE_NOEXCEPT
    {
        return m_buffer + m_offset + 209;
    }

    SBE_NODISCARD char baseCurrency(const std::uint64_t index) const
    {
        if (index >= 32)
        {
            throw std::runtime_error("index out of range for baseCurrency [E104]");
        }

        char val;
        std::memcpy(&val, m_buffer + m_offset + 209 + (index * 1), sizeof(char));
        return (val);
    }

    ReferenceData &baseCurrency(const std::uint64_t index, const char value)
    {
        if (index >= 32)
        {
            throw std::runtime_error("index out of range for baseCurrency [E105]");
        }

        char val = (value);
        std::memcpy(m_buffer + m_offset + 209 + (index * 1), &val, sizeof(char));
        return *this;
    }

    std::uint64_t getBaseCurrency(char *const dst, const std::uint64_t length) const
    {
        if (length > 32)
        {
            throw std::runtime_error("length too large for getBaseCurrency [E106]");
        }

        std::memcpy(dst, m_buffer + m_offset + 209, sizeof(char) * static_cast<std::size_t>(length));
        return length;
    }

    ReferenceData &putBaseCurrency(const char *const src) SBE_NOEXCEPT
    {
        std::memcpy(m_buffer + m_offset + 209, src, sizeof(char) * 32);
        return *this;
    }

    SBE_NODISCARD std::string getBaseCurrencyAsString() const
    {
        const char *buffer = m_buffer + m_offset + 209;
        std::size_t length = 0;

        for (; length < 32 && *(buffer + length) != '\0'; ++length);
        std::string result(buffer, length);

        return result;
    }

    std::string getBaseCurrencyAsJsonEscapedString()
    {
        std::ostringstream oss;
        std::string s = getBaseCurrencyAsString();

        for (const auto c : s)
        {
            switch (c)
            {
                case '"': oss << "\\\""; break;
                case '\\': oss << "\\\\"; break;
                case '\b': oss << "\\b"; break;
                case '\f': oss << "\\f"; break;
                case '\n': oss << "\\n"; break;
                case '\r': oss << "\\r"; break;
                case '\t': oss << "\\t"; break;

                default:
                    if ('\x00' <= c && c <= '\x1f')
                    {
                        oss << "\\u" << std::hex << std::setw(4)
                            << std::setfill('0') << (int)(c);
                    }
                    else
                    {
                        oss << c;
                    }
            }
        }

        return oss.str();
    }

    #if __cplusplus >= 201703L
    SBE_NODISCARD std::string_view getBaseCurrencyAsStringView() const SBE_NOEXCEPT
    {
        const char *buffer = m_buffer + m_offset + 209;
        std::size_t length = 0;

        for (; length < 32 && *(buffer + length) != '\0'; ++length);
        std::string_view result(buffer, length);

        return result;
    }
    #endif

    #if __cplusplus >= 201703L
    ReferenceData &putBaseCurrency(const std::string_view str)
    {
        const std::size_t srcLength = str.length();
        if (srcLength > 32)
        {
            throw std::runtime_error("string too large for putBaseCurrency [E106]");
        }

        std::memcpy(m_buffer + m_offset + 209, str.data(), srcLength);
        for (std::size_t start = srcLength; start < 32; ++start)
        {
            m_buffer[m_offset + 209 + start] = 0;
        }

        return *this;
    }
    #else
    ReferenceData &putBaseCurrency(const std::string &str)
    {
        const std::size_t srcLength = str.length();
        if (srcLength > 32)
        {
            throw std::runtime_error("string too large for putBaseCurrency [E106]");
        }

        std::memcpy(m_buffer + m_offset + 209, str.c_str(), srcLength);
        for (std::size_t start = srcLength; start < 32; ++start)
        {
            m_buffer[m_offset + 209 + start] = 0;
        }

        return *this;
    }
    #endif

    SBE_NODISCARD static const char *quoteCurrencyMetaAttribute(const MetaAttribute metaAttribute) SBE_NOEXCEPT
    {
        switch (metaAttribute)
        {
            case MetaAttribute::PRESENCE: return "required";
            default: return "";
        }
    }

    static SBE_CONSTEXPR std::uint16_t quoteCurrencyId() SBE_NOEXCEPT
    {
        return 6;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::uint64_t quoteCurrencySinceVersion() SBE_NOEXCEPT
    {
        return 0;
    }

    SBE_NODISCARD bool quoteCurrencyInActingVersion() SBE_NOEXCEPT
    {
        return true;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::size_t quoteCurrencyEncodingOffset() SBE_NOEXCEPT
    {
        return 241;
    }

    static SBE_CONSTEXPR char quoteCurrencyNullValue() SBE_NOEXCEPT
    {
        return static_cast<char>(0);
    }

    static SBE_CONSTEXPR char quoteCurrencyMinValue() SBE_NOEXCEPT
    {
        return static_cast<char>(32);
    }

    static SBE_CONSTEXPR char quoteCurrencyMaxValue() SBE_NOEXCEPT
    {
        return static_cast<char>(126);
    }

    static SBE_CONSTEXPR std::size_t quoteCurrencyEncodingLength() SBE_NOEXCEPT
    {
        return 32;
    }

    static SBE_CONSTEXPR std::uint64_t quoteCurrencyLength() SBE_NOEXCEPT
    {
        return 32;
    }

    SBE_NODISCARD const char *quoteCurrency() const SBE_NOEXCEPT
    {
        return m_buffer + m_offset + 241;
    }

    SBE_NODISCARD char *quoteCurrency() SBE_NOEXCEPT
    {
        return m_buffer + m_offset + 241;
    }

    SBE_NODISCARD char quoteCurrency(const std::uint64_t index) const
    {
        if (index >= 32)
        {
            throw std::runtime_error("index out of range for quoteCurrency [E104]");
        }

        char val;
        std::memcpy(&val, m_buffer + m_offset + 241 + (index * 1), sizeof(char));
        return (val);
    }

    ReferenceData &quoteCurrency(const std::uint64_t index, const char value)
    {
        if (index >= 32)
        {
            throw std::runtime_error("index out of range for quoteCurrency [E105]");
        }

        char val = (value);
        std::memcpy(m_buffer + m_offset + 241 + (index * 1), &val, sizeof(char));
        return *this;
    }

    std::uint64_t getQuoteCurrency(char *const dst, const std::uint64_t length) const
    {
        if (length > 32)
        {
            throw std::runtime_error("length too large for getQuoteCurrency [E106]");
        }

        std::memcpy(dst, m_buffer + m_offset + 241, sizeof(char) * static_cast<std::size_t>(length));
        return length;
    }

    ReferenceData &putQuoteCurrency(const char *const src) SBE_NOEXCEPT
    {
        std::memcpy(m_buffer + m_offset + 241, src, sizeof(char) * 32);
        return *this;
    }

    SBE_NODISCARD std::string getQuoteCurrencyAsString() const
    {
        const char *buffer = m_buffer + m_offset + 241;
        std::size_t length = 0;

        for (; length < 32 && *(buffer + length) != '\0'; ++length);
        std::string result(buffer, length);

        return result;
    }

    std::string getQuoteCurrencyAsJsonEscapedString()
    {
        std::ostringstream oss;
        std::string s = getQuoteCurrencyAsString();

        for (const auto c : s)
        {
            switch (c)
            {
                case '"': oss << "\\\""; break;
                case '\\': oss << "\\\\"; break;
                case '\b': oss << "\\b"; break;
                case '\f': oss << "\\f"; break;
                case '\n': oss << "\\n"; break;
                case '\r': oss << "\\r"; break;
                case '\t': oss << "\\t"; break;

                default:
                    if ('\x00' <= c && c <= '\x1f')
                    {
                        oss << "\\u" << std::hex << std::setw(4)
                            << std::setfill('0') << (int)(c);
                    }
                    else
                    {
                        oss << c;
                    }
            }
        }

        return oss.str();
    }

    #if __cplusplus >= 201703L
    SBE_NODISCARD std::string_view getQuoteCurrencyAsStringView() const SBE_NOEXCEPT
    {
        const char *buffer = m_buffer + m_offset + 241;
        std::size_t length = 0;

        for (; length < 32 && *(buffer + length) != '\0'; ++length);
        std::string_view result(buffer, length);

        return result;
    }
    #endif

    #if __cplusplus >= 201703L
    ReferenceData &putQuoteCurrency(const std::string_view str)
    {
        const std::size_t srcLength = str.length();
        if (srcLength > 32)
        {
            throw std::runtime_error("string too large for putQuoteCurrency [E106]");
        }

        std::memcpy(m_buffer + m_offset + 241, str.data(), srcLength);
        for (std::size_t start = srcLength; start < 32; ++start)
        {
            m_buffer[m_offset + 241 + start] = 0;
        }

        return *this;
    }
    #else
    ReferenceData &putQuoteCurrency(const std::string &str)
    {
        const std::size_t srcLength = str.length();
        if (srcLength > 32)
        {
            throw std::runtime_error("string too large for putQuoteCurrency [E106]");
        }

        std::memcpy(m_buffer + m_offset + 241, str.c_str(), srcLength);
        for (std::size_t start = srcLength; start < 32; ++start)
        {
            m_buffer[m_offset + 241 + start] = 0;
        }

        return *this;
    }
    #endif

    SBE_NODISCARD static const char *marginCurrencyMetaAttribute(const MetaAttribute metaAttribute) SBE_NOEXCEPT
    {
        switch (metaAttribute)
        {
            case MetaAttribute::PRESENCE: return "required";
            default: return "";
        }
    }

    static SBE_CONSTEXPR std::uint16_t marginCurrencyId() SBE_NOEXCEPT
    {
        return 7;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::uint64_t marginCurrencySinceVersion() SBE_NOEXCEPT
    {
        return 0;
    }

    SBE_NODISCARD bool marginCurrencyInActingVersion() SBE_NOEXCEPT
    {
        return true;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::size_t marginCurrencyEncodingOffset() SBE_NOEXCEPT
    {
        return 273;
    }

    static SBE_CONSTEXPR char marginCurrencyNullValue() SBE_NOEXCEPT
    {
        return static_cast<char>(0);
    }

    static SBE_CONSTEXPR char marginCurrencyMinValue() SBE_NOEXCEPT
    {
        return static_cast<char>(32);
    }

    static SBE_CONSTEXPR char marginCurrencyMaxValue() SBE_NOEXCEPT
    {
        return static_cast<char>(126);
    }

    static SBE_CONSTEXPR std::size_t marginCurrencyEncodingLength() SBE_NOEXCEPT
    {
        return 32;
    }

    static SBE_CONSTEXPR std::uint64_t marginCurrencyLength() SBE_NOEXCEPT
    {
        return 32;
    }

    SBE_NODISCARD const char *marginCurrency() const SBE_NOEXCEPT
    {
        return m_buffer + m_offset + 273;
    }

    SBE_NODISCARD char *marginCurrency() SBE_NOEXCEPT
    {
        return m_buffer + m_offset + 273;
    }

    SBE_NODISCARD char marginCurrency(const std::uint64_t index) const
    {
        if (index >= 32)
        {
            throw std::runtime_error("index out of range for marginCurrency [E104]");
        }

        char val;
        std::memcpy(&val, m_buffer + m_offset + 273 + (index * 1), sizeof(char));
        return (val);
    }

    ReferenceData &marginCurrency(const std::uint64_t index, const char value)
    {
        if (index >= 32)
        {
            throw std::runtime_error("index out of range for marginCurrency [E105]");
        }

        char val = (value);
        std::memcpy(m_buffer + m_offset + 273 + (index * 1), &val, sizeof(char));
        return *this;
    }

    std::uint64_t getMarginCurrency(char *const dst, const std::uint64_t length) const
    {
        if (length > 32)
        {
            throw std::runtime_error("length too large for getMarginCurrency [E106]");
        }

        std::memcpy(dst, m_buffer + m_offset + 273, sizeof(char) * static_cast<std::size_t>(length));
        return length;
    }

    ReferenceData &putMarginCurrency(const char *const src) SBE_NOEXCEPT
    {
        std::memcpy(m_buffer + m_offset + 273, src, sizeof(char) * 32);
        return *this;
    }

    SBE_NODISCARD std::string getMarginCurrencyAsString() const
    {
        const char *buffer = m_buffer + m_offset + 273;
        std::size_t length = 0;

        for (; length < 32 && *(buffer + length) != '\0'; ++length);
        std::string result(buffer, length);

        return result;
    }

    std::string getMarginCurrencyAsJsonEscapedString()
    {
        std::ostringstream oss;
        std::string s = getMarginCurrencyAsString();

        for (const auto c : s)
        {
            switch (c)
            {
                case '"': oss << "\\\""; break;
                case '\\': oss << "\\\\"; break;
                case '\b': oss << "\\b"; break;
                case '\f': oss << "\\f"; break;
                case '\n': oss << "\\n"; break;
                case '\r': oss << "\\r"; break;
                case '\t': oss << "\\t"; break;

                default:
                    if ('\x00' <= c && c <= '\x1f')
                    {
                        oss << "\\u" << std::hex << std::setw(4)
                            << std::setfill('0') << (int)(c);
                    }
                    else
                    {
                        oss << c;
                    }
            }
        }

        return oss.str();
    }

    #if __cplusplus >= 201703L
    SBE_NODISCARD std::string_view getMarginCurrencyAsStringView() const SBE_NOEXCEPT
    {
        const char *buffer = m_buffer + m_offset + 273;
        std::size_t length = 0;

        for (; length < 32 && *(buffer + length) != '\0'; ++length);
        std::string_view result(buffer, length);

        return result;
    }
    #endif

    #if __cplusplus >= 201703L
    ReferenceData &putMarginCurrency(const std::string_view str)
    {
        const std::size_t srcLength = str.length();
        if (srcLength > 32)
        {
            throw std::runtime_error("string too large for putMarginCurrency [E106]");
        }

        std::memcpy(m_buffer + m_offset + 273, str.data(), srcLength);
        for (std::size_t start = srcLength; start < 32; ++start)
        {
            m_buffer[m_offset + 273 + start] = 0;
        }

        return *this;
    }
    #else
    ReferenceData &putMarginCurrency(const std::string &str)
    {
        const std::size_t srcLength = str.length();
        if (srcLength > 32)
        {
            throw std::runtime_error("string too large for putMarginCurrency [E106]");
        }

        std::memcpy(m_buffer + m_offset + 273, str.c_str(), srcLength);
        for (std::size_t start = srcLength; start < 32; ++start)
        {
            m_buffer[m_offset + 273 + start] = 0;
        }

        return *this;
    }
    #endif

    SBE_NODISCARD static const char *commissionCurrencyMetaAttribute(const MetaAttribute metaAttribute) SBE_NOEXCEPT
    {
        switch (metaAttribute)
        {
            case MetaAttribute::PRESENCE: return "required";
            default: return "";
        }
    }

    static SBE_CONSTEXPR std::uint16_t commissionCurrencyId() SBE_NOEXCEPT
    {
        return 8;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::uint64_t commissionCurrencySinceVersion() SBE_NOEXCEPT
    {
        return 0;
    }

    SBE_NODISCARD bool commissionCurrencyInActingVersion() SBE_NOEXCEPT
    {
        return true;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::size_t commissionCurrencyEncodingOffset() SBE_NOEXCEPT
    {
        return 305;
    }

    static SBE_CONSTEXPR char commissionCurrencyNullValue() SBE_NOEXCEPT
    {
        return static_cast<char>(0);
    }

    static SBE_CONSTEXPR char commissionCurrencyMinValue() SBE_NOEXCEPT
    {
        return static_cast<char>(32);
    }

    static SBE_CONSTEXPR char commissionCurrencyMaxValue() SBE_NOEXCEPT
    {
        return static_cast<char>(126);
    }

    static SBE_CONSTEXPR std::size_t commissionCurrencyEncodingLength() SBE_NOEXCEPT
    {
        return 32;
    }

    static SBE_CONSTEXPR std::uint64_t commissionCurrencyLength() SBE_NOEXCEPT
    {
        return 32;
    }

    SBE_NODISCARD const char *commissionCurrency() const SBE_NOEXCEPT
    {
        return m_buffer + m_offset + 305;
    }

    SBE_NODISCARD char *commissionCurrency() SBE_NOEXCEPT
    {
        return m_buffer + m_offset + 305;
    }

    SBE_NODISCARD char commissionCurrency(const std::uint64_t index) const
    {
        if (index >= 32)
        {
            throw std::runtime_error("index out of range for commissionCurrency [E104]");
        }

        char val;
        std::memcpy(&val, m_buffer + m_offset + 305 + (index * 1), sizeof(char));
        return (val);
    }

    ReferenceData &commissionCurrency(const std::uint64_t index, const char value)
    {
        if (index >= 32)
        {
            throw std::runtime_error("index out of range for commissionCurrency [E105]");
        }

        char val = (value);
        std::memcpy(m_buffer + m_offset + 305 + (index * 1), &val, sizeof(char));
        return *this;
    }

    std::uint64_t getCommissionCurrency(char *const dst, const std::uint64_t length) const
    {
        if (length > 32)
        {
            throw std::runtime_error("length too large for getCommissionCurrency [E106]");
        }

        std::memcpy(dst, m_buffer + m_offset + 305, sizeof(char) * static_cast<std::size_t>(length));
        return length;
    }

    ReferenceData &putCommissionCurrency(const char *const src) SBE_NOEXCEPT
    {
        std::memcpy(m_buffer + m_offset + 305, src, sizeof(char) * 32);
        return *this;
    }

    SBE_NODISCARD std::string getCommissionCurrencyAsString() const
    {
        const char *buffer = m_buffer + m_offset + 305;
        std::size_t length = 0;

        for (; length < 32 && *(buffer + length) != '\0'; ++length);
        std::string result(buffer, length);

        return result;
    }

    std::string getCommissionCurrencyAsJsonEscapedString()
    {
        std::ostringstream oss;
        std::string s = getCommissionCurrencyAsString();

        for (const auto c : s)
        {
            switch (c)
            {
                case '"': oss << "\\\""; break;
                case '\\': oss << "\\\\"; break;
                case '\b': oss << "\\b"; break;
                case '\f': oss << "\\f"; break;
                case '\n': oss << "\\n"; break;
                case '\r': oss << "\\r"; break;
                case '\t': oss << "\\t"; break;

                default:
                    if ('\x00' <= c && c <= '\x1f')
                    {
                        oss << "\\u" << std::hex << std::setw(4)
                            << std::setfill('0') << (int)(c);
                    }
                    else
                    {
                        oss << c;
                    }
            }
        }

        return oss.str();
    }

    #if __cplusplus >= 201703L
    SBE_NODISCARD std::string_view getCommissionCurrencyAsStringView() const SBE_NOEXCEPT
    {
        const char *buffer = m_buffer + m_offset + 305;
        std::size_t length = 0;

        for (; length < 32 && *(buffer + length) != '\0'; ++length);
        std::string_view result(buffer, length);

        return result;
    }
    #endif

    #if __cplusplus >= 201703L
    ReferenceData &putCommissionCurrency(const std::string_view str)
    {
        const std::size_t srcLength = str.length();
        if (srcLength > 32)
        {
            throw std::runtime_error("string too large for putCommissionCurrency [E106]");
        }

        std::memcpy(m_buffer + m_offset + 305, str.data(), srcLength);
        for (std::size_t start = srcLength; start < 32; ++start)
        {
            m_buffer[m_offset + 305 + start] = 0;
        }

        return *this;
    }
    #else
    ReferenceData &putCommissionCurrency(const std::string &str)
    {
        const std::size_t srcLength = str.length();
        if (srcLength > 32)
        {
            throw std::runtime_error("string too large for putCommissionCurrency [E106]");
        }

        std::memcpy(m_buffer + m_offset + 305, str.c_str(), srcLength);
        for (std::size_t start = srcLength; start < 32; ++start)
        {
            m_buffer[m_offset + 305 + start] = 0;
        }

        return *this;
    }
    #endif

    SBE_NODISCARD static const char *tickSizeMetaAttribute(const MetaAttribute metaAttribute) SBE_NOEXCEPT
    {
        switch (metaAttribute)
        {
            case MetaAttribute::PRESENCE: return "required";
            default: return "";
        }
    }

    static SBE_CONSTEXPR std::uint16_t tickSizeId() SBE_NOEXCEPT
    {
        return 9;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::uint64_t tickSizeSinceVersion() SBE_NOEXCEPT
    {
        return 0;
    }

    SBE_NODISCARD bool tickSizeInActingVersion() SBE_NOEXCEPT
    {
        return true;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::size_t tickSizeEncodingOffset() SBE_NOEXCEPT
    {
        return 337;
    }

    static SBE_CONSTEXPR double tickSizeNullValue() SBE_NOEXCEPT
    {
        return SBE_DOUBLE_NAN;
    }

    static SBE_CONSTEXPR double tickSizeMinValue() SBE_NOEXCEPT
    {
        return 4.9E-324;
    }

    static SBE_CONSTEXPR double tickSizeMaxValue() SBE_NOEXCEPT
    {
        return 1.7976931348623157E308;
    }

    static SBE_CONSTEXPR std::size_t tickSizeEncodingLength() SBE_NOEXCEPT
    {
        return 8;
    }

    SBE_NODISCARD double tickSize() const SBE_NOEXCEPT
    {
        union sbe_double_as_uint_u val;
        std::memcpy(&val, m_buffer + m_offset + 337, sizeof(double));
        val.uint_value = SBE_LITTLE_ENDIAN_ENCODE_64(val.uint_value);
        return val.fp_value;
    }

    ReferenceData &tickSize(const double value) SBE_NOEXCEPT
    {
        union sbe_double_as_uint_u val;
        val.fp_value = value;
        val.uint_value = SBE_LITTLE_ENDIAN_ENCODE_64(val.uint_value);
        std::memcpy(m_buffer + m_offset + 337, &val, sizeof(double));
        return *this;
    }

    SBE_NODISCARD static const char *multiplierMetaAttribute(const MetaAttribute metaAttribute) SBE_NOEXCEPT
    {
        switch (metaAttribute)
        {
            case MetaAttribute::PRESENCE: return "required";
            default: return "";
        }
    }

    static SBE_CONSTEXPR std::uint16_t multiplierId() SBE_NOEXCEPT
    {
        return 10;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::uint64_t multiplierSinceVersion() SBE_NOEXCEPT
    {
        return 0;
    }

    SBE_NODISCARD bool multiplierInActingVersion() SBE_NOEXCEPT
    {
        return true;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::size_t multiplierEncodingOffset() SBE_NOEXCEPT
    {
        return 345;
    }

    static SBE_CONSTEXPR double multiplierNullValue() SBE_NOEXCEPT
    {
        return SBE_DOUBLE_NAN;
    }

    static SBE_CONSTEXPR double multiplierMinValue() SBE_NOEXCEPT
    {
        return 4.9E-324;
    }

    static SBE_CONSTEXPR double multiplierMaxValue() SBE_NOEXCEPT
    {
        return 1.7976931348623157E308;
    }

    static SBE_CONSTEXPR std::size_t multiplierEncodingLength() SBE_NOEXCEPT
    {
        return 8;
    }

    SBE_NODISCARD double multiplier() const SBE_NOEXCEPT
    {
        union sbe_double_as_uint_u val;
        std::memcpy(&val, m_buffer + m_offset + 345, sizeof(double));
        val.uint_value = SBE_LITTLE_ENDIAN_ENCODE_64(val.uint_value);
        return val.fp_value;
    }

    ReferenceData &multiplier(const double value) SBE_NOEXCEPT
    {
        union sbe_double_as_uint_u val;
        val.fp_value = value;
        val.uint_value = SBE_LITTLE_ENDIAN_ENCODE_64(val.uint_value);
        std::memcpy(m_buffer + m_offset + 345, &val, sizeof(double));
        return *this;
    }

    SBE_NODISCARD static const char *minNotionalMetaAttribute(const MetaAttribute metaAttribute) SBE_NOEXCEPT
    {
        switch (metaAttribute)
        {
            case MetaAttribute::PRESENCE: return "required";
            default: return "";
        }
    }

    static SBE_CONSTEXPR std::uint16_t minNotionalId() SBE_NOEXCEPT
    {
        return 11;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::uint64_t minNotionalSinceVersion() SBE_NOEXCEPT
    {
        return 0;
    }

    SBE_NODISCARD bool minNotionalInActingVersion() SBE_NOEXCEPT
    {
        return true;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::size_t minNotionalEncodingOffset() SBE_NOEXCEPT
    {
        return 353;
    }

    static SBE_CONSTEXPR double minNotionalNullValue() SBE_NOEXCEPT
    {
        return SBE_DOUBLE_NAN;
    }

    static SBE_CONSTEXPR double minNotionalMinValue() SBE_NOEXCEPT
    {
        return 4.9E-324;
    }

    static SBE_CONSTEXPR double minNotionalMaxValue() SBE_NOEXCEPT
    {
        return 1.7976931348623157E308;
    }

    static SBE_CONSTEXPR std::size_t minNotionalEncodingLength() SBE_NOEXCEPT
    {
        return 8;
    }

    SBE_NODISCARD double minNotional() const SBE_NOEXCEPT
    {
        union sbe_double_as_uint_u val;
        std::memcpy(&val, m_buffer + m_offset + 353, sizeof(double));
        val.uint_value = SBE_LITTLE_ENDIAN_ENCODE_64(val.uint_value);
        return val.fp_value;
    }

    ReferenceData &minNotional(const double value) SBE_NOEXCEPT
    {
        union sbe_double_as_uint_u val;
        val.fp_value = value;
        val.uint_value = SBE_LITTLE_ENDIAN_ENCODE_64(val.uint_value);
        std::memcpy(m_buffer + m_offset + 353, &val, sizeof(double));
        return *this;
    }

    SBE_NODISCARD static const char *minTradeVolMetaAttribute(const MetaAttribute metaAttribute) SBE_NOEXCEPT
    {
        switch (metaAttribute)
        {
            case MetaAttribute::PRESENCE: return "required";
            default: return "";
        }
    }

    static SBE_CONSTEXPR std::uint16_t minTradeVolId() SBE_NOEXCEPT
    {
        return 12;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::uint64_t minTradeVolSinceVersion() SBE_NOEXCEPT
    {
        return 0;
    }

    SBE_NODISCARD bool minTradeVolInActingVersion() SBE_NOEXCEPT
    {
        return true;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::size_t minTradeVolEncodingOffset() SBE_NOEXCEPT
    {
        return 361;
    }

    static SBE_CONSTEXPR double minTradeVolNullValue() SBE_NOEXCEPT
    {
        return SBE_DOUBLE_NAN;
    }

    static SBE_CONSTEXPR double minTradeVolMinValue() SBE_NOEXCEPT
    {
        return 4.9E-324;
    }

    static SBE_CONSTEXPR double minTradeVolMaxValue() SBE_NOEXCEPT
    {
        return 1.7976931348623157E308;
    }

    static SBE_CONSTEXPR std::size_t minTradeVolEncodingLength() SBE_NOEXCEPT
    {
        return 8;
    }

    SBE_NODISCARD double minTradeVol() const SBE_NOEXCEPT
    {
        union sbe_double_as_uint_u val;
        std::memcpy(&val, m_buffer + m_offset + 361, sizeof(double));
        val.uint_value = SBE_LITTLE_ENDIAN_ENCODE_64(val.uint_value);
        return val.fp_value;
    }

    ReferenceData &minTradeVol(const double value) SBE_NOEXCEPT
    {
        union sbe_double_as_uint_u val;
        val.fp_value = value;
        val.uint_value = SBE_LITTLE_ENDIAN_ENCODE_64(val.uint_value);
        std::memcpy(m_buffer + m_offset + 361, &val, sizeof(double));
        return *this;
    }

    SBE_NODISCARD static const char *maxTradeVolMetaAttribute(const MetaAttribute metaAttribute) SBE_NOEXCEPT
    {
        switch (metaAttribute)
        {
            case MetaAttribute::PRESENCE: return "required";
            default: return "";
        }
    }

    static SBE_CONSTEXPR std::uint16_t maxTradeVolId() SBE_NOEXCEPT
    {
        return 13;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::uint64_t maxTradeVolSinceVersion() SBE_NOEXCEPT
    {
        return 0;
    }

    SBE_NODISCARD bool maxTradeVolInActingVersion() SBE_NOEXCEPT
    {
        return true;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::size_t maxTradeVolEncodingOffset() SBE_NOEXCEPT
    {
        return 369;
    }

    static SBE_CONSTEXPR double maxTradeVolNullValue() SBE_NOEXCEPT
    {
        return SBE_DOUBLE_NAN;
    }

    static SBE_CONSTEXPR double maxTradeVolMinValue() SBE_NOEXCEPT
    {
        return 4.9E-324;
    }

    static SBE_CONSTEXPR double maxTradeVolMaxValue() SBE_NOEXCEPT
    {
        return 1.7976931348623157E308;
    }

    static SBE_CONSTEXPR std::size_t maxTradeVolEncodingLength() SBE_NOEXCEPT
    {
        return 8;
    }

    SBE_NODISCARD double maxTradeVol() const SBE_NOEXCEPT
    {
        union sbe_double_as_uint_u val;
        std::memcpy(&val, m_buffer + m_offset + 369, sizeof(double));
        val.uint_value = SBE_LITTLE_ENDIAN_ENCODE_64(val.uint_value);
        return val.fp_value;
    }

    ReferenceData &maxTradeVol(const double value) SBE_NOEXCEPT
    {
        union sbe_double_as_uint_u val;
        val.fp_value = value;
        val.uint_value = SBE_LITTLE_ENDIAN_ENCODE_64(val.uint_value);
        std::memcpy(m_buffer + m_offset + 369, &val, sizeof(double));
        return *this;
    }

    SBE_NODISCARD static const char *tradeVolStepSizeMetaAttribute(const MetaAttribute metaAttribute) SBE_NOEXCEPT
    {
        switch (metaAttribute)
        {
            case MetaAttribute::PRESENCE: return "required";
            default: return "";
        }
    }

    static SBE_CONSTEXPR std::uint16_t tradeVolStepSizeId() SBE_NOEXCEPT
    {
        return 14;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::uint64_t tradeVolStepSizeSinceVersion() SBE_NOEXCEPT
    {
        return 0;
    }

    SBE_NODISCARD bool tradeVolStepSizeInActingVersion() SBE_NOEXCEPT
    {
        return true;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::size_t tradeVolStepSizeEncodingOffset() SBE_NOEXCEPT
    {
        return 377;
    }

    static SBE_CONSTEXPR double tradeVolStepSizeNullValue() SBE_NOEXCEPT
    {
        return SBE_DOUBLE_NAN;
    }

    static SBE_CONSTEXPR double tradeVolStepSizeMinValue() SBE_NOEXCEPT
    {
        return 4.9E-324;
    }

    static SBE_CONSTEXPR double tradeVolStepSizeMaxValue() SBE_NOEXCEPT
    {
        return 1.7976931348623157E308;
    }

    static SBE_CONSTEXPR std::size_t tradeVolStepSizeEncodingLength() SBE_NOEXCEPT
    {
        return 8;
    }

    SBE_NODISCARD double tradeVolStepSize() const SBE_NOEXCEPT
    {
        union sbe_double_as_uint_u val;
        std::memcpy(&val, m_buffer + m_offset + 377, sizeof(double));
        val.uint_value = SBE_LITTLE_ENDIAN_ENCODE_64(val.uint_value);
        return val.fp_value;
    }

    ReferenceData &tradeVolStepSize(const double value) SBE_NOEXCEPT
    {
        union sbe_double_as_uint_u val;
        val.fp_value = value;
        val.uint_value = SBE_LITTLE_ENDIAN_ENCODE_64(val.uint_value);
        std::memcpy(m_buffer + m_offset + 377, &val, sizeof(double));
        return *this;
    }

    SBE_NODISCARD static const char *optionTypeMetaAttribute(const MetaAttribute metaAttribute) SBE_NOEXCEPT
    {
        switch (metaAttribute)
        {
            case MetaAttribute::PRESENCE: return "required";
            default: return "";
        }
    }

    static SBE_CONSTEXPR std::uint16_t optionTypeId() SBE_NOEXCEPT
    {
        return 15;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::uint64_t optionTypeSinceVersion() SBE_NOEXCEPT
    {
        return 0;
    }

    SBE_NODISCARD bool optionTypeInActingVersion() SBE_NOEXCEPT
    {
        return true;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::size_t optionTypeEncodingOffset() SBE_NOEXCEPT
    {
        return 385;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::size_t optionTypeEncodingLength() SBE_NOEXCEPT
    {
        return 1;
    }

    SBE_NODISCARD std::uint8_t optionTypeRaw() const SBE_NOEXCEPT
    {
        std::uint8_t val;
        std::memcpy(&val, m_buffer + m_offset + 385, sizeof(std::uint8_t));
        return (val);
    }

    SBE_NODISCARD OptionType::Value optionType() const
    {
        std::uint8_t val;
        std::memcpy(&val, m_buffer + m_offset + 385, sizeof(std::uint8_t));
        return OptionType::get((val));
    }

    ReferenceData &optionType(const OptionType::Value value) SBE_NOEXCEPT
    {
        std::uint8_t val = (value);
        std::memcpy(m_buffer + m_offset + 385, &val, sizeof(std::uint8_t));
        return *this;
    }

    SBE_NODISCARD static const char *strikeCurrencyMetaAttribute(const MetaAttribute metaAttribute) SBE_NOEXCEPT
    {
        switch (metaAttribute)
        {
            case MetaAttribute::PRESENCE: return "required";
            default: return "";
        }
    }

    static SBE_CONSTEXPR std::uint16_t strikeCurrencyId() SBE_NOEXCEPT
    {
        return 16;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::uint64_t strikeCurrencySinceVersion() SBE_NOEXCEPT
    {
        return 0;
    }

    SBE_NODISCARD bool strikeCurrencyInActingVersion() SBE_NOEXCEPT
    {
        return true;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::size_t strikeCurrencyEncodingOffset() SBE_NOEXCEPT
    {
        return 386;
    }

    static SBE_CONSTEXPR char strikeCurrencyNullValue() SBE_NOEXCEPT
    {
        return static_cast<char>(0);
    }

    static SBE_CONSTEXPR char strikeCurrencyMinValue() SBE_NOEXCEPT
    {
        return static_cast<char>(32);
    }

    static SBE_CONSTEXPR char strikeCurrencyMaxValue() SBE_NOEXCEPT
    {
        return static_cast<char>(126);
    }

    static SBE_CONSTEXPR std::size_t strikeCurrencyEncodingLength() SBE_NOEXCEPT
    {
        return 32;
    }

    static SBE_CONSTEXPR std::uint64_t strikeCurrencyLength() SBE_NOEXCEPT
    {
        return 32;
    }

    SBE_NODISCARD const char *strikeCurrency() const SBE_NOEXCEPT
    {
        return m_buffer + m_offset + 386;
    }

    SBE_NODISCARD char *strikeCurrency() SBE_NOEXCEPT
    {
        return m_buffer + m_offset + 386;
    }

    SBE_NODISCARD char strikeCurrency(const std::uint64_t index) const
    {
        if (index >= 32)
        {
            throw std::runtime_error("index out of range for strikeCurrency [E104]");
        }

        char val;
        std::memcpy(&val, m_buffer + m_offset + 386 + (index * 1), sizeof(char));
        return (val);
    }

    ReferenceData &strikeCurrency(const std::uint64_t index, const char value)
    {
        if (index >= 32)
        {
            throw std::runtime_error("index out of range for strikeCurrency [E105]");
        }

        char val = (value);
        std::memcpy(m_buffer + m_offset + 386 + (index * 1), &val, sizeof(char));
        return *this;
    }

    std::uint64_t getStrikeCurrency(char *const dst, const std::uint64_t length) const
    {
        if (length > 32)
        {
            throw std::runtime_error("length too large for getStrikeCurrency [E106]");
        }

        std::memcpy(dst, m_buffer + m_offset + 386, sizeof(char) * static_cast<std::size_t>(length));
        return length;
    }

    ReferenceData &putStrikeCurrency(const char *const src) SBE_NOEXCEPT
    {
        std::memcpy(m_buffer + m_offset + 386, src, sizeof(char) * 32);
        return *this;
    }

    SBE_NODISCARD std::string getStrikeCurrencyAsString() const
    {
        const char *buffer = m_buffer + m_offset + 386;
        std::size_t length = 0;

        for (; length < 32 && *(buffer + length) != '\0'; ++length);
        std::string result(buffer, length);

        return result;
    }

    std::string getStrikeCurrencyAsJsonEscapedString()
    {
        std::ostringstream oss;
        std::string s = getStrikeCurrencyAsString();

        for (const auto c : s)
        {
            switch (c)
            {
                case '"': oss << "\\\""; break;
                case '\\': oss << "\\\\"; break;
                case '\b': oss << "\\b"; break;
                case '\f': oss << "\\f"; break;
                case '\n': oss << "\\n"; break;
                case '\r': oss << "\\r"; break;
                case '\t': oss << "\\t"; break;

                default:
                    if ('\x00' <= c && c <= '\x1f')
                    {
                        oss << "\\u" << std::hex << std::setw(4)
                            << std::setfill('0') << (int)(c);
                    }
                    else
                    {
                        oss << c;
                    }
            }
        }

        return oss.str();
    }

    #if __cplusplus >= 201703L
    SBE_NODISCARD std::string_view getStrikeCurrencyAsStringView() const SBE_NOEXCEPT
    {
        const char *buffer = m_buffer + m_offset + 386;
        std::size_t length = 0;

        for (; length < 32 && *(buffer + length) != '\0'; ++length);
        std::string_view result(buffer, length);

        return result;
    }
    #endif

    #if __cplusplus >= 201703L
    ReferenceData &putStrikeCurrency(const std::string_view str)
    {
        const std::size_t srcLength = str.length();
        if (srcLength > 32)
        {
            throw std::runtime_error("string too large for putStrikeCurrency [E106]");
        }

        std::memcpy(m_buffer + m_offset + 386, str.data(), srcLength);
        for (std::size_t start = srcLength; start < 32; ++start)
        {
            m_buffer[m_offset + 386 + start] = 0;
        }

        return *this;
    }
    #else
    ReferenceData &putStrikeCurrency(const std::string &str)
    {
        const std::size_t srcLength = str.length();
        if (srcLength > 32)
        {
            throw std::runtime_error("string too large for putStrikeCurrency [E106]");
        }

        std::memcpy(m_buffer + m_offset + 386, str.c_str(), srcLength);
        for (std::size_t start = srcLength; start < 32; ++start)
        {
            m_buffer[m_offset + 386 + start] = 0;
        }

        return *this;
    }
    #endif

    SBE_NODISCARD static const char *strikePriceMetaAttribute(const MetaAttribute metaAttribute) SBE_NOEXCEPT
    {
        switch (metaAttribute)
        {
            case MetaAttribute::PRESENCE: return "required";
            default: return "";
        }
    }

    static SBE_CONSTEXPR std::uint16_t strikePriceId() SBE_NOEXCEPT
    {
        return 17;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::uint64_t strikePriceSinceVersion() SBE_NOEXCEPT
    {
        return 0;
    }

    SBE_NODISCARD bool strikePriceInActingVersion() SBE_NOEXCEPT
    {
        return true;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::size_t strikePriceEncodingOffset() SBE_NOEXCEPT
    {
        return 418;
    }

    static SBE_CONSTEXPR double strikePriceNullValue() SBE_NOEXCEPT
    {
        return SBE_DOUBLE_NAN;
    }

    static SBE_CONSTEXPR double strikePriceMinValue() SBE_NOEXCEPT
    {
        return 4.9E-324;
    }

    static SBE_CONSTEXPR double strikePriceMaxValue() SBE_NOEXCEPT
    {
        return 1.7976931348623157E308;
    }

    static SBE_CONSTEXPR std::size_t strikePriceEncodingLength() SBE_NOEXCEPT
    {
        return 8;
    }

    SBE_NODISCARD double strikePrice() const SBE_NOEXCEPT
    {
        union sbe_double_as_uint_u val;
        std::memcpy(&val, m_buffer + m_offset + 418, sizeof(double));
        val.uint_value = SBE_LITTLE_ENDIAN_ENCODE_64(val.uint_value);
        return val.fp_value;
    }

    ReferenceData &strikePrice(const double value) SBE_NOEXCEPT
    {
        union sbe_double_as_uint_u val;
        val.fp_value = value;
        val.uint_value = SBE_LITTLE_ENDIAN_ENCODE_64(val.uint_value);
        std::memcpy(m_buffer + m_offset + 418, &val, sizeof(double));
        return *this;
    }

    SBE_NODISCARD static const char *underlyingMetaAttribute(const MetaAttribute metaAttribute) SBE_NOEXCEPT
    {
        switch (metaAttribute)
        {
            case MetaAttribute::PRESENCE: return "required";
            default: return "";
        }
    }

    static SBE_CONSTEXPR std::uint16_t underlyingId() SBE_NOEXCEPT
    {
        return 18;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::uint64_t underlyingSinceVersion() SBE_NOEXCEPT
    {
        return 0;
    }

    SBE_NODISCARD bool underlyingInActingVersion() SBE_NOEXCEPT
    {
        return true;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::size_t underlyingEncodingOffset() SBE_NOEXCEPT
    {
        return 426;
    }

    static SBE_CONSTEXPR char underlyingNullValue() SBE_NOEXCEPT
    {
        return static_cast<char>(0);
    }

    static SBE_CONSTEXPR char underlyingMinValue() SBE_NOEXCEPT
    {
        return static_cast<char>(32);
    }

    static SBE_CONSTEXPR char underlyingMaxValue() SBE_NOEXCEPT
    {
        return static_cast<char>(126);
    }

    static SBE_CONSTEXPR std::size_t underlyingEncodingLength() SBE_NOEXCEPT
    {
        return 48;
    }

    static SBE_CONSTEXPR std::uint64_t underlyingLength() SBE_NOEXCEPT
    {
        return 48;
    }

    SBE_NODISCARD const char *underlying() const SBE_NOEXCEPT
    {
        return m_buffer + m_offset + 426;
    }

    SBE_NODISCARD char *underlying() SBE_NOEXCEPT
    {
        return m_buffer + m_offset + 426;
    }

    SBE_NODISCARD char underlying(const std::uint64_t index) const
    {
        if (index >= 48)
        {
            throw std::runtime_error("index out of range for underlying [E104]");
        }

        char val;
        std::memcpy(&val, m_buffer + m_offset + 426 + (index * 1), sizeof(char));
        return (val);
    }

    ReferenceData &underlying(const std::uint64_t index, const char value)
    {
        if (index >= 48)
        {
            throw std::runtime_error("index out of range for underlying [E105]");
        }

        char val = (value);
        std::memcpy(m_buffer + m_offset + 426 + (index * 1), &val, sizeof(char));
        return *this;
    }

    std::uint64_t getUnderlying(char *const dst, const std::uint64_t length) const
    {
        if (length > 48)
        {
            throw std::runtime_error("length too large for getUnderlying [E106]");
        }

        std::memcpy(dst, m_buffer + m_offset + 426, sizeof(char) * static_cast<std::size_t>(length));
        return length;
    }

    ReferenceData &putUnderlying(const char *const src) SBE_NOEXCEPT
    {
        std::memcpy(m_buffer + m_offset + 426, src, sizeof(char) * 48);
        return *this;
    }

    SBE_NODISCARD std::string getUnderlyingAsString() const
    {
        const char *buffer = m_buffer + m_offset + 426;
        std::size_t length = 0;

        for (; length < 48 && *(buffer + length) != '\0'; ++length);
        std::string result(buffer, length);

        return result;
    }

    std::string getUnderlyingAsJsonEscapedString()
    {
        std::ostringstream oss;
        std::string s = getUnderlyingAsString();

        for (const auto c : s)
        {
            switch (c)
            {
                case '"': oss << "\\\""; break;
                case '\\': oss << "\\\\"; break;
                case '\b': oss << "\\b"; break;
                case '\f': oss << "\\f"; break;
                case '\n': oss << "\\n"; break;
                case '\r': oss << "\\r"; break;
                case '\t': oss << "\\t"; break;

                default:
                    if ('\x00' <= c && c <= '\x1f')
                    {
                        oss << "\\u" << std::hex << std::setw(4)
                            << std::setfill('0') << (int)(c);
                    }
                    else
                    {
                        oss << c;
                    }
            }
        }

        return oss.str();
    }

    #if __cplusplus >= 201703L
    SBE_NODISCARD std::string_view getUnderlyingAsStringView() const SBE_NOEXCEPT
    {
        const char *buffer = m_buffer + m_offset + 426;
        std::size_t length = 0;

        for (; length < 48 && *(buffer + length) != '\0'; ++length);
        std::string_view result(buffer, length);

        return result;
    }
    #endif

    #if __cplusplus >= 201703L
    ReferenceData &putUnderlying(const std::string_view str)
    {
        const std::size_t srcLength = str.length();
        if (srcLength > 48)
        {
            throw std::runtime_error("string too large for putUnderlying [E106]");
        }

        std::memcpy(m_buffer + m_offset + 426, str.data(), srcLength);
        for (std::size_t start = srcLength; start < 48; ++start)
        {
            m_buffer[m_offset + 426 + start] = 0;
        }

        return *this;
    }
    #else
    ReferenceData &putUnderlying(const std::string &str)
    {
        const std::size_t srcLength = str.length();
        if (srcLength > 48)
        {
            throw std::runtime_error("string too large for putUnderlying [E106]");
        }

        std::memcpy(m_buffer + m_offset + 426, str.c_str(), srcLength);
        for (std::size_t start = srcLength; start < 48; ++start)
        {
            m_buffer[m_offset + 426 + start] = 0;
        }

        return *this;
    }
    #endif

    SBE_NODISCARD static const char *timeZoneMetaAttribute(const MetaAttribute metaAttribute) SBE_NOEXCEPT
    {
        switch (metaAttribute)
        {
            case MetaAttribute::PRESENCE: return "required";
            default: return "";
        }
    }

    static SBE_CONSTEXPR std::uint16_t timeZoneId() SBE_NOEXCEPT
    {
        return 19;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::uint64_t timeZoneSinceVersion() SBE_NOEXCEPT
    {
        return 0;
    }

    SBE_NODISCARD bool timeZoneInActingVersion() SBE_NOEXCEPT
    {
        return true;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::size_t timeZoneEncodingOffset() SBE_NOEXCEPT
    {
        return 474;
    }

    static SBE_CONSTEXPR char timeZoneNullValue() SBE_NOEXCEPT
    {
        return static_cast<char>(0);
    }

    static SBE_CONSTEXPR char timeZoneMinValue() SBE_NOEXCEPT
    {
        return static_cast<char>(32);
    }

    static SBE_CONSTEXPR char timeZoneMaxValue() SBE_NOEXCEPT
    {
        return static_cast<char>(126);
    }

    static SBE_CONSTEXPR std::size_t timeZoneEncodingLength() SBE_NOEXCEPT
    {
        return 32;
    }

    static SBE_CONSTEXPR std::uint64_t timeZoneLength() SBE_NOEXCEPT
    {
        return 32;
    }

    SBE_NODISCARD const char *timeZone() const SBE_NOEXCEPT
    {
        return m_buffer + m_offset + 474;
    }

    SBE_NODISCARD char *timeZone() SBE_NOEXCEPT
    {
        return m_buffer + m_offset + 474;
    }

    SBE_NODISCARD char timeZone(const std::uint64_t index) const
    {
        if (index >= 32)
        {
            throw std::runtime_error("index out of range for timeZone [E104]");
        }

        char val;
        std::memcpy(&val, m_buffer + m_offset + 474 + (index * 1), sizeof(char));
        return (val);
    }

    ReferenceData &timeZone(const std::uint64_t index, const char value)
    {
        if (index >= 32)
        {
            throw std::runtime_error("index out of range for timeZone [E105]");
        }

        char val = (value);
        std::memcpy(m_buffer + m_offset + 474 + (index * 1), &val, sizeof(char));
        return *this;
    }

    std::uint64_t getTimeZone(char *const dst, const std::uint64_t length) const
    {
        if (length > 32)
        {
            throw std::runtime_error("length too large for getTimeZone [E106]");
        }

        std::memcpy(dst, m_buffer + m_offset + 474, sizeof(char) * static_cast<std::size_t>(length));
        return length;
    }

    ReferenceData &putTimeZone(const char *const src) SBE_NOEXCEPT
    {
        std::memcpy(m_buffer + m_offset + 474, src, sizeof(char) * 32);
        return *this;
    }

    SBE_NODISCARD std::string getTimeZoneAsString() const
    {
        const char *buffer = m_buffer + m_offset + 474;
        std::size_t length = 0;

        for (; length < 32 && *(buffer + length) != '\0'; ++length);
        std::string result(buffer, length);

        return result;
    }

    std::string getTimeZoneAsJsonEscapedString()
    {
        std::ostringstream oss;
        std::string s = getTimeZoneAsString();

        for (const auto c : s)
        {
            switch (c)
            {
                case '"': oss << "\\\""; break;
                case '\\': oss << "\\\\"; break;
                case '\b': oss << "\\b"; break;
                case '\f': oss << "\\f"; break;
                case '\n': oss << "\\n"; break;
                case '\r': oss << "\\r"; break;
                case '\t': oss << "\\t"; break;

                default:
                    if ('\x00' <= c && c <= '\x1f')
                    {
                        oss << "\\u" << std::hex << std::setw(4)
                            << std::setfill('0') << (int)(c);
                    }
                    else
                    {
                        oss << c;
                    }
            }
        }

        return oss.str();
    }

    #if __cplusplus >= 201703L
    SBE_NODISCARD std::string_view getTimeZoneAsStringView() const SBE_NOEXCEPT
    {
        const char *buffer = m_buffer + m_offset + 474;
        std::size_t length = 0;

        for (; length < 32 && *(buffer + length) != '\0'; ++length);
        std::string_view result(buffer, length);

        return result;
    }
    #endif

    #if __cplusplus >= 201703L
    ReferenceData &putTimeZone(const std::string_view str)
    {
        const std::size_t srcLength = str.length();
        if (srcLength > 32)
        {
            throw std::runtime_error("string too large for putTimeZone [E106]");
        }

        std::memcpy(m_buffer + m_offset + 474, str.data(), srcLength);
        for (std::size_t start = srcLength; start < 32; ++start)
        {
            m_buffer[m_offset + 474 + start] = 0;
        }

        return *this;
    }
    #else
    ReferenceData &putTimeZone(const std::string &str)
    {
        const std::size_t srcLength = str.length();
        if (srcLength > 32)
        {
            throw std::runtime_error("string too large for putTimeZone [E106]");
        }

        std::memcpy(m_buffer + m_offset + 474, str.c_str(), srcLength);
        for (std::size_t start = srcLength; start < 32; ++start)
        {
            m_buffer[m_offset + 474 + start] = 0;
        }

        return *this;
    }
    #endif

    SBE_NODISCARD static const char *issueDateMetaAttribute(const MetaAttribute metaAttribute) SBE_NOEXCEPT
    {
        switch (metaAttribute)
        {
            case MetaAttribute::PRESENCE: return "required";
            default: return "";
        }
    }

    static SBE_CONSTEXPR std::uint16_t issueDateId() SBE_NOEXCEPT
    {
        return 20;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::uint64_t issueDateSinceVersion() SBE_NOEXCEPT
    {
        return 0;
    }

    SBE_NODISCARD bool issueDateInActingVersion() SBE_NOEXCEPT
    {
        return true;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::size_t issueDateEncodingOffset() SBE_NOEXCEPT
    {
        return 506;
    }

    static SBE_CONSTEXPR std::int32_t issueDateNullValue() SBE_NOEXCEPT
    {
        return SBE_NULLVALUE_INT32;
    }

    static SBE_CONSTEXPR std::int32_t issueDateMinValue() SBE_NOEXCEPT
    {
        return INT32_C(-2147483647);
    }

    static SBE_CONSTEXPR std::int32_t issueDateMaxValue() SBE_NOEXCEPT
    {
        return INT32_C(2147483647);
    }

    static SBE_CONSTEXPR std::size_t issueDateEncodingLength() SBE_NOEXCEPT
    {
        return 4;
    }

    SBE_NODISCARD std::int32_t issueDate() const SBE_NOEXCEPT
    {
        std::int32_t val;
        std::memcpy(&val, m_buffer + m_offset + 506, sizeof(std::int32_t));
        return SBE_LITTLE_ENDIAN_ENCODE_32(val);
    }

    ReferenceData &issueDate(const std::int32_t value) SBE_NOEXCEPT
    {
        std::int32_t val = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        std::memcpy(m_buffer + m_offset + 506, &val, sizeof(std::int32_t));
        return *this;
    }

    SBE_NODISCARD static const char *settlementDateMetaAttribute(const MetaAttribute metaAttribute) SBE_NOEXCEPT
    {
        switch (metaAttribute)
        {
            case MetaAttribute::PRESENCE: return "required";
            default: return "";
        }
    }

    static SBE_CONSTEXPR std::uint16_t settlementDateId() SBE_NOEXCEPT
    {
        return 21;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::uint64_t settlementDateSinceVersion() SBE_NOEXCEPT
    {
        return 0;
    }

    SBE_NODISCARD bool settlementDateInActingVersion() SBE_NOEXCEPT
    {
        return true;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::size_t settlementDateEncodingOffset() SBE_NOEXCEPT
    {
        return 510;
    }

    static SBE_CONSTEXPR std::int32_t settlementDateNullValue() SBE_NOEXCEPT
    {
        return SBE_NULLVALUE_INT32;
    }

    static SBE_CONSTEXPR std::int32_t settlementDateMinValue() SBE_NOEXCEPT
    {
        return INT32_C(-2147483647);
    }

    static SBE_CONSTEXPR std::int32_t settlementDateMaxValue() SBE_NOEXCEPT
    {
        return INT32_C(2147483647);
    }

    static SBE_CONSTEXPR std::size_t settlementDateEncodingLength() SBE_NOEXCEPT
    {
        return 4;
    }

    SBE_NODISCARD std::int32_t settlementDate() const SBE_NOEXCEPT
    {
        std::int32_t val;
        std::memcpy(&val, m_buffer + m_offset + 510, sizeof(std::int32_t));
        return SBE_LITTLE_ENDIAN_ENCODE_32(val);
    }

    ReferenceData &settlementDate(const std::int32_t value) SBE_NOEXCEPT
    {
        std::int32_t val = SBE_LITTLE_ENDIAN_ENCODE_32(value);
        std::memcpy(m_buffer + m_offset + 510, &val, sizeof(std::int32_t));
        return *this;
    }

    SBE_NODISCARD static const char *expiryDateTimeMetaAttribute(const MetaAttribute metaAttribute) SBE_NOEXCEPT
    {
        switch (metaAttribute)
        {
            case MetaAttribute::PRESENCE: return "required";
            default: return "";
        }
    }

    static SBE_CONSTEXPR std::uint16_t expiryDateTimeId() SBE_NOEXCEPT
    {
        return 22;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::uint64_t expiryDateTimeSinceVersion() SBE_NOEXCEPT
    {
        return 0;
    }

    SBE_NODISCARD bool expiryDateTimeInActingVersion() SBE_NOEXCEPT
    {
        return true;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::size_t expiryDateTimeEncodingOffset() SBE_NOEXCEPT
    {
        return 514;
    }

    static SBE_CONSTEXPR std::int64_t expiryDateTimeNullValue() SBE_NOEXCEPT
    {
        return SBE_NULLVALUE_INT64;
    }

    static SBE_CONSTEXPR std::int64_t expiryDateTimeMinValue() SBE_NOEXCEPT
    {
        return INT64_C(-9223372036854775807);
    }

    static SBE_CONSTEXPR std::int64_t expiryDateTimeMaxValue() SBE_NOEXCEPT
    {
        return INT64_C(9223372036854775807);
    }

    static SBE_CONSTEXPR std::size_t expiryDateTimeEncodingLength() SBE_NOEXCEPT
    {
        return 8;
    }

    SBE_NODISCARD std::int64_t expiryDateTime() const SBE_NOEXCEPT
    {
        std::int64_t val;
        std::memcpy(&val, m_buffer + m_offset + 514, sizeof(std::int64_t));
        return SBE_LITTLE_ENDIAN_ENCODE_64(val);
    }

    ReferenceData &expiryDateTime(const std::int64_t value) SBE_NOEXCEPT
    {
        std::int64_t val = SBE_LITTLE_ENDIAN_ENCODE_64(value);
        std::memcpy(m_buffer + m_offset + 514, &val, sizeof(std::int64_t));
        return *this;
    }

    SBE_NODISCARD static const char *expiryDateTimeUTCMetaAttribute(const MetaAttribute metaAttribute) SBE_NOEXCEPT
    {
        switch (metaAttribute)
        {
            case MetaAttribute::PRESENCE: return "required";
            default: return "";
        }
    }

    static SBE_CONSTEXPR std::uint16_t expiryDateTimeUTCId() SBE_NOEXCEPT
    {
        return 23;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::uint64_t expiryDateTimeUTCSinceVersion() SBE_NOEXCEPT
    {
        return 0;
    }

    SBE_NODISCARD bool expiryDateTimeUTCInActingVersion() SBE_NOEXCEPT
    {
        return true;
    }

    SBE_NODISCARD static SBE_CONSTEXPR std::size_t expiryDateTimeUTCEncodingOffset() SBE_NOEXCEPT
    {
        return 522;
    }

    static SBE_CONSTEXPR std::int64_t expiryDateTimeUTCNullValue() SBE_NOEXCEPT
    {
        return SBE_NULLVALUE_INT64;
    }

    static SBE_CONSTEXPR std::int64_t expiryDateTimeUTCMinValue() SBE_NOEXCEPT
    {
        return INT64_C(-9223372036854775807);
    }

    static SBE_CONSTEXPR std::int64_t expiryDateTimeUTCMaxValue() SBE_NOEXCEPT
    {
        return INT64_C(9223372036854775807);
    }

    static SBE_CONSTEXPR std::size_t expiryDateTimeUTCEncodingLength() SBE_NOEXCEPT
    {
        return 8;
    }

    SBE_NODISCARD std::int64_t expiryDateTimeUTC() const SBE_NOEXCEPT
    {
        std::int64_t val;
        std::memcpy(&val, m_buffer + m_offset + 522, sizeof(std::int64_t));
        return SBE_LITTLE_ENDIAN_ENCODE_64(val);
    }

    ReferenceData &expiryDateTimeUTC(const std::int64_t value) SBE_NOEXCEPT
    {
        std::int64_t val = SBE_LITTLE_ENDIAN_ENCODE_64(value);
        std::memcpy(m_buffer + m_offset + 522, &val, sizeof(std::int64_t));
        return *this;
    }

template<typename CharT, typename Traits>
friend std::basic_ostream<CharT, Traits> & operator << (
    std::basic_ostream<CharT, Traits> &builder, const ReferenceData &_writer)
{
    ReferenceData writer(
        _writer.m_buffer,
        _writer.m_offset,
        _writer.m_bufferLength,
        _writer.m_actingBlockLength,
        _writer.m_actingVersion);

    builder << '{';
    builder << R"("Name": "ReferenceData", )";
    builder << R"("sbeTemplateId": )";
    builder << writer.sbeTemplateId();
    builder << ", ";

    builder << R"("exchange": )";
    builder << '"' <<
        writer.getExchangeAsJsonEscapedString().c_str() << '"';

    builder << ", ";
    builder << R"("symbol": )";
    builder << '"' <<
        writer.getSymbolAsJsonEscapedString().c_str() << '"';

    builder << ", ";
    builder << R"("description": )";
    builder << '"' <<
        writer.getDescriptionAsJsonEscapedString().c_str() << '"';

    builder << ", ";
    builder << R"("security_type": )";
    builder << '"' << writer.security_type() << '"';

    builder << ", ";
    builder << R"("baseCurrency": )";
    builder << '"' <<
        writer.getBaseCurrencyAsJsonEscapedString().c_str() << '"';

    builder << ", ";
    builder << R"("quoteCurrency": )";
    builder << '"' <<
        writer.getQuoteCurrencyAsJsonEscapedString().c_str() << '"';

    builder << ", ";
    builder << R"("marginCurrency": )";
    builder << '"' <<
        writer.getMarginCurrencyAsJsonEscapedString().c_str() << '"';

    builder << ", ";
    builder << R"("commissionCurrency": )";
    builder << '"' <<
        writer.getCommissionCurrencyAsJsonEscapedString().c_str() << '"';

    builder << ", ";
    builder << R"("tickSize": )";
    builder << +writer.tickSize();

    builder << ", ";
    builder << R"("multiplier": )";
    builder << +writer.multiplier();

    builder << ", ";
    builder << R"("minNotional": )";
    builder << +writer.minNotional();

    builder << ", ";
    builder << R"("minTradeVol": )";
    builder << +writer.minTradeVol();

    builder << ", ";
    builder << R"("maxTradeVol": )";
    builder << +writer.maxTradeVol();

    builder << ", ";
    builder << R"("tradeVolStepSize": )";
    builder << +writer.tradeVolStepSize();

    builder << ", ";
    builder << R"("optionType": )";
    builder << '"' << writer.optionType() << '"';

    builder << ", ";
    builder << R"("strikeCurrency": )";
    builder << '"' <<
        writer.getStrikeCurrencyAsJsonEscapedString().c_str() << '"';

    builder << ", ";
    builder << R"("strikePrice": )";
    builder << +writer.strikePrice();

    builder << ", ";
    builder << R"("underlying": )";
    builder << '"' <<
        writer.getUnderlyingAsJsonEscapedString().c_str() << '"';

    builder << ", ";
    builder << R"("timeZone": )";
    builder << '"' <<
        writer.getTimeZoneAsJsonEscapedString().c_str() << '"';

    builder << ", ";
    builder << R"("issueDate": )";
    builder << +writer.issueDate();

    builder << ", ";
    builder << R"("settlementDate": )";
    builder << +writer.settlementDate();

    builder << ", ";
    builder << R"("expiryDateTime": )";
    builder << +writer.expiryDateTime();

    builder << ", ";
    builder << R"("expiryDateTimeUTC": )";
    builder << +writer.expiryDateTimeUTC();

    builder << '}';

    return builder;
}

void skip()
{
}

SBE_NODISCARD static SBE_CONSTEXPR bool isConstLength() SBE_NOEXCEPT
{
    return true;
}

SBE_NODISCARD static std::size_t computeLength()
{
#if defined(__GNUG__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits"
#endif
    std::size_t length = sbeBlockLength();

    return length;
#if defined(__GNUG__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif
}
};
}
#endif
