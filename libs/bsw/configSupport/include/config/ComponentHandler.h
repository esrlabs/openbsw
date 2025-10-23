// Copyright 2025 Accenture.

#ifndef GUARD_DF3C58FA_5991_4E9A_8432_180492086BCB
#define GUARD_DF3C58FA_5991_4E9A_8432_180492086BCB

#include "config/Access.h"
#include "config/AddOnHandler.h"
#include "config/Traits.h"
#include "config/Types.h"

#include <lifecycle/ILifecycleComponent.h>
#include <lifecycle/ILifecycleComponentCallback.h>

#include <estd/type_traits.h>

namespace config
{

namespace internal
{

template<typename Id, typename Accessor>
struct ServiceDesc
{
    using ServiceId = Id;

    template<typename Component>
    static typename Id::Type& bind(Component& component)
    {
        return Accessor::bind(component);
    }
};

template<typename Component, typename PathAccessor>
struct ServiceAccessor
{
    template<typename Id>
    constexpr auto service() const
    {
        return ServiceDesc<Id, PathAccessor>{};
    }
};

template<typename Component>
constexpr auto callServices(::config::internal::Strong) ->
    typename ::std::result_of<decltype (&Component::services)()>::type
{
    return Component::services();
}

template<typename Component>
constexpr auto callServices(Default) -> ::std::tuple<>
{
    return {};
}

template<typename Scope, typename Component>
void connect(Scope&, Component&)
{}

template<typename Scope, typename Component, typename Desc, typename... Descs>
void connect(Scope& scope, Component& component)
{
    scope.template setService<typename Desc::ServiceId>(Desc::template bind<Component>(component));
    connect<Scope, Component, Descs...>(scope, component);
}

} // namespace internal

template<typename Scope, typename AddOnHandler = ::config::AddOnHandler<>>
struct ComponentHandler
{
    using ScopeType        = Scope;
    using AddOnHandlerType = AddOnHandler;

    class IComponentHandler
    {
    public:
        virtual char const* getName() const                   = 0;
        virtual void connect()                                = 0;
        virtual ::lifecycle::ILifecycleComponent& construct() = 0;
    };

    static void init(Scope& scope) { Scope::ConsumerBase::init(scope); }

    template<typename Component>
    class For
    : public IComponentHandler
    , public ScopeType::ConsumerBase
    , public ScopeType::NestedConsumer<For<Component>, typename Component::DefaultCtxId>
    , public AddOnHandler::For<For<Component>, Scope, Component>
    , private ::lifecycle::ILifecycleComponent
    , private IComponentCallback
    {
    public:
        using ComponentType = Component;

        For(char const* name) : _name(name), _buffer(), _component(*_buffer.cast_to_type()) {}

        char const* getName() const override { return _name; }

        void connect() override
        {
            ConnectHelper<decltype(::config::internal::callServices<Component>(
                ::config::internal::Strong()))>::
                connect(ScopeType::ConsumerBase::getScope(), _component);
        }

        ::lifecycle::ILifecycleComponent& construct() override
        {
            (void)new (&_component) Component();
            return *this;
        }

        Component& getComponent() { return _component; }

    private:
        using AddOnHandlerType = typename AddOnHandler::For<For<Component>, Scope, Component>;

        void initCallback(::lifecycle::ILifecycleComponentCallback& callback) override
        {
            _callback = &callback;
            _component.initCallback(*this);
        }

        ::async::ContextType getTransitionContext(Transition::Type /*transition*/)
        {
            return static_cast<typename ScopeType::NestedConsumer<
                For<Component>,
                typename Component::DefaultCtxId>*>(this)
                ->template getContext<typename Component::DefaultCtxId>();
        }

        void startTransition(::lifecycle::ILifecycleComponent::Transition::Type transition)
        {
            _transition = transition;
            switch (transition)
            {
                case Transition::Type::INIT: _component.init(); break;
                case Transition::Type::RUN:  _component.start(); break;
                // case Transition::Type::SHUTDOWN:
                default:
                    AddOnHandlerType::preStop();
                    _component.stop();
                    break;
            }
        }

    private:
        template<typename T>
        struct ConnectHelper
        {
            static void connect(Scope& scope, Component& component)
            {
                ::config::internal::connect<Scope, Component, T>(scope, component);
            }
        };

        template<typename... Descs>
        struct ConnectHelper<::std::tuple<Descs...>>
        {
            static void connect(Scope& scope, Component& component)
            {
                ::config::internal::connect<Scope, Component, Descs...>(scope, component);
            }
        };

        void transitionDone()
        {
            switch (_transition)
            {
                case Transition::Type::INIT: AddOnHandlerType::postInit(); break;
                case Transition::Type::RUN:  AddOnHandlerType::postStart(); break;
                // case Transition::Type::SHUTDOWN:
                default:                     break;
            }
            if (_callback != nullptr)
            {
                _callback->transitionDone(*this);
            }
        }

        char const* _name;
        ::estd::aligned_mem<Component> _buffer;
        Component& _component;
        ::lifecycle::ILifecycleComponentCallback* _callback = nullptr;
        ::lifecycle::ILifecycleComponent::Transition::Type _transition
            = ::lifecycle::ILifecycleComponent::Transition::Type::INIT;
    };
};

// free functions
template<typename Component>
constexpr auto serviceAccessor()
{
    return ComponentAccessor<::config::internal::ServiceAccessor, Component>{};
}

} // namespace config

// macros
#define COMPONENT_CONCAT(X, Y)  X##Y
#define COMPONENT_CONCAT2(X, Y) COMPONENT_CONCAT(X, Y)

#define DECLARE_COMPONENT_LOOKUP()                                           \
    template<typename CompId>                                                \
    typename ComponentHandlerType::IComponentHandler& getComponentHandler(); \
    struct ComponentLookup                                                   \
    {                                                                        \
        template<typename CompId>                                            \
        static typename ComponentHandlerType::IComponentHandler& get()       \
        {                                                                    \
            return getComponentHandler<CompId>();                            \
        }                                                                    \
    };

#define DEFINE_COMPONENT(_id, _namespace, _name, _type)                                  \
    namespace _namespace                                                                 \
    {                                                                                    \
    typename ComponentHandlerType::For<_type> COMPONENT_CONCAT2(_name, Handler)(#_name); \
    }                                                                                    \
    template<>                                                                           \
    typename ComponentHandlerType::IComponentHandler& getComponentHandler<_id>()         \
    {                                                                                    \
        return _namespace::COMPONENT_CONCAT2(_name, Handler);                            \
    }

#endif // GUARD_DF3C58FA_5991_4E9A_8432_180492086BCB
