// Copyright 2024 Accenture.

#include "systems/RuntimeSystem.h"

namespace
{
constexpr uint32_t SYSTEM_CYCLE_TIME = 1000;
}

DEFINE_COMPONENT(
    ::config::CompId<::config::Comp::RUNTIME>, config, runtimeSystem, ::config::RuntimeSystem)

namespace config
{

RuntimeSystem::RuntimeSystem()
: RuntimeSystem(getContext<CtxId<Ctx::RUNTIME>>())
{}

RuntimeSystem::RuntimeSystem(::async::ContextType context)
: _timeout()
, _statisticsCommand(getService<Id<::async::AsyncBinding::RuntimeMonitorType>>())
, _asyncCommandWrapperForStatisticsCommand(_statisticsCommand, context)
{
}

void RuntimeSystem::init()
{
    _statisticsCommand.setTicksPerUs(0); // use hardware ticks rate

    transitionDone();
}

void RuntimeSystem::start()
{
    ::async::ContextType const context = getContext<CtxId<Ctx::RUNTIME>>();
    ::async::scheduleAtFixedRate(
        context, *this, _timeout, SYSTEM_CYCLE_TIME, ::async::TimeUnit::MILLISECONDS);

    transitionDone();
}

void RuntimeSystem::stop()
{
    _timeout.cancel();

    transitionDone();
}

void RuntimeSystem::execute() { _statisticsCommand.cyclic_1000ms(); }

} // namespace config
