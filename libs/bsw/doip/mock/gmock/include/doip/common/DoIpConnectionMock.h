// Copyright 2025 Accenture.

/**
 * \ingroup doip
 */
#pragma once

#include "doip/common/IDoIpConnection.h"

#include <gmock/gmock.h>

namespace doip
{
/**
 * Mock for a DoIp connection.
 */
class DoIpConnectionMock : public IDoIpConnection
{
public:
    MOCK_METHOD1(init, void(IDoIpConnectionHandler&));
    MOCK_CONST_METHOD0(getLocalEndpoint, ::ip::IPEndpoint());
    MOCK_CONST_METHOD0(getRemoteEndpoint, ::ip::IPEndpoint());
    MOCK_METHOD2(receivePayload, bool(::estd::slice<uint8_t>, PayloadReceivedCallbackType));
    MOCK_METHOD0(endReceiveMessage, void());
    MOCK_METHOD1(sendMessage, bool(DoIpSendJob&));
    MOCK_METHOD0(close, void());
};

} // namespace doip
