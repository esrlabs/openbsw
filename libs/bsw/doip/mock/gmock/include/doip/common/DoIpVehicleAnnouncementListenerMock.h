// Copyright 2025 Accenture.

/**
 * \ingroup doip
 */
#pragma once

#include "doip/common/IDoIpVehicleAnnouncementListener.h"

#include <gmock/gmock.h>

namespace doip
{
/**
 * DoIP vehicle announcement listener mock.
 */
class DoIpVehicleAnnouncementListenerMock : public IDoIpVehicleAnnouncementListener
{
public:
    MOCK_METHOD3(
        vehicleAnnouncementReceived,
        void(uint16_t, ::ip::IPEndpoint const&, ::ip::IPEndpoint const&));
};

} // namespace doip
