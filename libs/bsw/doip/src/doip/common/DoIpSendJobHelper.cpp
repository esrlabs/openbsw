// Copyright 2025 Accenture.

#include "doip/common/DoIpSendJobHelper.h"

#include "doip/common/DoIpHeader.h"

#include <util/estd/assert.h>

#include <estd/big_endian.h>

namespace doip
{
::estd::slice<uint8_t const> DoIpSendJobHelper::prepareHeaderBuffer(
    ::estd::slice<uint8_t> const buffer,
    uint8_t const protocolVersion,
    uint16_t const payloadType,
    uint32_t const payloadLength)
{
    estd_assert(buffer.size() >= DoIpConstants::DOIP_HEADER_LENGTH);

    auto headerBuffer              = buffer.reinterpret_as<DoIpHeader>();
    DoIpHeader& header             = headerBuffer[0];
    header.protocolVersion         = protocolVersion;
    header.invertedProtocolVersion = static_cast<uint8_t>(~protocolVersion);
    header.payloadType             = payloadType;
    header.payloadLength           = payloadLength;

    return buffer.subslice(DoIpConstants::DOIP_HEADER_LENGTH);
}

} // namespace doip
