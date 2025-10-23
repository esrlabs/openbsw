// Copyright 2024 Accenture.

#include "app/app.h"

#include "console/console.h"
#include "estd/typed_mem.h"
#include "logger/logger.h"
#include "reset/softwareSystemReset.h"

#include <app/appConfig.h>
#include <config/AppHandler.h>
#include <config/ConfigIds.h>
#include <config/LifecycleHelper.h>

#include <async/AsyncBinding.h>
#include <lifecycle/LifecycleLogger.h>
#include <lifecycle/LifecycleManager.h>

namespace platform
{
extern void initPlatform(::config::ScopeType& scope);
} // namespace platform

namespace config
{
using ::util::logger::LIFECYCLE;
using ::util::logger::Logger;

using AsyncAdapter = ::async::AsyncBinding::AdapterType;

constexpr auto lifecycleDesc = lifecycle(
    level<
        CompId<Comp::BSP>,
        CompId<Comp::CAN>,
        CompId<Comp::SAFETY>>(),
    level<CompId<Comp::TRANSPORT>>(),
    level<CompId<Comp::DOCAN>>(),
    level<CompId<Comp::UDS>>(),
    level<CompId<Comp::DEMO>>(),
    level<
        CompId<Comp::RUNTIME>,
        CompId<Comp::SYSADMIN>>());

constexpr auto scopeInput = ScopeType::prepareInput(
    context<CtxId<Ctx::BSP>>(TASK_BSP),
    context<CtxId<Ctx::CAN>>(TASK_CAN),
    context<CtxId<Ctx::DEMO>>(TASK_DEMO),
    context<CtxId<Ctx::DIAG>>(TASK_UDS),
    context<CtxId<Ctx::LIFECYCLE>>(TASK_SYSADMIN),
    context<CtxId<Ctx::RUNTIME>>(TASK_BACKGROUND),
    context<CtxId<Ctx::SAFETY>>(TASK_SAFETY));

ScopeType appScope(scopeInput);

AppHandler<
    ComponentHandlerType,
    decltype(lifecycleDesc),
    Types<
        CompId<Comp::BSP>,
#ifdef PLATFORM_SUPPORT_CAN
        CompId<Comp::CAN>,
        CompId<Comp::DOCAN>,
#endif
#ifdef PLATFORM_SUPPORT_UDS
        CompId<Comp::TRANSPORT>,
        CompId<Comp::UDS>,
#endif
        CompId<Comp::DEMO>,
        CompId<Comp::RUNTIME>,
        CompId<Comp::SAFETY>,
        CompId<Comp::SYSADMIN>>>
    appHandler(
        TASK_SYSADMIN,
        ::lifecycle::LifecycleManager::GetTimestampType::create<&getSystemTimeUs32Bit>());

using AsyncAdapter        = ::async::AsyncBinding::AdapterType;
using AsyncRuntimeMonitor = ::async::AsyncBinding::RuntimeMonitorType;
using AsyncContextHook    = ::async::AsyncBinding::ContextHookType;

char const* const isrGroupNames[ISR_GROUP_COUNT] = {"test"};

AsyncRuntimeMonitor runtimeMonitor{
    AsyncContextHook::InstanceType::GetNameType::create<&AsyncAdapter::getTaskName>(),
    isrGroupNames};


class LifecycleMonitor : private ::lifecycle::ILifecycleListener
{
public:
    explicit LifecycleMonitor(::lifecycle::ILifecycleManager& manager)
    {
        manager.addLifecycleListener(*this);
    }

    bool isReadyForReset() const { return _isReadyForReset; }

private:
    void lifecycleLevelReached(
        uint8_t const level,
        ::lifecycle::ILifecycleComponent::Transition::Type const /* transition */) override
    {
        if (0 == level)
        {
            _isReadyForReset = true;
        }
    }

private:
    bool _isReadyForReset = false;
};

LifecycleMonitor lifecycleMonitor(appHandler.getLifecycleManager());

void staticInit()
{
    ::logger::init();

    ::console::init();
    ::console::enable();
}

void staticShutdown()
{
    Logger::info(LIFECYCLE, "Lifecycle shutdown complete");
    ::logger::flush();

    softwareSystemReset();
}

void run()
{
    printf("hello\r\n");
    staticInit();
    AsyncAdapter::init();

    ::platform::initPlatform(appScope);

    appScope.setService<Id<::async::AsyncBinding::RuntimeMonitorType>>(runtimeMonitor);
    appScope.setService<Id<::lifecycle::ILifecycleManager>>(
        appHandler.getLifecycleManager());
    appHandler.init<ComponentLookup>(appScope);

    appHandler.getLifecycleManager().transitionToLevel(
        appHandler.getLifecycleManager().getLevelCount());

    runtimeMonitor.start();
    AsyncAdapter::run();

    while (true)
    {
        ;
    }
}

void idle(AsyncAdapter::TaskContextType& taskContext)
{
    taskContext.dispatchWhileWork();
    ::logger::run();
    ::console::run();
    if (lifecycleMonitor.isReadyForReset())
    {
        staticShutdown();
    }
}

using IdleTask = AsyncAdapter::IdleTask<1024 * 2>;
IdleTask idleTask{"idle", AsyncAdapter::TaskFunctionType::create<&idle>()};

using TimerTask = AsyncAdapter::TimerTask<1024 * 1>;
TimerTask timerTask{"timer"};

using UdsTask = AsyncAdapter::Task<TASK_UDS, 1024 * 2>;
UdsTask udsTask{"uds"};

using SysadminTask = AsyncAdapter::Task<TASK_SYSADMIN, 1024 * 2>;
SysadminTask sysadminTask{"sysadmin"};

using CanTask = AsyncAdapter::Task<TASK_CAN, 1024 * 2>;
CanTask canTask{"can"};

using BspTask = AsyncAdapter::Task<TASK_BSP, 1024 * 2>;
BspTask bspTask{"bsp"};

using DemoTask = AsyncAdapter::Task<TASK_DEMO, 1024 * 2>;
DemoTask demoTask{"demo"};

using BackgroundTask = AsyncAdapter::Task<TASK_BACKGROUND, 1024 * 2>;
BackgroundTask backgroundTask{"background"};

using SafetyTask = AsyncAdapter::Task<TASK_SAFETY, 1024 * 2>;
SafetyTask safetyTask{"safety"};

AsyncContextHook contextHook{runtimeMonitor};

} // namespace config

namespace app
{
void run()
{
    ::config::run();
}
}


