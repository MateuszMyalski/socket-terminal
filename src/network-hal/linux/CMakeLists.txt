set(LIB_SOURCE_FILES sockets_api.cpp)

add_library(libnetworkinghal-linux ${LIB_SOURCE_FILES})

target_include_directories(libnetworkinghal-linux INTERFACE ${CMAKE_SOURCE_DIR}/inc
                            PUBLIC ${CMAKE_SOURCE_DIR}/src
                            PUBLIC ${CMAKE_SOURCE_DIR}
                            )