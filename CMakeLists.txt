# ---------------------------------------------------------------------------- #
#                                Standard setup                                #
# ---------------------------------------------------------------------------- #

cmake_minimum_required(VERSION 3.13)
project(soc-multi-srv)
set(CMAKE_CXX_STANDARD 17)

# ---------------------------------------------------------------------------- #
#                                Compiler setup                                #
# ---------------------------------------------------------------------------- #

#set(CMAKE_CXX_COMPILER g++)
#set(CMAKE_CXX_FLAGS "-fPIC -g -D DEBUG -Wall -Wfatal-errors -fstrict-aliasing")

# ---------------------------------------------------------------------------- #
#                                Subdirectories                                #
# ---------------------------------------------------------------------------- #

add_subdirectory(src)
add_subdirectory(program)

#add_subdirectory(test)
#add_subdirectory(lib/wave)
#add_subdirectory(lib/googletest)



# ---------------------------------------------------------------------------- #
#                                 Target setup                                 #
# ---------------------------------------------------------------------------- #

set(SOURCE_LIBS libsocketserver libcommands libnetworkinghal-linux libcommands)
set(SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/program/main.cpp)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${CMAKE_PROJECT_NAME} ${SOURCE_LIBS})

# ---------------------------------------------------------------------------- #
#                                 Include dirs                                 #
# ---------------------------------------------------------------------------- #

target_include_directories(${CMAKE_PROJECT_NAME} INTERFACE ${CMAKE_SOURCE_DIR}/inc
                            PUBLIC ${CMAKE_SOURCE_DIR}/src
                            PUBLIC ${CMAKE_SOURCE_DIR}
                            )
