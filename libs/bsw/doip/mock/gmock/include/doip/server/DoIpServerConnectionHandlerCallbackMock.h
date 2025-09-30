// Copyright 2025 Accenture.

/**
 * \ingroup doip
 */
#pragma once

#include "doip/server/IDoIpServerConnectionHandlerCallback.h"

#include <gmock/gmock.h>

namespace doip
{
/**
 * Callback interface for a DoIp server connection handler.
 */
class DoIpServerConnectionHandlerCallbackMock : public IDoIpServerConnectionHandlerCallback
{
public:
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
    MOCK_METHOD1(routingActive, void(DoIpServerConnectionHandler& handler));
    MOCK_METHOD1(handleRoutingActivationRequest, void(DoIpServerConnectionHandler& handler));
    MOCK_METHOD2(
        aliveCheckResponseReceived, void(DoIpServerConnectionHandler& handler, bool isAlive));
    MOCK_METHOD1(connectionClosed, void(DoIpServerConnectionHandler& handler));
};

} // namespace doip
