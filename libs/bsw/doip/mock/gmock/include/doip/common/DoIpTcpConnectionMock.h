// Copyright 2025 Accenture.

/**
 * \ingroup doip
 */
#pragma once

#include "doip/common/IDoIpTcpConnection.h"

#include <gmock/gmock.h>

namespace doip
{
/**
 * Mock for a DoIp TCP connection.
 */
class DoIpTcpConnectionMock : public IDoIpTcpConnection
{
public:
    MOCK_METHOD1(init, void(IDoIpConnectionHandler&));
    MOCK_CONST_METHOD0(getLocalEndpoint, ::ip::IPEndpoint());
    MOCK_CONST_METHOD0(getRemoteEndpoint, ::ip::IPEndpoint());
    MOCK_METHOD2(receivePayload, bool(::estd::slice<uint8_t>, PayloadReceivedCallbackType));
    MOCK_METHOD1(endReceiveMessage, void(IDoIpConnection::PayloadDiscardedCallbackType));
    MOCK_METHOD1(sendMessage, bool(IDoIpSendJob&));
    MOCK_METHOD0(close, void());
    MOCK_METHOD1(setCloseMode, void(CloseMode));
    MOCK_METHOD1(detach, void(DetachCallbackType));
    MOCK_METHOD0(suspendSending, void());
    MOCK_METHOD0(resumeSending, void());
};

} // namespace doip
