// Copyright 2024 Accenture.

#pragma once

#include <async/Async.h>
#include <async/IRunnable.h>
#include <config/ConfigIds.h>
#include <console/AsyncCommandWrapper.h>
#include <lifecycle/console/StatisticsCommand.h>

namespace config
{

class RuntimeSystem
: public ComponentBase<
      ScopeType,
      CtxId<Ctx::RUNTIME>,
      Types<Id<::async::AsyncBinding::RuntimeMonitorType>>>
, private ::async::IRunnable
{
public:
    RuntimeSystem();
    RuntimeSystem(RuntimeSystem const&)            = delete;
    RuntimeSystem& operator=(RuntimeSystem const&) = delete;

    void init();
    void start();
    void stop();

private:
    RuntimeSystem(::async::ContextType context);

    void execute() override;

private:
    ::async::TimeoutType _timeout;

    ::lifecycle::StatisticsCommand _statisticsCommand;
    ::console::AsyncCommandWrapper _asyncCommandWrapperForStatisticsCommand;
};

} // namespace config
