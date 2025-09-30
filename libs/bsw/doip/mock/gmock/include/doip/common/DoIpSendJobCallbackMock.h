// Copyright 2025 Accenture.

/**
 * \ingroup doip
 */
#pragma once

#include "doip/common/IDoIpSendJobCallback.h"

#include <gmock/gmock.h>

namespace doip
{
/**
 * Mock for DoIP send job callback.
 * \tparam T send job class type
 */
template<class T>
class DoIpSendJobCallbackMock : public IDoIpSendJobCallback<T>
{
public:
    MOCK_METHOD2_T(releaseSendJob, void(T& sendJob, bool success));
};

} // namespace doip
