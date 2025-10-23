// Copyright 2025 Accenture.

#ifndef GUARD_EA8E6C15_1E9C_4454_80B2_2E2D77AC646B
#define GUARD_EA8E6C15_1E9C_4454_80B2_2E2D77AC646B

#include <estd/tuple.h>
#include <platform/estdint.h>

namespace config
{

namespace internal
{

template<typename Component>
struct BasePathAccessor
{
    using ComponentType = Component;
    using Type          = Component;

    static Type& bind(Component& component) { return component; }
};

template<typename PathAccessor, typename T, T PathAccessor::Type::*Member>
struct MemberPathAccessor
{
    using ComponentType = typename PathAccessor::ComponentType;
    using Type          = T;

    static Type& bind(ComponentType& component) { return PathAccessor::bind(component).*Member; }
};

template<typename PathAccessor, typename T, T& (PathAccessor::Type::*Method)()>
struct MethodCallPathAccessor
{
    using ComponentType = typename PathAccessor::ComponentType;
    using Type          = T;

    static Type& bind(ComponentType& component)
    {
        return (PathAccessor::bind(component).*Method)();
    }
};

} // namespace internal

template<
    template<typename, typename>
    typename ScopeAccessor,
    typename Component,
    typename PathAccessor = ::config::internal::BasePathAccessor<Component>>
struct ComponentAccessor : public ScopeAccessor<Component, PathAccessor>
{
    using Type             = typename PathAccessor::Type;
    using ComponentType    = Component;
    using PathAccessorType = PathAccessor;

    static Type& bind(Component& component) { return PathAccessor::bind(component); }

    template<typename T, T Type::*Member>
    constexpr auto member() const
    {
        return ComponentAccessor<
            ScopeAccessor,
            Component,
            ::config::internal::MemberPathAccessor<PathAccessor, T, Member>>{};
    }

    template<typename T, T& (Type::*Method)()>
    constexpr auto methodCall() const
    {
        return ComponentAccessor<
            ScopeAccessor,
            Component,
            ::config::internal::MethodCallPathAccessor<PathAccessor, T, Method>>{};
    }
};

// free functions
template<typename... Descs>
constexpr auto all(Descs&&... descs)
{
    return ::std::make_tuple(::std::forward<Descs>(descs)...);
}

} // namespace config

#endif // GUARD_EA8E6C15_1E9C_4454_80B2_2E2D77AC646B
