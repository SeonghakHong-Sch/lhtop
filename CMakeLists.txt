cmake_minimum_required(VERSION 3.10)
project(lhtop
        VERSION 0.1.0
        DESCRIPTION "light htop"
        LANGUAGES C)

#library
set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_library(lhtop_lib STATIC
    ${SRC}/display.c
    ${SRC}/process.c
    ${SRC}/stats.c
    ${SRC}/utils.c)

#include
target_include_directories(lhtop_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

#exec file
add_executable(${PROJECT_NAME} main.c)

#link library
target_link_libraries(${PROJECT_NAME} PRIVATE lhtop_lib)