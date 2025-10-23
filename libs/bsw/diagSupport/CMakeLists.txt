add_library(diagSupport 
            src/uds/ReadDataById.cpp)

target_include_directories(diagSupport PUBLIC include)

target_link_libraries(diagSupport INTERFACE configSupport 
                                  PUBLIC uds)
