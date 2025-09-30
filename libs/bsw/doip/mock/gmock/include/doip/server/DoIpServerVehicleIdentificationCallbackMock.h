// Copyright 2025 Accenture.

/**
 * \ingroup doip
 */
#pragma once

#include "doip/server/IDoIpServerVehicleIdentificationCallback.h"

#include <gmock/gmock.h>

namespace doip
{
/**
 * Mock class for vehicle identification
 */
class DoIpServerVehicleIdentificationCallbackMock : public IDoIpServerVehicleIdentificationCallback
{
public:
    MOCK_METHOD1(getVin, void(VinType));
    MOCK_METHOD1(getGid, void(GidType));
    MOCK_METHOD1(getEid, void(EidType));
    MOCK_METHOD0(getPowerMode, DoIpConstants::DiagnosticPowerMode());
    MOCK_METHOD0(onVirReceived, void());
    MOCK_CONST_METHOD1(getOemMessageHandler, IDoIpUdpOemMessageHandler*(uint16_t));
};

} // namespace doip
