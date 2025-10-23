// Copyright 2025 Accenture.

#include "config/AppHandler.h"

#include "config/Traits.h"

#include <lifecycle/LifecycleComponentMock.h>

namespace
{
using namespace ::config;
using namespace ::testing;

struct TestScope
{};

struct ComponentHandlerMock : public ::lifecycle::LifecycleComponentMock
{
    ComponentHandlerMock()
    {
        EXPECT_CALL(*this, construct()).WillRepeatedly(ReturnRef(*this));
    }

    char const* getName() const
    {
        return "name";
    }

    MOCK_METHOD(void, connect, (TestScope&), ());
    MOCK_METHOD(::lifecycle::ILifecycleComponent&, construct, (), ());
};

ComponentHandlerMock* mock1;
ComponentHandlerMock* mock2;

struct AppHandlerTest : public Test
{
    void SetUp() override
    {
        ::mock1 = &mock1;
        ::mock2 = &mock2;
    }

    void TearDown() override
    {
        ::mock1 = nullptr;
        ::mock2 = nullptr;
    }

    ComponentHandlerMock mock1;
    ComponentHandlerMock mock2;
};

using TestComponentId1 = EnumId<uint32_t, 1>;
using TestComponentId2 = EnumId<uint32_t, 2>;
using TestComponentId3 = EnumId<uint32_t, 3>;

template<typename Id>
ComponentHandlerMock& getComponentHandler();

template<>
ComponentHandlerMock& getComponentHandler<TestComponentId1>()
{
    return *mock1;
}

template<>
ComponentHandlerMock& getComponentHandler<TestComponentId2>()
{
    return *mock2;
}

struct ComponentLookup
{
    template<typename Id>
    static ComponentHandlerMock& get()
    {
        return getComponentHandler<Id>();
    }
};

struct ComponentHandler
{
    using ScopeType = TestScope;
};

TEST_F(AppHandlerTest, testStart)
{
    constexpr auto lifecycleDesc = ::config::lifecycle(
        level<TestComponentId1>(), level<TestComponentId2>(), level<TestComponentId3>());

    AppHandler<
        ComponentHandler,
        decltype(lifecycleDesc),
        ::config::Types<TestComponentId1, TestComponentId2>>
        appHandler(::async::ContextType(1U), ::lifecycle::LifecycleManager::GetTimestampType());

    TestScope scope;

    InSequence seq;
    EXPECT_CALL(mock1, connect(Ref(scope))).Times(1);
    EXPECT_CALL(mock2, connect(Ref(scope))).Times(1);

    EXPECT_CALL(mock1, initCallback(_)).Times(1);
    EXPECT_CALL(mock2, initCallback(_)).Times(1);

    appHandler.init<ComponentLookup>(scope);
}

} // namespace
