// Copyright 2025 Accenture.

#include "config/ContextScope.h"

#include "config/Scope.h"

#include <gtest/gtest.h>

namespace
{
using namespace ::config;
using namespace ::testing;

enum class Context
{
    CONTEXT_A,
    CONTEXT_B,
    CONTEXT_C
};

template<Context>
struct ContextId
{};

using ContextAId = ContextId<Context::CONTEXT_A>;
using ContextBId = ContextId<Context::CONTEXT_B>;

using ContextScopeType = ContextScope<ContextAId, ContextBId>;
using ScopeType        = Scope<ContextScopeType>;
using ConsumerType     = typename ScopeType::Consumer<ScopeType, ContextAId, ContextBId>;

struct TestConsumer : public ConsumerType
{};

constexpr auto input = ScopeType::prepareInput(context<ContextAId>(1U), context<ContextBId>(2U));

TEST(ContextScopeTest, testCreateScope)
{
    ScopeType s(input);
    ConsumerType consumer;
    s.inject(consumer);
    EXPECT_EQ(1U, consumer.getContext<ContextAId>());
    EXPECT_EQ(2U, consumer.getContext<ContextBId>());
}

} // namespace
