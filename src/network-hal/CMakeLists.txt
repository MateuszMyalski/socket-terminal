add_subdirectory(linux)
add_subdirectory(win)

set(LINUX_LIB_SOURCE_FILES ${LINUX_LIB_SOURCE_FILES}
                            PARENT_SCOPE)

set(WIN_LIB_SOURCE_FILES ${WIN_LIB_SOURCE_FILES}
                         PARENT_SCOPE)
