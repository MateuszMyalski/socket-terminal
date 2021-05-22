add_subdirectory(ut_main)
add_subdirectory(commands)
add_subdirectory(server)

set(COMMANDS_UT_SOURCES ${COMMANDS_UT_SOURCES} ${UT_MAIN_SOURCE})
set(SERVER_UT_SOURCES ${SERVER_UT_SOURCES} ${UT_MAIN_SOURCE})

add_executable(commands_ut ${COMMANDS_UT_SOURCES})
#target_include_directories(commands PUBLIC commands)
target_link_libraries(commands_ut gtest)

add_test(NAME commands_ut COMMAND commands_ut)

add_executable(server_ut ${SERVER_UT_SOURCES})
target_link_libraries(server_ut gtest)

add_test(NAME server_ut COMMAND server_ut)
