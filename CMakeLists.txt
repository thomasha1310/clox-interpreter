cmake_minimum_required(VERSION 3.10)
project(clox C)

include_directories(src)

file(GLOB SOURCES "src/*.c")

add_executable(clox ${SOURCES})

target_compile_options(clox PRIVATE -g -Wall)