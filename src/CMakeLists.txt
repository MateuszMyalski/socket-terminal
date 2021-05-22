add_subdirectory(server)
add_subdirectory(network-hal)
add_subdirectory(commands)
add_subdirectory(utils)

add_library(libcommands ${COMMANDS_LIB_SOURCE_FILES})
target_include_directories(libcommands PRIVATE ${INCLUDE_DIRS})

add_library(libnetworkinghal-linux ${LINUX_LIB_SOURCE_FILES})
target_include_directories(libnetworkinghal-linux PRIVATE ${INCLUDE_DIRS})

#add_library(libnetworkinghal-win ${WIN_LIB_SOURCE_FILES})

add_library(libserver ${SERVER_LIB_SOURCE_FILES})
target_include_directories(libserver PRIVATE ${INCLUDE_DIRS})
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(libserver PRIVATE Threads::Threads)