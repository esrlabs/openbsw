// Copyright 2024 Accenture.

#include "systems/TransportSystem.h"

#include <lifecycle/ILifecycleManager.h>

#include <platform/estdint.h>

DEFINE_COMPONENT(
    ::config::CompId<::config::Comp::TRANSPORT>,
    config,
    transportSystem,
    ::config::TransportSystem)

namespace config
{

void TransportSystem::init()
{
    _transportRouter.init();

    // Inform the lifecycle manager that the transition has been completed
    transitionDone();
}

void TransportSystem::start()
{
    // Inform the lifecycle manager that the transition has been completed
    transitionDone();
}

void TransportSystem::stop()
{
    // Inform the lifecycle manager that the transition has been completed
    transitionDone();
}

void TransportSystem::addTransportLayer(::transport::AbstractTransportLayer& layer)
{
    _transportRouter.addTransportLayer(layer);
}

void TransportSystem::removeTransportLayer(::transport::AbstractTransportLayer& layer)
{
    _transportRouter.removeTransportLayer(layer);
}

::transport::ITransportMessageProvider& TransportSystem::getTransportMessageProvider()
{
    return _transportRouter;
}

} // namespace config
