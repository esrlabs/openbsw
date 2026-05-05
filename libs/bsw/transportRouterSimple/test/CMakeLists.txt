add_executable(transportRouterSimpleTest src/TransportRouterSimpleTest.cpp)

# The router's ETH_0 branch is behind PLATFORM_SUPPORT_ETHERNET. In the
# referenceApp build this is set globally via Options.cmake. But for our test we
# need to ensure the library was compiled with it as well.
target_compile_definitions(transportRouterSimple
                           PRIVATE PLATFORM_SUPPORT_ETHERNET)

target_link_libraries(
    transportRouterSimpleTest
    PRIVATE transportRouterSimple
            transportConfiguration
            asyncMockImpl
            gmock_main
            transportMock
            utilMock
            etl)

gtest_discover_tests(transportRouterSimpleTest
                     PROPERTIES LABELS "transportRouterSimpleTest")
