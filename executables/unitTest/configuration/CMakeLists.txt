add_library(commonImpl common/src/busid/BusId.cpp)

target_include_directories(commonImpl PUBLIC common/include)

target_link_libraries(commonImpl PRIVATE common)

# Empty dummy lib for unit tests
add_library(configuration INTERFACE)
