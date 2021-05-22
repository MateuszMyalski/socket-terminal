cmake_minimum_required(VERSION 3.13)
project(soc-multi-srv)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)
add_subdirectory(startup)
add_subdirectory(ut_test)
add_subdirectory(googletest)
add_subdirectory(cmake-conf)

add_custom_target(run)