// Copyright 2025 Accenture.

/**
 * \ingroup doip
 */
#pragma once

#include "doip/server/IDoIpServerSocketHandlerListener.h"

#include <gmock/gmock.h>

namespace doip
{
/**
 * Mock class for DoIP socket handler listener.
 */
class DoIpServerSocketHandlerListenerMock : public IDoIpServerSocketHandlerListener
{
public:
    MOCK_METHOD3(
        serverSocketBound,
        void(
            uint8_t serverSocketId, ip::IPEndpoint const&, DoIpTcpConnection::ConnectionType type));
    MOCK_METHOD2(filterConnection, bool(uint8_t serverSocketId, ip::IPEndpoint const&));
    MOCK_METHOD3(
        connectionAccepted,
        void(
            uint8_t serverSocketId,
            ::tcp::AbstractSocket&,
            DoIpTcpConnection::ConnectionType type));
};

} // namespace doip
