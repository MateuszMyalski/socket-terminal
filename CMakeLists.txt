# ---------------------------------------------------------------------------- #
#                                Standard setup                                #
# ---------------------------------------------------------------------------- #

cmake_minimum_required(VERSION 3.13)
project(soc-multi-srv)
set(CMAKE_CXX_STANDARD 17)

# ---------------------------------------------------------------------------- #
#                                Subdirectories                                #
# ---------------------------------------------------------------------------- #

add_subdirectory(src)
add_subdirectory(program)

#add_subdirectory(test)
#add_subdirectory(lib/wave)
#add_subdirectory(lib/googletest)

# ---------------------------------------------------------------------------- #
#                                 Include dirs                                 #
# ---------------------------------------------------------------------------- #

include_directories(SYSTEM inc)
include_directories(SYSTEM src)

# ---------------------------------------------------------------------------- #
#                                 Target setup                                 #
# ---------------------------------------------------------------------------- #

set(SOURCE_LIBS libsocketserver)
set(SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/program/main.cpp)
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${CMAKE_PROJECT_NAME} ${SOURCE_LIBS})