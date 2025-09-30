// Copyright 2025 Accenture.

/**
 * \ingroup doip
 */
#pragma once

#include "doip/common/IDoIpTransportMessageProvidingListener.h"

#include <gmock/gmock.h>

namespace doip
{
/**
 * Mock for DoIP specific transport message providing listener.
 */
class DoIpTransportMessageProvidingListenerMock : public IDoIpTransportMessageProvidingListener
{
public:
    MOCK_METHOD6(
        getTransportMessage,
        GetResult(
            uint8_t sourceBusId,
            uint16_t sourceId,
            uint16_t targetId,
            uint16_t size,
            ::estd::slice<uint8_t const> const& peek,
            ::transport::TransportMessage*& transportMessage));
    MOCK_METHOD1(releaseTransportMessage, void(::transport::TransportMessage& transportMessage));
    MOCK_METHOD3(
        messageReceived,
        ReceiveResult(
            uint8_t sourceBusId,
            ::transport::TransportMessage& transportMessage,
            ::transport::ITransportMessageProcessedListener* notificationListener));
};

} // namespace doip
