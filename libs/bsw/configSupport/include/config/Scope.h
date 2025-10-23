// Copyright 2025 Accenture.

#ifndef GUARD_BCA0622B_17A6_49AD_A154_0D8530201099
#define GUARD_BCA0622B_17A6_49AD_A154_0D8530201099

// #include "config/Types.h"

#include <estd/array.h>

namespace config
{

namespace internal
{};

template<typename... NestedScopes>
struct Scope : public NestedScopes...
{
public:
    struct ConsumerBase
    {
    public:
        ConsumerBase() {}

        static void init(Scope& scope) { _scope = &scope; }

        static Scope& getScope() { return *_scope; }

    private:
        friend class Scope;

        static Scope* _scope;
    };

    template<typename Parent, typename... CIds>
    class NestedConsumer : public NestedScopes::Consumer<Parent, CIds...>...
    {
        using ConsumerType = NestedConsumer<Parent, CIds...>;
    };

    template<typename... CIds>
    class Consumer
    : public ConsumerBase
    , public NestedConsumer<Consumer<CIds...>, CIds...>
    {};

    template<typename... Descs>
    static constexpr auto prepareInput(Descs&&... descs)
    {
        auto const& desc = ::std::make_tuple(::std::forward<Descs>(descs)...);
        return ::std::make_tuple(NestedScopes::prepareInput(desc)...);
    }

    template<typename T>
    Scope(T const& input) : NestedScopes(::std::get<typename NestedScopes::InputType>(input))...
    {}
};

template<typename... NestedScopes>
Scope<NestedScopes...>* Scope<NestedScopes...>::ConsumerBase::_scope = nullptr;

// free functions
template<typename... NestedDescs>
auto scope(NestedDescs const&... descs)
{
    return Scope<typename NestedDescs::ScopeType...>(descs.getInput()...);
}

} // namespace config

#endif // GUARD_BCA0622B_17A6_49AD_A154_0D8530201099
