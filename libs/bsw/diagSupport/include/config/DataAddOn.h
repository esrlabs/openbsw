// Copyright 2025 Accenture.

#ifndef GUARD_B85AE337_4A54_4733_B588_8EB3250217B5
#define GUARD_B85AE337_4A54_4733_B588_8EB3250217B5

#include "diag/DataHandler.h"
#include "diag/IReadDataRequest.h"

#include <config/Access.h>
#include <config/AddOnHandler.h>
#include <config/Traits.h>

#include <estd/array.h>
#include <estd/big_endian.h>

namespace config
{

namespace internal
{

template<typename T, size_t N>
struct Array
{
    static constexpr size_t size = N;

    T data[N];
};

template<typename Component, typename R>
struct IdBuilder
{
    using RequestType        = R;
    using IdInfoType         = typename ::diag::DataHandler<R>::IdInfo;
    using HandleFunctionType = typename ::diag::DataHandler<R>::HandleFunctionType;

    template<typename CtxId>
    static constexpr uint8_t getContextIndex()
    {
        return Component::ScopeType::ContextIds::template IndexOf<CtxId>::value;
    }

    static constexpr uint8_t getDefaultContextIndex()
    {
        return getContextIndex<typename Component::DefaultCtxId>();
    }

    template<typename CtxId>
    constexpr IdBuilder fromContext() const
    {
        return IdBuilder{dataId, handleFunction, getContextIndex<CtxId>(), isAlwaysActive};
    }

    constexpr IdBuilder alwaysActive() const
    {
        return IdBuilder{dataId, handleFunction, contextIdx, true};
    }

    constexpr IdInfoType build() const
    {
        return IdInfoType{handleFunction, dataId, contextIdx, isAlwaysActive};
    }

    uint16_t dataId;
    HandleFunctionType handleFunction;
    uint8_t contextIdx;
    bool isAlwaysActive;
};

template<typename T>
static void appendAndSendPositiveResponse(::diag::IReadDataRequest& request, T const& value)
{
    uint8_t buffer[sizeof(T)];
    ::estd::write_be<T>(buffer, value);
    request.appendData(buffer);
    request.sendResponse(::diag::IReadDataRequest::ResponseCode::OK);
}

template<typename Component>
using ReadIdBuilder = IdBuilder<Component, ::diag::IReadDataRequest>;

template<typename Component, typename PathAccessor>
struct DiagAccessor
{
    using Type = typename PathAccessor::Type;

    template<bool (Type::*Method)(::diag::IReadDataRequest&)>
    constexpr auto readData(uint16_t dataId) const
    {
        return ReadIdBuilder<Component>{
            dataId,
            &DiagAccessor<Component, PathAccessor>::callRead<Method>,
            ReadIdBuilder<Component>::getDefaultContextIndex(),
            false};
    }

    template<typename T, T (Type::*Method)() const>
    constexpr auto readData(uint16_t dataId) const
    {
        return ReadIdBuilder<Component>{
            dataId,
            &DiagAccessor::sendValue<T, Method>,
            ReadIdBuilder<Component>::getDefaultContextIndex(),
            false};
    }

private:
    template<bool (Type::*Method)(::diag::IReadDataRequest&)>
    static bool callRead(void* component, ::diag::IReadDataRequest& request)
    {
        return (PathAccessor::bind(*static_cast<Component*>(component)).*Method)(request);
    }

    template<typename T, T (Type::*Method)() const>
    static bool sendValue(void* component, ::diag::IReadDataRequest& request)
    {
        T const value = (PathAccessor::bind(*static_cast<Component*>(component)).*Method)();
        appendAndSendPositiveResponse(request, value);
        return true;
    }
};

template<typename Component>
constexpr auto callDiagnostics(Strong) ->
    typename ::std::result_of<decltype (&Component::diagnostics)()>::type
{
    return Component::diagnostics();
}

template<typename Component>
constexpr auto callDiagnostics(Default) -> Empty
{
    return {};
}

template<
    typename R,
    typename... Args,
    size_t... Idxs,
    typename = typename ::std::enable_if<(sizeof...(Idxs) > 0)>::type>
constexpr auto
extractRequests(::std::tuple<Args...> const& requests, ::std::index_sequence<Idxs...>)
{
    return Array<typename ::diag::DataHandler<R>::IdInfo, sizeof...(Idxs)>{
        {std::get<Idxs>(requests).build()...}};
}

template<typename R, typename... Args>
constexpr auto extractRequests(::std::tuple<Args...> const& requests, ::std::index_sequence<>)
    -> Empty
{
    return {};
}

template<typename Component, typename R, typename... Args>
constexpr auto extractRequests(::std::tuple<Args...> const& requests)
{
    return extractRequests<R>(
        requests, typename Types<Args...>::template IndicesOf<IdBuilder<Component, R>>::Type{});
}

template<typename Component, typename R>
constexpr auto extractRequests(::std::tuple<Empty>) -> Empty
{
    return {};
}

template<typename Component, typename R>
constexpr auto extractRequests()
{
    return extractRequests<Component, R>(createTuple(callDiagnostics<Component>(Strong{})));
}

} // namespace internal

template<typename R>
struct DataAddOn
{
    template<
        typename Parent,
        typename Scope,
        typename Component,
        bool = ::std::is_same<
            decltype(::config::internal::extractRequests<Component, R>()),
            ::config::internal::Empty>::value>
    class For : public ::config::AddOnHandler<>::For<Parent, Scope, Component>
    {};

    template<typename Parent, typename Scope, typename Component>
    class For<Parent, Scope, Component, false>
    : public Scope::NestedConsumer<Parent, Id<::diag::DataHandler<R>>>
    {
    public:
        using ComponentType = Component;

        For() : _componentHandler(_componentInfo) {}

        void postInit()
        {
            _componentHandler.initComponent(&static_cast<Parent&>(*this).getComponent());
            this->template getService<Id<::diag::DataHandler<R>>>().addComponentHandler(
                _componentHandler);
        }

        void postStart() { _componentHandler.setActive(true); }

        void preStop() { _componentHandler.setActive(false); }

    private:
        using IdInfosType = decltype(::config::internal::extractRequests<ComponentType, R>());

        static constexpr IdInfosType _idInfos
            = ::config::internal::extractRequests<ComponentType, R>();
        static constexpr typename ::diag::DataHandler<R>::ComponentInfo _componentInfo{
            _idInfos.data, _idInfos.size};

        typename ::diag::DataHandler<R>::ComponentHandler _componentHandler;
    };
};

template<typename R>
template<typename Parent, typename Scope, typename Component>
constexpr typename DataAddOn<R>::For<Parent, Scope, Component, false>::IdInfosType
    DataAddOn<R>::For<Parent, Scope, Component, false>::_idInfos;

template<typename R>
template<typename Parent, typename Scope, typename Component>
constexpr typename ::diag::DataHandler<R>::ComponentInfo
    DataAddOn<R>::For<Parent, Scope, Component, false>::_componentInfo;

// free functions
template<typename Component>
constexpr auto diagAccessor()
{
    return ComponentAccessor<::config::internal::DiagAccessor, Component>{};
}

} // namespace config

#endif // GUARD_B85AE337_4A54_4733_B588_8EB3250217B5
