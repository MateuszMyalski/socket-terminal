add_subdirectory(ut_main)
add_subdirectory(commands)
add_subdirectory(server)

include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(COMMANDS_UT_SOURCES ${COMMANDS_UT_SOURCES} ${UT_MAIN_SOURCE})
set(SERVER_UT_SOURCES ${SERVER_UT_SOURCES} ${UT_MAIN_SOURCE})

add_executable(commands_ut ${COMMANDS_UT_SOURCES})
target_include_directories(commands_ut PRIVATE ${INCLUDE_DIRS})
target_link_libraries(commands_ut libcommands gtest)
target_compile_options(commands_ut PRIVATE ${COMPILE_FLAGS})
add_test(NAME commands_ut COMMAND commands_ut)

add_executable(server_ut ${SERVER_UT_SOURCES})
target_include_directories(server_ut PRIVATE ${INCLUDE_DIRS})
target_link_libraries(server_ut libserver gtest)
target_compile_options(commands_ut PRIVATE ${COMPILE_FLAGS})
add_test(NAME server_ut COMMAND server_ut)
