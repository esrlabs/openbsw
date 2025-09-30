// Copyright 2025 Accenture.

/**
 * \ingroup doip
 */
#pragma once

#include "doip/server/IDoIpServerMessageHandler.h"

#include <gmock/gmock.h>

namespace doip
{
/**
 * Interface for a DoIp server message handler.
 */
class DoIpServerMessageHandlerMock : public IDoIpServerMessageHandler
{
public:
    MOCK_METHOD1(connectionOpened, void(IDoIpServerConnection& connection));
    MOCK_METHOD0(routingActive, void());
    MOCK_METHOD1(headerReceived, bool(DoIpHeader const& header));
    MOCK_METHOD0(connectionClosed, void());
};

} // namespace doip
