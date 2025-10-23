// Copyright 2024 Accenture.

#include "systems/DemoSystem.h"
#ifdef PLATFORM_SUPPORT_IO
#include <bsp/adc/AnalogInputScale.h>
#include <inputManager/DigitalInput.h>
#include <outputManager/Output.h>
#include <outputPwm/OutputPwm.h>
#endif
#include "app/CanDemoListener.h"
#include "app/DemoLogger.h"

#include <bsp/SystemTime.h>

#include <estd/big_endian.h>
#ifdef PLATFORM_SUPPORT_CAN
#include <can/transceiver/AbstractCANTransceiver.h>
#endif

namespace
{
constexpr uint32_t DEMO_CYCLE_TIME = 10;
}

#ifdef PLATFORM_SUPPORT_IO
using bios::AnalogInput;
using bios::AnalogInputScale;
using bios::DigitalInput;
using bios::Io;
using bios::Output;
using bios::OutputPwm;
#endif

DEFINE_COMPONENT(::config::CompId<::config::Comp::DEMO>, config, demoSystem, ::config::DemoSystem)

namespace config
{
using ::util::logger::DEMO;
using ::util::logger::Logger;

DemoSystem::DemoSystem()
: DemoSystem(
    getService<CanId<Bus::CAN_0>>(),
    getContext<CtxId<Ctx::DEMO>>())
{}

DemoSystem::DemoSystem(::can::ICanTransceiver& canTransceiver, ::async::ContextType context)
: _timeout()
#ifdef PLATFORM_SUPPORT_CAN
, _canDemoListener(canTransceiver)
, _canCommand(canTransceiver)
, _asyncCommandWrapperForCanCommand(_canCommand, context)
#endif
{}

void DemoSystem::init() { transitionDone(); }

void DemoSystem::start()
{
#ifdef PLATFORM_SUPPORT_CAN
    _canDemoListener.run();
#endif
    ::async::ContextType const context = getContext<CtxId<Ctx::DEMO>>();
    ::async::scheduleAtFixedRate(
        context, *this, _timeout, DEMO_CYCLE_TIME, ::async::TimeUnit::MILLISECONDS);
    transitionDone();
}

void DemoSystem::stop()
{
#ifdef PLATFORM_SUPPORT_CAN
    _canDemoListener.shutdown();
#endif
    _timeout.cancel();
    transitionDone();
}

void DemoSystem::execute() { cyclic(); }

void DemoSystem::cyclic()
{
#ifdef PLATFORM_SUPPORT_IO
    uint32_t value;
    static uint8_t timeCounter = 0;
    timeCounter++;

    bool buttonStatus = false;

    // This is to read a digital input.
    DigitalInput::get(DigitalInput::EVAL_SW3, buttonStatus);

    // This is to set an output.
    Output::set(Output::EVAL_LED_RED, buttonStatus ? 1 : 0);

    if (timeCounter >= 50)
    {
        // This is to read an analog input and set the duty cycle of a PWM output.
        if (AnalogInputScale::get(AnalogInput::AiEVAL_POTI_ADC, value) == bsp::BSP_OK)
        {
            OutputPwm::setDuty(OutputPwm::EVAL_LED_GREEN_PWM, value * 10000 / 5000);
            OutputPwm::setDuty(OutputPwm::EVAL_LED_BLUE_PWM, value * 10000 / 5000);
        }
        timeCounter = 0;
    }
#endif

#ifdef PLATFORM_SUPPORT_CAN
    static uint32_t previousSentTime = getSystemTimeMs32Bit();
    static uint32_t canSentCount     = 0;
    uint32_t const now               = getSystemTimeMs32Bit();
    uint32_t const deltaTimeMs       = now - previousSentTime;
    // Send a CAN frame every second.
    if (deltaTimeMs >= 1000)
    {
        previousSentTime = now;
        //::can::ICanTransceiver& canTransceiver
        //    = getService<CanId<Bus::CAN_0>>();
        // Logger::debug(DEMO, "Sending frame %d", canSentCount);
        uint8_t canData[4] = {0};
        ::estd::write_be(canData, canSentCount);
        ::can::CANFrame frame(0x558, canData, 4);
        //canTransceiver.write(frame);
        ++canSentCount;
    }
#endif
}

} // namespace systems
