// Copyright 2024 Accenture.

#include "systems/SysAdminSystem.h"

namespace
{
constexpr uint32_t SYSTEM_CYCLE_TIME = 10;
}

DEFINE_COMPONENT(
    ::config::CompId<::config::Comp::SYSADMIN>, config, sysAdminSystem, ::config::SysAdminSystem)

namespace config
{

SysAdminSystem::SysAdminSystem()
: _timeout()
, _lifecycleControlCommand(getService<Id<::lifecycle::ILifecycleManager>>())
, _asyncCommandWrapperForLifecycleControlCommand(
      _lifecycleControlCommand, getContext<CtxId<Ctx::LIFECYCLE>>())
{}

void SysAdminSystem::init() { transitionDone(); }

void SysAdminSystem::start()
{
    ::async::ContextType context = getContext<CtxId<Ctx::LIFECYCLE>>();
    ::async::scheduleAtFixedRate(
        context, *this, _timeout, SYSTEM_CYCLE_TIME, ::async::TimeUnit::MILLISECONDS);

    transitionDone();
}

void SysAdminSystem::stop()
{
    _timeout.cancel();

    transitionDone();
}

void SysAdminSystem::execute()
{
    //
}

} // namespace config
