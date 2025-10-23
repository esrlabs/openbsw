// Copyright 2024 Accenture.

#include "systems/BspSystem.h"

DEFINE_COMPONENT(::config::CompId<::config::Comp::BSP>, config, bspSystem, ::config::BspSystem)

namespace config
{

void BspSystem::init() { transitionDone(); }

void BspSystem::start() { transitionDone(); }

void BspSystem::stop() { transitionDone(); }

} // namespace systems
