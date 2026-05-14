cmake_minimum_required(VERSION 3.20)
project(Program)

set(SOURCE_FILES src/main.cpp)

add_executable(Program ${SOURCE_FILES})

target_link_libraries(Program Library)

target_include_directories(Program PUBLIC include)