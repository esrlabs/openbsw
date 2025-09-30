// Copyright 2025 Accenture.

#pragma once

#include "doip/server/IDoIpServerVehicleAnnouncementParameterProvider.h"

#include <gmock/gmock.h>

namespace doip
{
/**
 * Mock class for vehicle identification parameters provider
 */
class DoIpServerVehicleAnnouncementParameterProviderMock
: public IDoIpServerVehicleAnnouncementParameterProvider
{
public:
    MOCK_CONST_METHOD0(getAnnounceWait, uint16_t(void));
    MOCK_CONST_METHOD0(getAnnounceInterval, uint16_t(void));
};

} // namespace doip
