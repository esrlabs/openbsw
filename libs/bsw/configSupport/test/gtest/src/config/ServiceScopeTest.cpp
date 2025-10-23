// Copyright 2025 Accenture.

#include "config/ServiceScope.h"

#include "config/Scope.h"

#include <gtest/gtest.h>

namespace
{
using namespace ::config;
using namespace ::testing;

struct TestServiceA
{};

struct TestServiceB
{};

using ServiceScopeType = ServiceScope<Id<TestServiceA>, Id<TestServiceB>>;
using ScopeType        = Scope<ServiceScopeType>;
using ConsumerType = typename ScopeType::Consumer<ScopeType, Id<TestServiceA>, Id<TestServiceB>>;

constexpr auto input = ScopeType::prepareInput();

TEST(ServiceScopeTest, testCreateScope)
{
    TestServiceA serviceA;
    TestServiceB serviceB;

    ScopeType s(input);
    ConsumerType consumer;
    s.inject(consumer);
    s.setService<Id<TestServiceA>>(serviceA);
    EXPECT_EQ(&serviceA, &consumer.getService<Id<TestServiceA>>());
    s.setService<Id<TestServiceB>>(serviceB);
    EXPECT_EQ(&serviceB, &consumer.getService<Id<TestServiceB>>());
}

} // namespace
