cmake_minimum_required(VERSION 3.10)

add_library(numcpp matrix.cpp)
target_include_directories(numcpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(driver driver/main.cpp)
target_link_libraries(driver numcpp)

