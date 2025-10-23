// Copyright 2024 Accenture.

#include "systems/SafetySystem.h"

#include <safeLifecycle/SafetyLogger.h>
#include <safeLifecycle/SafetyManager.h>

::safety::SafetyManager safetyManager;

namespace
{
constexpr uint32_t SYSTEM_CYCLE_TIME = 10;
}

DEFINE_COMPONENT(
    ::config::CompId<::config::Comp::SAFETY>, config, safetySystem, ::config::SafetySystem)

namespace config
{
using ::util::logger::Logger;
using ::util::logger::SAFETY;

void SafetySystem::init()
{
    safetyManager.init();
    transitionDone();
}

void SafetySystem::start()
{
    ::async::scheduleAtFixedRate(
        getContext<CtxId<Ctx::SAFETY>>(),
        *this,
        _timeout,
        SYSTEM_CYCLE_TIME,
        ::async::TimeUnit::MILLISECONDS);

    transitionDone();
}

void SafetySystem::stop()
{
    _timeout.cancel();

    transitionDone();
}

void SafetySystem::execute() { cyclic(); }

void SafetySystem::cyclic() { safetyManager.cyclic(); }

} // namespace systems
