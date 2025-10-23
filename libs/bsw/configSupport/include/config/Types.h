// Copyright 2025 Accenture.

#ifndef GUARD_A2834DD8_EEDC_4291_BF91_1CA7E671889F
#define GUARD_A2834DD8_EEDC_4291_BF91_1CA7E671889F

#include <platform/estdint.h>

#include <utility>

namespace config
{

namespace internal
{

template<size_t Idx, typename T, typename... Ts>
struct IndexHelper
{
    static constexpr size_t value = Idx;
};

template<size_t Idx, typename T, typename U, typename... Ts>
struct IndexHelper<Idx, T, U, Ts...>
{
    static constexpr size_t value = IndexHelper<Idx + 1, T, Ts...>::value;
};

template<size_t Idx, typename T, typename... Ts>
struct IndexHelper<Idx, T, T, Ts...>
{
    static constexpr size_t value = Idx;
};

template<typename T, typename... Ts>
struct IndexOf
{
    static constexpr size_t value = IndexHelper<0, T, Ts...>::value;
};

template<size_t Idx, size_t CurrentIdx, typename T, typename... Ts>
struct TypeOfHelper
{
    using Type = typename TypeOfHelper<Idx, CurrentIdx + 1, Ts...>::Type;
};

template<size_t Idx, typename T, typename... Ts>
struct TypeOfHelper<Idx, Idx, T, Ts...>
{
    using Type = T;
};

template<size_t Idx, typename... Ts>
struct TypeOf
{
    using Type = typename TypeOfHelper<Idx, 0, Ts...>::Type;
};

template<typename T, typename... Ts>
struct IsContained
{
    static constexpr bool value = IndexOf<T, Ts...>::value != sizeof...(Ts);
};

template<typename... Ts>
struct IsUnique
{
    static constexpr bool value = true;
};

template<typename T, typename... Ts>
struct IsUnique<T, Ts...>
{
    static constexpr bool value = !IsContained<T, Ts...>::value && IsUnique<Ts...>::value;
};

template<template<typename> typename Pred, typename T, bool = Pred<T>::value>
struct PredType
{
    using Type = void;
};

template<template<typename> typename Pred, typename T>
struct PredType<Pred, T, true>
{
    using Type = T;
};

template<typename T, typename U>
struct IndexSequenceConcat
{};

template<size_t... Idxs1, size_t... Idxs2>
struct IndexSequenceConcat<::std::index_sequence<Idxs1...>, ::std::index_sequence<Idxs2...>>
{
    using Type = ::std::index_sequence<Idxs1..., Idxs2...>;
};

} // namespace internal

template<typename... Ts>
struct Types
{
    template<template<typename...> typename C>
    struct Classed
    {
        using Type = C<Ts...>;
    };

    template<template<typename...> typename C>
    struct ClassedArgs
    {
        using Type = Types<C<Ts>...>;
    };

    struct Sizeof
    {
        static constexpr size_t value = sizeof...(Ts);
    };

    template<typename T>
    struct IndexOf
    {
        static constexpr size_t value = internal::IndexOf<T, Ts...>::value;
    };

    template<typename T>
    struct IndicesOf
    {
    private:
        template<size_t Idx, typename... Us>
        struct IndicesOfHelper
        {
            using Type = ::std::index_sequence<>;
        };

        template<size_t Idx, typename U, typename... Us>
        struct IndicesOfHelper<Idx, U, Us...>
        {
            using Type = typename IndicesOfHelper<Idx + 1, Us...>::Type;
        };

        template<size_t Idx, typename... Us>
        struct IndicesOfHelper<Idx, T, Us...>
        {
            using Type = typename internal::IndexSequenceConcat<
                ::std::index_sequence<Idx>,
                typename IndicesOfHelper<Idx + 1, Us...>::Type>::Type;
        };

    public:
        using Type = typename IndicesOfHelper<0, Ts...>::Type;
    };

    template<size_t Idx>
    struct TypeOf
    {
        using Type = typename internal::TypeOf<Idx, Ts...>::Type;
    };

    template<typename T>
    struct IsContained
    {
        static constexpr bool value = internal::IsContained<T, Ts...>::value;
    };

    struct IsUnique
    {
        static constexpr bool value = internal::IsUnique<Ts...>::value;
    };

    template<typename T>
    struct Append
    {
        using Type = Types<Ts..., T>;
    };

    template<typename... Us>
    struct Append<Types<Us...>>
    {
        using Type = Types<Ts..., Us...>;
    };

    template<template<typename> typename Pred>
    class Filter
    {
    private:
        template<typename...>
        struct FilterHelper
        {
            using Type = Types<>;
        };

        template<typename U, typename... Us>
        struct FilterHelper<U, Us...>
        {
            using Type = typename Types<U>::Append<typename FilterHelper<Us...>::Type>::Type;
        };

        template<typename... Us>
        struct FilterHelper<void, Us...>
        {
            using Type = typename FilterHelper<Us...>::Type;
        };

    public:
        using Type = typename FilterHelper<typename internal::PredType<Pred, Ts>::Type...>::Type;
    };

    template<typename... Us>
    struct Intersect : public Intersect<Types<Us...>>
    {};

    template<typename... Us>
    struct Intersect<Types<Us...>>
    {
        using Type = typename Types<Us...>::Filter<IsContained>::Type;
    };
};

template<typename... Ts, typename... Us>
struct Types<Types<Ts...>, Us...> : public Types<Ts..., Us...>
{};

template<typename...>
struct Union
{
    using Type = Types<>;
};

template<typename... Ts, typename... Us>
struct Union<Types<Ts...>, Us...>
{
    using Type = typename Union<Ts..., Us...>::Type;
};

template<typename T, typename... Us>
struct Union<T, Us...>
{
    using Type = typename Types<T>::Append<typename Union<Us...>::Type>::Type;
};

} // namespace config

#endif // GUARD_A2834DD8_EEDC_4291_BF91_1CA7E671889F
