// Copyright 2024 Accenture.

#pragma once

#include <async/Async.h>
#include <async/IRunnable.h>
#include <config/ConfigIds.h>
#include <console/AsyncCommandWrapper.h>
#include <lifecycle/console/LifecycleControlCommand.h>

namespace config
{

class SysAdminSystem
: public ComponentBase<
      ScopeType,
      CtxId<Ctx::LIFECYCLE>,
      Types<Id<::lifecycle::ILifecycleManager>>>
, private ::async::IRunnable
{
public:
    SysAdminSystem();
    SysAdminSystem(SysAdminSystem const&)            = delete;
    SysAdminSystem& operator=(SysAdminSystem const&) = delete;

    void init();
    void start();
    void stop();

private:
    void execute() override;

private:
    ::async::TimeoutType _timeout;

    ::lifecycle::LifecycleControlCommand _lifecycleControlCommand;
    ::console::AsyncCommandWrapper _asyncCommandWrapperForLifecycleControlCommand;
};

} // namespace config
