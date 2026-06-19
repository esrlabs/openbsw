add_library(commonHeaders INTERFACE)

target_include_directories(commonHeaders INTERFACE include)

target_link_libraries(commonHeaders INTERFACE etl platform)

add_library(common INTERFACE)

target_link_libraries(common INTERFACE commonHeaders configuration)
