// Copyright 2025 Accenture.

/**
 * \ingroup doip
 */
#pragma once

#include "doip/server/IDoIpServerEntityStatusCallback.h"

#include <gmock/gmock.h>

namespace doip
{
/**
 * Interface for retrieving information about entity status.
 */
class DoIpServerEntityStatusCallbackMock : public IDoIpServerEntityStatusCallback
{
public:
    MOCK_CONST_METHOD1(getEntityStatus, EntityStatus(uint8_t socketGroupId));
};

} // namespace doip
