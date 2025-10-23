// Copyright 2025 Accenture.

#ifndef GUARD_FBD116E4_056B_403A_9854_11E10708EA58
#define GUARD_FBD116E4_056B_403A_9854_11E10708EA58

#include <estd/tuple.h>
#include <estd/type_traits.h>
#include <platform/estdint.h>

namespace config
{

namespace internal
{

struct Empty
{};

using Default = Empty;

struct Strong : public Default
{};

} // namespace internal

template<typename T>
struct Id
{
    using Type = T;
};

template<typename Enum, Enum Value>
struct EnumId
{
    using ValueType = Enum;
};

template<typename Enum, Enum Value, typename T>
struct TypedEnumId
{
    using Type                  = T;
    using ValueType             = T;
    static constexpr Enum value = Value;
};

// free functions
template<typename T>
constexpr auto createTuple(T&& t)
{
    return ::std::make_tuple(::std::forward<T>(t));
}

template<typename... Ts>
constexpr auto createTuple(::std::tuple<Ts...>&& tuple)
{
    return ::std::move(tuple);
}

} // namespace config

#endif // GUARD_FBD116E4_056B_403A_9854_11E10708EA58
