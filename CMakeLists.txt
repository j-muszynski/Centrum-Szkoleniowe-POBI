cmake_minimum_required(VERSION 3.20)
cmake_policy(SET CMP0167 NEW)
project(CentrumSzkoleniowe LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wpedantic -Wextra)
set(CMAKE_BUILD_TYPE Debug)

# Rejestrujemy podprojekty
add_subdirectory(library)
add_subdirectory(program)

enable_testing()