cmake_minimum_required(VERSION 3.13)
project(hpx_adjacent_difference CXX)

find_package(HPX REQUIRED)
add_executable(hpx_adjacent_difference hpx_adjacent.cpp)
target_link_libraries(hpx_adjacent_difference HPX::hpx HPX::wrap_main)
