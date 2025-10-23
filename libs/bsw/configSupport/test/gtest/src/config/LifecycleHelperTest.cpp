// Copyright 2025 Accenture.

#include "config/LifecycleHelper.h"

#include "config/Traits.h"

#include <gtest/gtest.h>

namespace
{
using namespace ::config;
using namespace ::testing;

template<uint32_t Value>
using CompId = EnumId<uint32_t, Value>;

template<size_t Value>
using StoreId = EnumId<size_t, Value>;

TEST(LifecycleHelperTest, testLifecycle)
{
    constexpr auto lifecycleDesc = ::config::lifecycle(
        storeId<StoreId<0>>(),
        level<CompId<0>, CompId<1>>(),
        level<CompId<2>, CompId<3>, CompId<4>>(),
        storeId<StoreId<1>>(),
        level<CompId<5>, CompId<6>>(),
        level<CompId<7>>(),
        storeId<StoreId<2>>());
    using LifecycleDescType = decltype(lifecycleDesc);

    EXPECT_EQ(4, size_t(LifecycleDescType::levelCount));
    EXPECT_EQ(8, size_t(LifecycleDescType::componentCount));
    EXPECT_EQ(3, size_t(LifecycleDescType::maxComponentPerLevelCount));

    EXPECT_EQ(0, size_t(LifecycleDescType::getLevel<StoreId<0>>()));
    EXPECT_EQ(2, size_t(LifecycleDescType::getLevel<StoreId<1>>()));
    EXPECT_EQ(4, size_t(LifecycleDescType::getLevel<StoreId<2>>()));

    using FilteredLifecycleDescType = typename ::config::internal::FilterLifecycleHelper<
        ::config::Types<CompId<0>, CompId<2>, CompId<4>, CompId<6>, CompId<7>>,
        typename LifecycleDescType::DescTypes>::Type;

    EXPECT_EQ(4, size_t(FilteredLifecycleDescType::levelCount));
    EXPECT_EQ(5, size_t(FilteredLifecycleDescType::componentCount));
    EXPECT_EQ(2, size_t(FilteredLifecycleDescType::maxComponentPerLevelCount));

    EXPECT_EQ(0, size_t(FilteredLifecycleDescType::getLevel<StoreId<0>>()));
    EXPECT_EQ(2, size_t(FilteredLifecycleDescType::getLevel<StoreId<1>>()));
    EXPECT_EQ(4, size_t(FilteredLifecycleDescType::getLevel<StoreId<2>>()));
}

} // namespace
