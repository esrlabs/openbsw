// Copyright 2025 Accenture.

#pragma once

#include "doip/server/IDoIpUdpOemMessageHandler.h"

#include <gmock/gmock.h>

namespace doip
{

class DoIpUdpOemMessageHandlerMock : public IDoIpUdpOemMessageHandler
{
public:
    MOCK_CONST_METHOD1(createResponse, void(::estd::slice<uint8_t> response));
    MOCK_CONST_METHOD0(getRequestPayloadType, uint16_t(void));
    MOCK_CONST_METHOD0(getRequestPayloadSize, uint8_t(void));
    MOCK_CONST_METHOD0(getResponsePayloadType, uint16_t(void));
    MOCK_CONST_METHOD0(getResponsePayloadSize, uint8_t(void));
    MOCK_CONST_METHOD2(
        onPayloadReceived, bool(::estd::slice<uint8_t const> payload, uint8_t& nackCode));
};
} // namespace doip
