// Copyright 2024 Accenture.

#pragma once

#include <can/SocketCanTransceiver.h>
#include <config/ConfigIds.h>
#include <systems/ICanSystem.h>

namespace config
{

class CanSystem final
: public ComponentBase<ScopeType, CtxId<Ctx::CAN>>
, private ::async::IRunnable
{
public:
    // [PUBLIC_API_START]
    CanSystem();
    CanSystem(CanSystem const&)            = delete;
    CanSystem& operator=(CanSystem const&) = delete;

    void init();
    void start();
    void stop();

    // [PUBLIC_API_END]

    void execute() final;

    static constexpr auto services()
    {
        return serviceAccessor<CanSystem>()
            .member<::can::SocketCanTransceiver, &CanSystem::_canTransceiver>()
            .service<CanId<Bus::CAN_0>>();
    }

private:
    ::async::TimeoutType _timeout;
    ::async::ContextType _context;

    ::can::SocketCanTransceiver _canTransceiver;
};

} // namespace systems
