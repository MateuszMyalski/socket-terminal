set(LIB_SOURCE_FILES in_socket_api.cpp socket_api.cpp)

add_library(libnetworkinghal-linux ${LIB_SOURCE_FILES})

target_include_directories(libnetworkinghal-linux INTERFACE ${CMAKE_SOURCE_DIR}/inc
                            PUBLIC ${CMAKE_SOURCE_DIR}/src
                            PUBLIC ${CMAKE_SOURCE_DIR}
                            )