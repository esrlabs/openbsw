add_library(configSupport INTERFACE)

target_include_directories(configSupport INTERFACE include)

target_link_libraries(configSupport INTERFACE estd async lifecycle)
