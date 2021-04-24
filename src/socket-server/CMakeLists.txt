set(LIB_SOURCE_FILES socket_server.cpp 
                     client_session.cpp 
    )

add_library(libsocketserver ${LIB_SOURCE_FILES})

target_include_directories(libsocketserver INTERFACE ${CMAKE_SOURCE_DIR}/inc
                            PUBLIC ${CMAKE_SOURCE_DIR}/src
                            PUBLIC ${CMAKE_SOURCE_DIR}
                            )