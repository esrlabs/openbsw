// Copyright 2025 Accenture.

/**
 * \ingroup doip
 */
#pragma once

#include "doip/server/IDoIpServerTransportLayerCallback.h"

#include <gmock/gmock.h>

namespace doip
{
/**
 * Callback interface for a DoIP transport layer.
 */
class DoIpServerTransportLayerCallbackMock : public IDoIpServerTransportLayerCallback
{
public:
    MOCK_CONST_METHOD2(
        getSocketGroupId, uint8_t(uint8_t serverSocketId, ::ip::IPEndpoint const& remoteEndpoint));
    MOCK_CONST_METHOD1(getMaxConnectionCount, uint8_t(uint8_t socketGroupId));
    MOCK_METHOD2(
        filterConnection, bool(uint8_t socketGroupId, ::ip::IPEndpoint const& remoteEndpoint));
    MOCK_METHOD7(
        checkRoutingActivation,
        RoutingActivationCheckResult(
            uint16_t sourceAddress,
            uint8_t activationType,
            uint8_t socketGroupId,
            ::ip::IPEndpoint const& localEndpoint,
            ::ip::IPEndpoint const& remoteEndpoint,
            ::estd::optional<uint32_t> const oemField,
            bool isResuming));
    MOCK_METHOD5(
        routingActive,
        void(
            uint16_t sourceAddress,
            uint16_t internalSourceAddress,
            ::ip::IPEndpoint const& localEndpoint,
            ::ip::IPEndpoint const& remoteEndpoint,
            DoIpTcpConnection::ConnectionType type));
    MOCK_METHOD1(routingInactive, void(uint16_t internalSourceAddress));
    MOCK_METHOD1(connectionClosed, void(uint16_t sourceAddress));
};

} // namespace doip
