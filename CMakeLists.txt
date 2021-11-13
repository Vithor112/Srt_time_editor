cmake_minimum_required(VERSION 3.0.0)
project(srt_time_editor VERSION 0.1.0)

include(CTest)
enable_testing()
add_library(auxFuncs
STATIC
    src/libs/time_funcs.c
    src/libs/error_funcs.c
    src/libs/IO_funcs.c
    src/libs/macros_and_funcs.h)
add_executable(srt_time_editor src/main.c)
target_link_libraries(srt_time_editor auxFuncs)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
