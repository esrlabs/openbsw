add_executable(
    configSupportTest
    #src/config/AppHandlerTest.cpp
    #src/config/ComponentHandlerTest.cpp
    src/config/ContextScopeTest.cpp
    src/config/LifecycleHelperTest.cpp
    src/config/ParamScopeTest.cpp
    src/config/ServiceScopeTest.cpp
    src/config/TypesTest.cpp)

target_link_libraries(
    configSupportTest
    PRIVATE configSupport
            estdMock
            lifecycle
            lifecycleMock
            asyncMockImpl
            gtest_main)

gtest_discover_tests(configSupportTest PROPERTIES LABELS "configSupportTest")
