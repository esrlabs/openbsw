// Copyright 2025 Accenture.

#pragma once

#include "ip/NetworkInterfaceConfig.h"

#include <gmock/gmock.h>

namespace ip
{
struct NetworkInterfaceConfigRegistryMock : public ::ip::NetworkInterfaceConfigRegistry
{
    NetworkInterfaceConfigRegistryMock() : ::ip::NetworkInterfaceConfigRegistry({}, {}) {}

    MOCK_METHOD(::ip::NetworkInterfaceConfig, getConfig, (uint8_t), (const, override));
};

} // namespace ip
