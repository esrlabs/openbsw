add_library(asyncBinding INTERFACE)

if (BUILD_TARGET_PLATFORM MATCHES "FREERTOS")
    target_include_directories(asyncBinding INTERFACE freertos/include)
elseif (BUILD_TARGET_PLATFORM MATCHES "THREADX")
    target_include_directories(asyncBinding INTERFACE threadx/include)
endif ()

target_link_libraries(asyncBinding INTERFACE runtime)
