// Copyright 2025 Accenture.

#ifndef GUARD_C07BAC2F_F51B_46D6_B18D_3226582A2DEA
#define GUARD_C07BAC2F_F51B_46D6_B18D_3226582A2DEA

#include "config/Types.h"

#include <lifecycle/LifecycleManager.h>

#include <estd/algorithm.h>
#include <estd/type_traits.h>

namespace config
{

namespace internal
{

template<typename... CIds>
struct LevelDesc
{
    using CompIds = Types<CIds...>;
};

template<typename SId>
struct StoreDesc
{};

template<typename T>
struct IsLevelDesc
{
    static constexpr bool value = false;
};

template<typename... CIds>
struct IsLevelDesc<LevelDesc<CIds...>>
{
    static constexpr bool value = true;
};

template<typename T>
struct IsStoreDesc
{
    static constexpr bool value = false;
};

template<typename SId>
struct IsStoreDesc<StoreDesc<SId>>
{
    static constexpr bool value = true;
};

template<typename... LevelDescs>
struct MaxComponentPerLevelHelper
{
    static constexpr size_t value = 0;
};

template<typename... LevelDescs>
struct MaxComponentPerLevelHelper<Types<LevelDescs...>>
: public MaxComponentPerLevelHelper<LevelDescs...>
{};

template<typename... CIds, typename... LevelDescs>
struct MaxComponentPerLevelHelper<LevelDesc<CIds...>, LevelDescs...>
{
    static constexpr size_t value
        = ::estd::max(sizeof...(CIds), MaxComponentPerLevelHelper<LevelDescs...>::value);
};

template<typename...>
struct ComponentUnionHelper
{};

template<typename... LevelDescs>
struct ComponentUnionHelper<Types<LevelDescs...>>
{
    using Type = typename Union<typename LevelDescs::CompIds...>::Type;
};

template<size_t Idx, typename SId, typename... Descs>
struct GetLevelHelper
{};

template<size_t Idx, typename SId, typename T, typename... Descs>
struct GetLevelHelper<Idx, SId, T, Descs...>
{
    static constexpr size_t value = GetLevelHelper<Idx, SId, Descs...>::value;
};

template<size_t Idx, typename SId, typename... CIds, typename... Descs>
struct GetLevelHelper<Idx, SId, LevelDesc<CIds...>, Descs...>
{
    static constexpr size_t value = GetLevelHelper<Idx + 1, SId, Descs...>::value;
};

template<size_t Idx, typename SId, typename... Descs>
struct GetLevelHelper<Idx, SId, StoreDesc<SId>, Descs...>
{
    static constexpr size_t value = Idx;
};

template<typename ComponentLookup, typename LifecycleManager, typename... CIds>
struct AddComponentHelper
{
    static void add(LifecycleManager&, uint8_t) {}
};

template<typename ComponentLookup, typename LifecycleManager, typename CId, typename... CIds>
struct AddComponentHelper<ComponentLookup, LifecycleManager, CId, CIds...>
{
    static void add(LifecycleManager& lifecycleManager, uint8_t level)
    {
        auto& componentHandler = ComponentLookup::template get<CId>();
        auto& lifecycleComponent = componentHandler.construct();
        lifecycleManager.addComponent(componentHandler.getName(), lifecycleComponent, level);
        AddComponentHelper<ComponentLookup, LifecycleManager, CIds...>::add(
            lifecycleManager, level);
    }
};

template<typename ComponentLookup, typename LifecycleManager, typename...>
struct AddLevelHelper
{
    static void add(LifecycleManager&, uint8_t) {}
};

template<typename ComponentLookup, typename LifecycleManager, typename... CIds, typename... Descs>
struct AddLevelHelper<ComponentLookup, LifecycleManager, LevelDesc<CIds...>, Descs...>
{
    static void add(LifecycleManager& lifecycleManager, uint8_t level)
    {
        AddComponentHelper<ComponentLookup, LifecycleManager, CIds...>::add(
            lifecycleManager, level);
        AddLevelHelper<ComponentLookup, LifecycleManager, Descs...>::add(
            lifecycleManager, level + 1);
    }
};

template<typename ComponentLookup, typename LifecycleManager, typename... Descs>
void addLevels(LifecycleManager& lifecycleManager, Types<Descs...>)
{
    AddLevelHelper<ComponentLookup, LifecycleManager, Descs...>::add(lifecycleManager, 1);
}

template<typename... Descs>
struct LifecycleDesc
{
    using DescTypes      = Types<Descs...>;
    using LevelDescTypes = typename Types<Descs...>::Filter<IsLevelDesc>::Type;
    using StoreDescTypes = typename Types<Descs...>::Filter<IsStoreDesc>::Type;
    using CompIds        = typename ComponentUnionHelper<LevelDescTypes>::Type;

    static constexpr size_t componentCount = CompIds::Sizeof::value;
    static constexpr size_t levelCount     = LevelDescTypes::Sizeof::value;
    static constexpr size_t maxComponentPerLevelCount
        = MaxComponentPerLevelHelper<LevelDescTypes>::value;

    static_assert(CompIds::IsUnique::value, "Component Ids must be unique");
    static_assert(levelCount > 0, "At least one level must be defined");
    static_assert(componentCount > 0, "At least one component must be defined");

    template<typename SId>
    static constexpr size_t getLevel()
    {
        return GetLevelHelper<0, SId, Descs...>::value;
    }

    template<typename ComponentLookup>
    static void addLevels(::lifecycle::LifecycleManager& lifecycleManager)
    {
        addLevels<ComponentLookup>(lifecycleManager, LevelDescTypes());
    }
};

template<typename, typename Desc>
struct FilterLevelHelper
{
    using Type = Desc;
};

template<typename... CIds, typename... LevelCIds>
struct FilterLevelHelper<Types<CIds...>, LevelDesc<LevelCIds...>>
{
    template<typename>
    struct LevelDescTypeHelper
    {};

    template<typename... DescCIds>
    struct LevelDescTypeHelper<Types<DescCIds...>>
    {
        using Type = LevelDesc<DescCIds...>;
    };

    using Type = typename LevelDescTypeHelper<
        typename Types<CIds...>::template Intersect<LevelCIds...>::Type>::Type;
};

template<typename, typename>
struct FilterLifecycleHelper
{};

template<typename... CIds, typename... Descs>
struct FilterLifecycleHelper<Types<CIds...>, Types<Descs...>>
{
    using Type = LifecycleDesc<typename FilterLevelHelper<Types<CIds...>, Descs>::Type...>;
};

} // namespace internal

// free functions
template<typename... CompIds>
constexpr auto level()
{
    return ::config::internal::LevelDesc<CompIds...>{};
}

template<typename SId>
constexpr auto storeId()
{
    return ::config::internal::StoreDesc<SId>{};
}

template<typename... Descs>
constexpr auto lifecycle(Descs const&...)
{
    return ::config::internal::LifecycleDesc<Descs...>{};
}

} // namespace config

#endif // GUARD_C07BAC2F_F51B_46D6_B18D_3226582A2DEA
