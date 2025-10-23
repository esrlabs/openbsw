// Copyright 2024 Accenture.

#include "systems/CanSystem.h"

#include <can/CanLogger.h>

DEFINE_COMPONENT(::config::CompId<::config::Comp::CAN>, config, canSystem, ::config::CanSystem)

namespace config
{
static uint32_t const TIMEOUT_CAN_SYSTEM_IN_MS = 1U;
static int const MAX_SENT_PER_RUN              = 3;
static int const MAX_RECEIVED_PER_RUN          = 3;

using ::util::logger::CAN;
using ::util::logger::Logger;

namespace
{

::can::SocketCanTransceiver::DeviceConfig canConfig{"vcan0", ::busid::CAN_0};

} // namespace

CanSystem::CanSystem() : _canTransceiver(canConfig) {}

void CanSystem::init() { transitionDone(); }

void CanSystem::start()
{
    _canTransceiver.init();
    _canTransceiver.open();
    ::async::scheduleAtFixedRate(
        getContext<CtxId<Ctx::CAN>>(),
        *this,
        _timeout,
        TIMEOUT_CAN_SYSTEM_IN_MS,
        ::async::TimeUnit::MILLISECONDS);
    transitionDone();
}

void CanSystem::stop()
{
    _timeout.cancel();
    _canTransceiver.close();
    _canTransceiver.shutdown();
    transitionDone();
}

void CanSystem::execute() { _canTransceiver.run(MAX_SENT_PER_RUN, MAX_RECEIVED_PER_RUN); }

} // namespace systems
