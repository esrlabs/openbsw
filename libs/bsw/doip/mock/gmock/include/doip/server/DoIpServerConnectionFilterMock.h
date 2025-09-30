// Copyright 2025 Accenture.

/**
 * \ingroup doip
 */
#pragma once

#include "doip/server/IDoIpServerConnectionFilter.h"

#include <gmock/gmock.h>

namespace doip
{
/**
 * Filter interface for a DoIP server connection.
 */
class DoIpServerConnectionFilterMock : public IDoIpServerConnectionFilter
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
};

} // namespace doip
