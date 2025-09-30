// Copyright 2025 Accenture.

/**
 * \ingroup doip
 */
#pragma once

#include "doip/common/IDoIpSendJob.h"

#include <gmock/gmock.h>

namespace doip
{
/**
 * Mock for a DoIp send job.
 */
class DoIpSendJobMock : public IDoIpSendJob
{
public:
    MOCK_CONST_METHOD0(getSendBufferCount, uint8_t());
    MOCK_CONST_METHOD0(getTotalLength, uint16_t());
    MOCK_CONST_METHOD0(getDestinationEndpoint, ::ip::IPEndpoint const*());
    MOCK_METHOD1(release, void(bool success));
    MOCK_METHOD2(
        getSendBuffer,
        ::estd::slice<uint8_t const>(::estd::slice<uint8_t> staticBuffer, uint8_t index));
};

} // namespace doip
