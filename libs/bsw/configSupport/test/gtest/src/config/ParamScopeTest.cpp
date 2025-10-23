// Copyright 2025 Accenture.

#include "config/ParamScope.h"

#include "config/Scope.h"

#include <gtest/gtest.h>

namespace
{
using namespace ::config;
using namespace ::testing;

enum class Param
{
    A,
    B,
    C
};
template<Param Value, typename T>
using ParamId = TypedEnumId<Param, Value, T>;

using ParamAId = ParamId<Param::A, uint32_t>;
using ParamBId = ParamId<Param::B, uint8_t>;

using ParamScopeType = ParamScope<ParamAId, ParamBId>;
using ScopeType      = Scope<ParamScopeType>;
using ConsumerType   = typename ScopeType::Consumer<ScopeType, ParamAId, ParamBId>;

constexpr auto input = ScopeType::prepareInput(param<ParamAId>(15U), param<ParamBId>(5U));

TEST(ParamScopeTest, testCreateScope)
{
    ScopeType s(input);
    ConsumerType consumer;
    s.inject(consumer);
    EXPECT_EQ(15U, consumer.getParam<ParamAId>());
    EXPECT_EQ(5U, consumer.getParam<ParamBId>());
}

} // namespace
