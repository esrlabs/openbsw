// Copyright 2025 Accenture.

#ifndef GUARD_F833E5E6_FBEB_491F_A284_A7025E1A7BE1
#define GUARD_F833E5E6_FBEB_491F_A284_A7025E1A7BE1

#include "config/Traits.h"
#include "config/Types.h"

#include <estd/array.h>

namespace config
{

template<typename... SIds>
class ServiceScope
{
public:
    using Ids       = Types<SIds...>;
    using InputType = ::config::internal::Empty;

    template<typename Parent, typename... CIds>
    struct Consumer
    {
        using ConsumerServiceIds = Types<CIds...>;

        template<
            typename SId,
            typename
            = typename ::std::enable_if<Types<CIds...>::template IsContained<SId>::value>::type>
        typename SId::Type& getService() const
        {
            return static_cast<Parent const*>(this)->getScope().template getService<SId>();
        }
    };

    template<typename Desc>
    static constexpr auto prepareInput(Desc const&)
    {
        return ::config::internal::Empty();
    }

    ServiceScope(::config::internal::Empty const& = ::config::internal::Empty()) : _services() {}

    template<typename SId>
    void setService(typename SId::Type& service)
    {
        static size_t const Idx = Ids::template IndexOf<SId>::value;
        static_assert(Idx < sizeof...(SIds), "Invalid Service");
        void*& servicePtr = _services[Idx];
        estd_assert(servicePtr == nullptr);
        servicePtr = &service;
    }

private:
    template<typename SId>
    typename SId::Type& getService() const
    {
        static size_t const Idx = Ids::template IndexOf<SId>::value;
        static_assert(Idx < sizeof...(SIds), "Invalid Service");
        void* const& servicePtr = _services[Idx];
        estd_assert(servicePtr != nullptr);
        return *static_cast<typename SId::Type*>(servicePtr);
    }

    using ServiceArrayType = ::estd::array<void*, sizeof...(SIds)>;

    ServiceArrayType _services;
};

} // namespace config

#endif // GUARD_F833E5E6_FBEB_491F_A284_A7025E1A7BE1
