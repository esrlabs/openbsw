// Copyright 2025 Accenture.

/**
 * \ingroup doip
 */
#pragma once

#include <estd/slice.h>

namespace doip
{
/**
 * Simple helper class for common send job functionality.
 */
class DoIpSendJobHelper
{
public:
    /**
     * Prepare a send buffer for a DoIP Header.
     * \param staticBuffer reference to static buffer that will be used
     * \param protocolVersion DoIP protocol version to use
     * \param payloadType payload type
     * \param payloadLength payloadLength
     */
    static ::estd::slice<uint8_t const> prepareHeaderBuffer(
        ::estd::slice<uint8_t> staticBuffer,
        uint8_t protocolVersion,
        uint16_t payloadType,
        uint32_t payloadLength);
};

} // namespace doip
