// Copyright 2025 Accenture.

#ifndef GUARD_C0166B92_4B0B_467A_BD9D_6DF585FA0F9B
#define GUARD_C0166B92_4B0B_467A_BD9D_6DF585FA0F9B

#include <config/LifecycleHelper.h>

namespace config
{

namespace internal
{

template<typename ComponentLookup, typename... CompIds>
struct ConnectHelper
{
    static void connect(){};
};

template<typename ComponentLookup, typename CompId, typename... CompIds>
struct ConnectHelper<ComponentLookup, CompId, CompIds...>
{
    static void connect()
    {
        ComponentLookup::template get<CompId>().connect();
        ConnectHelper<ComponentLookup, CompIds...>::connect();
    }
};

} // namespace internal

template<
    typename ComponentHandler,
    typename LifecycleDesc,
    typename CompIds = typename LifecycleDesc::CompIds>
struct AppHandler
{};

template<typename... CompIds, typename ComponentHandler, typename... Descs>
class AppHandler<
    ComponentHandler,
    ::config::internal::LifecycleDesc<Descs...> const,
    ::config::Types<CompIds...>>
{
public:
    AppHandler(
        ::async::ContextType const transitionContext,
        ::lifecycle::LifecycleManager::GetTimestampType const getTimestamp)
    : _lifecycleManager(transitionContext, getTimestamp)
    {}

    ::lifecycle::ILifecycleManager& getLifecycleManager() { return _lifecycleManager; }

    template<typename ComponentLookup>
    void init(typename ComponentHandler::ScopeType& scope)
    {
        ComponentHandler::init(scope);
        ::config::internal::ConnectHelper<ComponentLookup, CompIds...>::connect();
        ::config::internal::addLevels<ComponentLookup>(
            _lifecycleManager, typename LifecycleDescType::LevelDescTypes());
    }

private:
    using LifecycleDescType = typename ::config::internal::
        FilterLifecycleHelper<::config::Types<CompIds...>, ::config::Types<Descs...>>::Type;

    using LifecycleManagerType = typename ::lifecycle::declare::LifecycleManager<
        LifecycleDescType::componentCount,
        LifecycleDescType::levelCount,
        LifecycleDescType::maxComponentPerLevelCount>;

    template<typename ComponentLookup>
    struct ConstructLookup
    {
        template<typename CompId>
        static auto get()
        {
            auto handler = ComponentLookup::template get<CompId>();
            handler.construct();
            return handler;
        }
    };

    LifecycleManagerType _lifecycleManager;
};

} // namespace config

#endif // GUARD_C0166B92_4B0B_467A_BD9D_6DF585FA0F9B
