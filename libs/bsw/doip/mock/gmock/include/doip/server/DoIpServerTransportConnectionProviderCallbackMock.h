// Copyright 2025 Accenture.

/**
 * \ingroup doip
 */
#pragma once

#include "doip/server/IDoIpServerTransportConnectionProviderCallback.h"

#include <gmock/gmock.h>

namespace doip
{
class DoIpServerTransportConnection;

/**
 * Interface for creation of new transport connections.
 */
class DoIpServerTransportConnectionProviderCallbackMock
: public IDoIpServerTransportConnectionProviderCallback
{
public:
    MOCK_CONST_METHOD2(
        getSocketGroupId, uint8_t(uint8_t serverSocketId, ::ip::IPEndpoint const& remoteEndpoint));
    MOCK_METHOD2(
        filterConnection, bool(uint8_t socketGroupId, ::ip::IPEndpoint const& remoteEndpoint));
    MOCK_METHOD1(connectionAccepted, void(DoIpServerTransportConnection& connection));
};

} // namespace doip
