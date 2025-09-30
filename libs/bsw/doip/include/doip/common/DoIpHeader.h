// Copyright 2025 Accenture.

/**
 * \ingroup doip
 */
#pragma once

#include "doip/common/DoIpConstants.h"

#include <estd/big_endian.h>

#include <cstdint>

namespace doip
{
struct DoIpHeader
{
    uint8_t protocolVersion;
    uint8_t invertedProtocolVersion;
    ::estd::be_uint16_t payloadType;
    ::estd::be_uint32_t payloadLength;
};

inline bool checkProtocolVersion(DoIpHeader const& header, uint8_t const expectedVersion)
{
    if (static_cast<uint8_t>(~header.protocolVersion) != header.invertedProtocolVersion)
    {
        return false;
    }
    return header.protocolVersion == expectedVersion;
}

static_assert(sizeof(DoIpHeader) == DoIpConstants::DOIP_HEADER_LENGTH, "");

} // namespace doip
