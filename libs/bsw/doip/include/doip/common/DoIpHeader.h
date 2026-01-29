// Copyright 2025 Accenture.

/**
 * \ingroup doip
 */
#pragma once

#include "doip/common/DoIpConstants.h"

#include <etl/unaligned_type.h>

#include <cstdint>

namespace doip
{
struct DoIpHeader
{
    uint8_t protocolVersion;
    uint8_t invertedProtocolVersion;
    ::etl::be_uint16_t payloadType;
    ::etl::be_uint32_t payloadLength;
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
