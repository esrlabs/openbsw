set(app.referenceAppExtraSources)

if (PLATFORM_SUPPORT_UDS)
    list(
        APPEND
        app.referenceAppExtraSources
        src/systems/TransportSystem.cpp
        src/systems/UdsSystem.cpp
        src/uds/ReadIdentifierPot.cpp)

    if (PLATFORM_SUPPORT_CAN)
        list(APPEND app.referenceAppExtraSources src/systems/DoCanSystem.cpp)
    endif ()
endif ()

# Base sources
set(BASE_SOURCES
    src/app/CanDemoListener.cpp
    src/console/console.cpp
    src/systems/DemoSystem.cpp
    src/systems/RuntimeSystem.cpp
    src/systems/SysAdminSystem.cpp
    src/systems/SafetySystem.cpp
    ${app.referenceAppExtraSources}
    src/main.cpp)

if (BUILD_TARGET_PLATFORM MATCHES "FREERTOS")
    list(APPEND BASE_SOURCES src/app/app-freertos.cpp
         src/logger/logger-freertos.cpp)
elseif (BUILD_TARGET_PLATFORM MATCHES "THREADX")
    list(
        APPEND
        BASE_SOURCES
        src/app/app-threadx.cpp
        src/systems/BackgroundSystem.cpp
        src/logger/logger-threadx.cpp)
else ()
    message(
        FATAL_ERROR
            "The target_build_platform '${target_build_platform}' is not included in the suppported platforms."
    )
endif ()

add_executable(app.referenceApp ${BASE_SOURCES})

set_target_properties(app.referenceApp PROPERTIES SUFFIX ".elf")

if (TARGET startUp)
    get_target_property(LINKER_SCRIPT startUp PROP_LINKER_SCRIPT)

    if (DEFINED LINKER_SCRIPT)
        target_link_options(app.referenceApp PRIVATE -T ${LINKER_SCRIPT})
    endif ()
endif ()

target_include_directories(app.referenceApp PRIVATE include)

target_link_libraries(
    app.referenceApp
    PRIVATE asyncBinding
            asyncConsole
            bspMcu
            configuration
            estd
            lifecycle
            consoleCommands
            logger
            loggerIntegration
            main
            stdioConsoleInput
            util
            udsConfigurationImpl
            runtime
            safeLifecycle
            socBsp)

if (BUILD_TARGET_PLATFORM MATCHES "FREERTOS")
    target_link_libraries(app.referenceApp PRIVATE osFreeRtos asyncFreeRtosImpl)
elseif (BUILD_TARGET_PLATFORM MATCHES "THREADX")
    target_link_libraries(app.referenceApp PRIVATE threadX)
else ()
    message(
        FATAL_ERROR
            "The target_build_platform '${target_build_platform}' is not included in the suppported platforms."
    )
endif ()

if (PLATFORM_SUPPORT_UDS)

    target_link_libraries(app.referenceApp PRIVATE transport
                                                   transportRouterSimple uds)

endif ()

if (PLATFORM_SUPPORT_CAN)

    target_link_libraries(app.referenceApp PRIVATE cpp2can docan)
endif ()
