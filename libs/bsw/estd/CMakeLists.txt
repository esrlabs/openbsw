add_library(estd INTERFACE)

target_include_directories(estd INTERFACE include)

target_link_libraries(estd INTERFACE util platform)

if (BUILD_EXECUTABLE STREQUAL "unitTest")

    target_compile_definitions(estd INTERFACE ESTD_HAS_EXCEPTIONS
                                              ESTL_ASSERT_MESSAGE_ALL)

    add_library(estdMock INTERFACE)

    target_include_directories(estdMock INTERFACE mock/include)

endif ()
