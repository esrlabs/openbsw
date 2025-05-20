add_library(asyncCoreConfiguration INTERFACE)

if (BUILD_TARGET_PLATFORM MATCHES "FREERTOS")
    target_include_directories(asyncCoreConfiguration
                               INTERFACE freertos/include)
elseif (BUILD_TARGET_PLATFORM MATCHES "THREADX")
    target_include_directories(asyncCoreConfiguration INTERFACE threadx/include)
endif ()

target_link_libraries(asyncCoreConfiguration INTERFACE common)
