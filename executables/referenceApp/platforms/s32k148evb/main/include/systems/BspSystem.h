// Copyright 2024 Accenture.

#pragma once

#include "console/AsyncCommandWrapper.h"
#include "inputManager/DigitalInputTester.h"
#include "lifecycle/StaticBsp.h"
#include "outputManager/OutputTester.h"
#include "outputPwm/OutputPwmTester.h"
#include "tester/AnalogTester.h"

#include <config/ConfigIds.h>

namespace config
{

class BspSystem
: public ComponentBase<
      ScopeType,
      CtxId<Ctx::BSP>,
      Types<Id<StaticBsp>>>
, private ::async::RunnableType
{
public:
    BspSystem();

    void init();
    void start();
    void stop();

    void cyclic();

private:
    BspSystem(::async::ContextType const context);

    void execute() override;

    ::async::TimeoutType _timeout;
    bios::AnalogTester _analogTester;
    bios::OutputPwmTester _outputPwmTester;
    bios::DigitalInputTester _digitalInputTester;
    bios::OutputTester _outputTester;
    ::console::AsyncCommandWrapper _asyncCommandWrapper_for_analogTester;
    ::console::AsyncCommandWrapper _asyncOutputPwmTester;
    ::console::AsyncCommandWrapper _asyncDigitalInputTester;
    ::console::AsyncCommandWrapper _asyncOutputTester;
};

} // namespace config
