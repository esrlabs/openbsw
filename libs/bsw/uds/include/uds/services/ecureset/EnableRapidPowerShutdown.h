// Copyright 2024 Accenture.

#pragma once

#include "uds/IUdsResetConfiguration.h"
#include "uds/base/Subfunction.h"
#include "uds/lifecycle/IUdsLifecycleConnector.h"

namespace uds
{
class EnableRapidPowerShutdown : public Subfunction
{
public:
    explicit EnableRapidPowerShutdown(IUdsLifecycleConnector& udsLifecycleConnector);

private:
    static uint8_t const sfImplementedRequest[2];
    static uint8_t const RESPONSE_LENGTH = 1U;
    static uint8_t const ShutDownTime    = 10U; // power down time

    virtual DiagReturnCode::Type process(
        IncomingDiagConnection& connection, uint8_t const request[], uint16_t requestLength) final;
    virtual void responseSent(IncomingDiagConnection& connection, ResponseSendResult result) final;

    IUdsLifecycleConnector& fUdsLifecycleConnector;
};

} // namespace uds
