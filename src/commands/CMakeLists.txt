set(LIB_SOURCE_FILES basic_cmd.cpp commands_dispatcher.cpp)

add_library(libcommands ${LIB_SOURCE_FILES})

target_include_directories(libcommands INTERFACE ${CMAKE_SOURCE_DIR}/inc
                            PUBLIC ${CMAKE_SOURCE_DIR}/src
                            PUBLIC ${CMAKE_SOURCE_DIR}
                            )
