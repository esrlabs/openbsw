// Copyright 2024 Accenture.

#include "systems/BspSystem.h"

#include "bsp/timer/SystemTimer.h"
#include "outputPwm/OutputPwm.h"

DEFINE_COMPONENT(::config::CompId<::config::Comp::BSP>, config, bspSystem, ::config::BspSystem)

namespace config
{

BspSystem::BspSystem() : BspSystem(getContext<CtxId<Ctx::BSP>>()) {}

BspSystem::BspSystem(::async::ContextType const context)
: _analogTester()
, _outputPwmTester()
, _digitalInputTester()
, _outputTester()
, _asyncCommandWrapper_for_analogTester(_analogTester, context)
, _asyncOutputPwmTester(_outputPwmTester, context)
, _asyncDigitalInputTester(_digitalInputTester, context)
, _asyncOutputTester(_outputTester, context)
{}

void BspSystem::init() { transitionDone(); }

void BspSystem::start()
{
    ::async::scheduleAtFixedRate(
        getContext<CtxId<Ctx::BSP>>(),
        *this,
        _timeout,
        10u,
        ::async::TimeUnit::MILLISECONDS);
    transitionDone();
}

void BspSystem::stop()
{
    _timeout.cancel();
    transitionDone();
}

void BspSystem::execute()
{
    cyclic();
    getService<Id<StaticBsp>>().cyclic();
}

uint8_t bspSystemCycleCount = 0;

void BspSystem::cyclic() {}

} // namespace config
