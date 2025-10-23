// Copyright 2025 Accenture.

#include "config/ComponentHandler.h"

#include "config/Access.h"
#include "config/Scope.h"
#include "config/ServiceScope.h"

#include <lifecycle/SimpleLifecycleComponent.h>

#include <gtest/gtest.h>

namespace
{
using namespace ::config;
using namespace ::testing;

struct TestServiceA
{
    uint32_t value;
};

struct TestServiceB
{
    uint32_t value;
};

struct TestServiceC
{
    uint32_t value;
};

struct TestServiceD
{
    uint32_t value;
};

struct DerivedServiceB : public TestServiceB
{
    DerivedServiceB(uint32_t value) : TestServiceB{value} {}
};

struct TestNestedService
{
    TestServiceD serviceD{4712U};
};

using TestServiceScopeType
    = ServiceScope<Id<TestServiceA>, Id<TestServiceB>, Id<TestServiceC>, Id<TestServiceD>>;
using TestScopeType    = Scope<TestServiceScopeType>;
using TestConsumerType = typename TestScopeType::Consumer<
    TestServiceScopeType,
    Id<TestServiceA>,
    Id<TestServiceB>,
    Id<TestServiceC>,
    Id<TestServiceD>>;

struct TestComponent
: public TestScopeType::Consumer<TestComponent, Id<TestServiceC>>
, public ::lifecycle::SimpleLifecycleComponent
{
public:
    TestComponent() : _a{174U}, _b{29437U}, _c(getService<Id<TestServiceC>>()) {}

    static constexpr auto services()
    {
        auto accessor = serviceAccessor<TestComponent>();
        return all(
            accessor.member<TestServiceA, &TestComponent::_a>().service<Id<TestServiceA>>(),
            accessor.methodCall<DerivedServiceB, &TestComponent::getTestServiceB>()
                .service<Id<TestServiceB>>(),
            accessor.member<TestNestedService, &TestComponent::_nested>()
                .member<TestServiceD, &TestNestedService::serviceD>()
                .service<Id<TestServiceD>>());
    }

    void init() override {}

    void run() override {}

    void shutdown() override {}

    // static constexpr auto commands()

    TestServiceA& getTestServiceA() { return _a; }

    DerivedServiceB& getTestServiceB() { return _b; }

    TestServiceC& getTestServiceC() { return _c; }

    TestServiceD& getTestServiceD() { return _nested.serviceD; }

private:
    TestServiceA _a;
    DerivedServiceB _b;
    TestServiceC& _c;
    TestNestedService _nested;
};

using CompHandlerType    = ComponentHandler<TestScopeType>;
using CompForHandlerType = typename CompHandlerType::For<TestComponent>;

CompForHandlerType instance("name");

TEST(ComponentHandlerTest, testComponentCreate)
{
    TestServiceC serviceC;
    TestScopeType scope{TestScopeType::prepareInput()};
    TestConsumerType consumer;
    scope.setService<Id<TestServiceC>>(serviceC);
    scope.inject(consumer);
    EXPECT_STREQ("name", instance.getName());
    instance.connect(scope);
    EXPECT_TRUE(&consumer.getService<Id<TestServiceA>>() != nullptr);
    EXPECT_TRUE(&consumer.getService<Id<TestServiceB>>() != nullptr);
    EXPECT_TRUE(&consumer.getService<Id<TestServiceD>>() != nullptr);
    EXPECT_TRUE(&instance.construct() != nullptr);
    EXPECT_EQ(174U, consumer.getService<Id<TestServiceA>>().value);
    EXPECT_EQ(29437U, consumer.getService<Id<TestServiceB>>().value);
    EXPECT_EQ(4712U, consumer.getService<Id<TestServiceD>>().value);
}

using CompId               = Id<TestComponent>;
using ComponentHandlerType = ComponentHandler<TestScopeType>;

DECLARE_COMPONENT_LOOKUP()
DEFINE_COMPONENT(CompId, test, testComp, TestComponent)

TEST(ComponentHandlerTest, testComponentCreateByMacro)
{
    TestServiceC serviceC;
    TestScopeType scope{TestScopeType::prepareInput()};
    TestConsumerType consumer;
    EXPECT_EQ(&ComponentLookup::get<CompId>(), &test::testCompHandler);
    EXPECT_EQ(&getComponentHandler<CompId>(), &test::testCompHandler);
    scope.setService<Id<TestServiceC>>(serviceC);
    scope.inject(consumer);
    test::testCompHandler.connect(scope);
    EXPECT_TRUE(&consumer.getService<Id<TestServiceA>>() != nullptr);
    EXPECT_TRUE(&consumer.getService<Id<TestServiceB>>() != nullptr);
    EXPECT_TRUE(&consumer.getService<Id<TestServiceD>>() != nullptr);
    test::testCompHandler.construct();
    EXPECT_EQ(174U, consumer.getService<Id<TestServiceA>>().value);
    EXPECT_EQ(29437U, consumer.getService<Id<TestServiceB>>().value);
    EXPECT_EQ(4712U, consumer.getService<Id<TestServiceD>>().value);
}

} // namespace
