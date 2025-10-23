// Copyright 2024 Accenture.

#pragma once

#include <async/Async.h>
#include <async/IRunnable.h>
#include <config/ConfigIds.h>
#include <lifecycle/console/LifecycleControlCommand.h>
#ifdef PLATFORM_SUPPORT_CAN
#include "app/CanDemoListener.h"

#include <can/console/CanCommand.h>
#include <console/AsyncCommandWrapper.h>
#include <systems/ICanSystem.h>
#endif

namespace config
{

class DemoSystem
: public ComponentBase<
      ScopeType,
      CtxId<Ctx::DEMO>,
      Types<CanId<Bus::CAN_0>>>
, private ::async::IRunnable
{
public:
    DemoSystem();

    DemoSystem(DemoSystem const&)            = delete;
    DemoSystem& operator=(DemoSystem const&) = delete;

    void init();
    void start();
    void stop();

    void cyclic();

    static constexpr auto diagnostics()
    {
        return all(
            diagAccessor<DemoSystem>().readData<&DemoSystem::readData>(0xc100),
            diagAccessor<DemoSystem>().readData<uint16_t, &DemoSystem::readValue>(
                0xc101));
    }

private:
    DemoSystem(::can::ICanTransceiver& canTransceiver, ::async::ContextType context);

    void execute() override;

    bool readData(::diag::IReadDataRequest& request)
    {
        uint8_t data[] = {0x12U, 0x34U};
        request.appendData(data);
        request.sendResponse(::diag::IReadDataRequest::ResponseCode::OK);
        return true;
    }

    uint16_t readValue() const { return 0x5678U; }

private:
    ::async::TimeoutType _timeout;
#ifdef PLATFORM_SUPPORT_CAN
    ::can::CanDemoListener _canDemoListener;
    ::can::CanCommand _canCommand;
    ::console::AsyncCommandWrapper _asyncCommandWrapperForCanCommand;
#endif
};

} // namespace config
