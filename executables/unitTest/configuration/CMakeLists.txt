add_library(configuration common/src/busid/BusId.cpp)

target_include_directories(configuration PUBLIC common/include)

target_link_libraries(configuration PRIVATE common)

# Empty dummy lib for unit tests add_library(configuration INTERFACE)
