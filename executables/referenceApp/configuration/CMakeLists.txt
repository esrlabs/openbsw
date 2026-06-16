add_library(configuration common/src/busid/BusId.cpp)

target_include_directories(configuration PUBLIC include)

target_link_libraries(configuration PUBLIC common async)
