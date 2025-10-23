// Copyright 2025 Accenture.

#ifndef GUARD_DE017B63_699C_48AA_84B9_824425CF29FC
#define GUARD_DE017B63_699C_48AA_84B9_824425CF29FC

#include "async/AsyncBinding.h"

#include <config/ComponentBase.h>
#include <config/ComponentHandler.h>
#include <config/ContextScope.h>
#include <config/DataAddOn.h>
#include <config/ParamScope.h>
#include <config/Scope.h>
#include <config/ServiceScope.h>
#include <config/Traits.h>
#include <config/Types.h>

class StaticBsp;

namespace can
{
class ICanTransceiver;
}

namespace lifecycle
{
class ILifecycleManager;
}

namespace config
{

class ITransportSystem;

// Services
enum class Bus : uint8_t
{
    CAN_0 = 0,
};
template<Bus bus>
using BusId = EnumId<Bus, bus>;

template<Bus bus>
using CanId = TypedEnumId<Bus, bus, ::can::ICanTransceiver>;

// Components
enum class Comp : uint8_t
{
    BSP,
    CAN,
    DEMO,
    DOCAN,
    RUNTIME,
    SYSADMIN,
    SAFETY,
    TRANSPORT,
    UDS
};
template<Comp comp>
using CompId = EnumId<Comp, comp>;

// Contexts
enum class Ctx : uint8_t
{
    BSP,
    CAN,
    DEMO,
    DIAG,
    LIFECYCLE,
    RUNTIME,
    SAFETY
};
template<Ctx ctx>
using CtxId = EnumId<Ctx, ctx>;

// Scope
using ScopeType = Scope<
    ServiceScope<
        Id<StaticBsp>,
        CanId<Bus::CAN_0>,
        Id<::diag::DataHandler<::diag::IReadDataRequest>>,
        Id<ITransportSystem>,
        Id<::lifecycle::ILifecycleManager>,
        Id<::async::AsyncBinding::RuntimeMonitorType>>,
    ContextScope<
        CtxId<Ctx::BSP>,
        CtxId<Ctx::CAN>,
        CtxId<Ctx::DEMO>,
        CtxId<Ctx::DIAG>,
        CtxId<Ctx::LIFECYCLE>,
        CtxId<Ctx::RUNTIME>,
        CtxId<Ctx::SAFETY>>>;

// Add-Ons
using AddOnHandlerType = AddOnHandler<DataAddOn<::diag::IReadDataRequest>>;

} // namespace config

using ComponentHandlerType
    = ::config::ComponentHandler<::config::ScopeType, ::config::AddOnHandlerType>;

DECLARE_COMPONENT_LOOKUP()

#endif // GUARD_DE017B63_699C_48AA_84B9_824425CF29FC
