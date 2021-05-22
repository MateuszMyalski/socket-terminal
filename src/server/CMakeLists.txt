set(SERVER_LIB_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/socket_server.cpp 
                     ${CMAKE_CURRENT_SOURCE_DIR}/client_session.cpp
                     ${CMAKE_CURRENT_SOURCE_DIR}/identity.cpp
                     ${CMAKE_CURRENT_SOURCE_DIR}/session_controller.cpp 
                     ${CMAKE_CURRENT_SOURCE_DIR}/input_collector.cpp
                     PARENT_SCOPE
    )