// Copyright 2025 Accenture.

#ifndef GUARD_A8C12A7C_1666_49E3_8FDC_54B3BF149DFE
#define GUARD_A8C12A7C_1666_49E3_8FDC_54B3BF149DFE

#include "config/Access.h"
#include "config/AddOnHandler.h"
#include "config/Traits.h"

#include <util/command/CommandContext.h>

namespace config
{

namespace internal
{

struct CommandInfo
{
    
};

struct CommandDesc
{
    using ExecuteFunction = void (*)(void*, ::util::command::CommandContext&);

    char const* name;
    char const* desc;
    ExecuteFunction execute;
};

template<typename Component, typename PathAccessor>
struct CommandAccessor
{
    using Type             = typename PathAccessor::Type;

    template<void (Type::*Method)(::util::command::CommandContext&)>
    constexpr auto command(char const* name, char const* desc) const
    {
        return CommandDesc{name, desc, &CommandAccessor::executeCommand<Method>};
    }

    template<void (Type::*Method)(::util::command::CommandContext&)>
    static void executeCommand(void* component, ::util::command::CommandContext& context)
    {
        (PathAccessor::bind(*static_cast<Component*>(component)).*Method)(context);
    }
};

template<typename Component>
constexpr auto callCommands(::config::internal::Strong) ->
    typename ::std::result_of<decltype (&Component::commands)()>::type
{
    return Component::commands();
}

template<typename Component>
constexpr auto callCommands(Default) -> Empty
{
    return {};
}

} // namespace internal

struct CommandAddOn
{
    template<
        typename Component,
        bool = ::std::is_same<
            decltype(::config::internal::callCommands<Component>(::config::internal::Strong())),
            ::config::internal::Empty>::value>
    class For
    {};

    template<typename Component>
    class For<Component, true> : public ::config::AddOnHandler<>::For<Component>
    {};
};

// free functions
template<typename Component>
constexpr auto commandAccessor()
{
    return ComponentAccessor<::config::internal::CommandAccessor, Component>{};
}

} // namespace config

#endif // GUARD_A8C12A7C_1666_49E3_8FDC_54B3BF149DFE
