// Copyright 2025 Accenture.

/**
 * \ingroup doip
 */
#pragma once

#include "doip/common/IDoIpConnectionHandler.h"

#include <gmock/gmock.h>

namespace doip
{
/**
 * Interface for a DoIp connection handler.
 */
class DoIpConnectionHandlerMock : public IDoIpConnectionHandler
{
public:
    MOCK_METHOD1(connectionClosed, void(bool));
    MOCK_METHOD1(
        headerReceived, IDoIpConnectionHandler::HeaderReceivedContinuation(DoIpHeader const&));
};

} // namespace doip
