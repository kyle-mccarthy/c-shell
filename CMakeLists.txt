cmake_minimum_required (VERSION 2.8)
project (libSoneFiveGP)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -std=c99 -D_XOPEN_SOURCE=500 -Wall -l readline")

include_directories(${CMAKE_CURRENT_DIRECTORY}/include)

add_library(SoneFiveGP SHARED src/shell_utils.c)

install(TARGETS SoneFiveGP DESTINATION lib)
install(FILES include/shell_utils.h DESTINATION include)

add_executable (test_GP src/osShell.c)
target_link_libraries (test_GP SoneFiveGP)