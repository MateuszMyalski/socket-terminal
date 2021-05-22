set(INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/inc;
                 ${CMAKE_SOURCE_DIR}/src;
                 ${CMAKE_SOURCE_DIR}
                 CACHE INTERNAL "")

set(COMPILE_FLAGS "-g"
                  "-Wall" 
                  "-Wpedantic"
                  "-Wextra"
                CACHE INTERNAL "")