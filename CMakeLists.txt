cmake_minimum_required(VERSION 3.20)

enable_testing()

# 1. Konfiguracja pod maca
if(APPLE AND EXISTS "/opt/homebrew")
    set(CMAKE_PREFIX_PATH "/opt/homebrew")
    set(Boost_ROOT "/opt/homebrew")
    #set(Boost_USE_STATIC_LIBS OFF)       # Wymuszenie bezpiecznych plikow .a zamiast .dylib
    set(Boost_USE_MULTITHREADED ON)
endif()

if(POLICY CMP0167) #dodalem ifa bo mam starego cmake'a
    cmake_policy(SET CMP0167 NEW)
endif()

project(CentrumSzkoleniowe LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wpedantic -Wextra)
set(CMAKE_BUILD_TYPE Debug)

# 2. szukanie Boosta globalnie - raz dla całego projektu
set(Boost_NO_BOOST_CMAKE ON)
find_package(Boost 1.80.0 COMPONENTS "unit_test_framework" "date_time" "serialization" REQUIRED)

if(APPLE)
    add_definitions(-DBOOST_DATE_TIME_NO_LIB)
    add_definitions(-DBOOST_SERIALIZATION_NO_LIB)
endif()

# Rejestrujemy podprojekty
add_subdirectory(library)
add_subdirectory(program)