// Copyright 2024 Accenture.

#include "systems/CanSystem.h"

#include "async/Config.h"
#include "async/Hook.h"
#include "bsp/can/canConfiguration.h"
#include "busid/BusId.h"
#include "lifecycle/ILifecycleManager.h"
#include "lifecycle/StaticBsp.h"
#include "util/command/IParentCommand.h"

#include <lifecycle/StaticBsp.h>

DEFINE_COMPONENT(::config::CompId<::config::Comp::CAN>, config, canSystem, ::config::CanSystem)

namespace config
{

CanSystem::CanSystem()
: CanSystem(
    getContext<CtxId<Ctx::CAN>>(), getService<Id<StaticBsp>>())
{}

CanSystem::CanSystem(::async::ContextType context, StaticBsp& staticBsp)
: ::estd::singleton<CanSystem>(*this)
, _context(context)
, _transceiver0(
      context,
      ::busid::CAN_0,
      Can0Config,
      staticBsp.getCanPhy(),
      staticBsp.getPowerStateController())
, _canRxRunnable(*this)
{}

void CanSystem::init() { transitionDone(); }

void CanSystem::start()
{
    _canRxRunnable.setEnabled(true);

    (void)_transceiver0.init();
    (void)_transceiver0.open();

    transitionDone();
}

void CanSystem::stop()
{
    (void)_transceiver0.close();
    _transceiver0.shutdown();

    _canRxRunnable.setEnabled(false);

    transitionDone();
}

void CanSystem::dispatchRxTask() { ::async::execute(_context, _canRxRunnable); }

CanSystem::CanRxRunnable::CanRxRunnable(CanSystem& parent) : _parent(parent), _enabled(false) {}

void CanSystem::CanRxRunnable::execute()
{
    if (_enabled)
    {
        _parent._transceiver0.receiveTask();
    }
}

} // namespace config

extern "C"
{
void call_can_isr_RX()
{
    ::asyncEnterIsrGroup(ISR_GROUP_CAN);

    uint8_t framesReceived;
    {
        ::async::LockType const lock;
        framesReceived = bios::CanFlex2Transceiver::receiveInterrupt(0);
    }

    if (framesReceived > 0)
    {
        ::config::CanSystem::instance().dispatchRxTask();
    }

    ::asyncLeaveIsrGroup(ISR_GROUP_CAN);
}

void call_can_isr_TX()
{
    ::asyncEnterIsrGroup(ISR_GROUP_CAN);
    bios::CanFlex2Transceiver::transmitInterrupt(0);
    ::asyncLeaveIsrGroup(ISR_GROUP_CAN);
}

} // extern "C"
