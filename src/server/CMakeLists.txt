set(LIB_SOURCE_FILES socket_server.cpp 
                     client_session.cpp
                     identity.cpp
                     session_controller.cpp 
                     input_collector.cpp
    )

add_library(libserver ${LIB_SOURCE_FILES})

target_include_directories(libserver INTERFACE ${CMAKE_SOURCE_DIR}/inc
                            PUBLIC ${CMAKE_SOURCE_DIR}/src
                            PUBLIC ${CMAKE_SOURCE_DIR}
                            )

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(libserver PRIVATE Threads::Threads)