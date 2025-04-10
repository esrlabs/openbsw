// Copyright 2024 Accenture.

#pragma once
#include "uds/resume/IResumableDiagDispatcher.h"

#include <gmock/gmock.h>

namespace uds
{
class ResumableDiagDispatcherMock : public IResumableDiagDispatcher
{
public:
    ResumableDiagDispatcherMock(IDiagSessionManager& sessionManager, DiagJobRoot& jobRoot)
    : IResumableDiagDispatcher(sessionManager, jobRoot)
    {}

    MOCK_METHOD2(
        resume,
        transport::AbstractTransportLayer::ErrorCode(
            ::transport::TransportMessage& transportMessage,
            ::transport::ITransportMessageProcessedListener* notificationListener));
    MOCK_CONST_METHOD0(getSourceId, uint16_t(void));
    MOCK_METHOD1(dispatchTriggerEventRequest, uint8_t(transport::TransportMessage& msg));
    MOCK_METHOD3(
        getOutgoingDiagConnection,
        ErrorCode(
            uint16_t targetId,
            OutgoingDiagConnection*& connection,
            transport::TransportMessage* pRequestMessage = nullptr));

#ifdef IS_VARIANT_HANDLING_NEEDED
    MOCK_METHOD1(setSourceId, void(uint16_t));
#endif
};
} // namespace uds
