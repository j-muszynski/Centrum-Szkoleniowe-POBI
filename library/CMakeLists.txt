cmake_minimum_required(VERSION 3.20)
project(Library)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Werror)
endif()

set(SOURCE_FILES
        src/model/Address.cpp
        src/model/Vehicle.cpp
        src/model/PassengerCar.cpp
        src/model/HeavyVehicle.cpp
        src/model/Truck.cpp
        src/model/Bus.cpp
        src/model/Motorcycle.cpp
        src/model/Person.cpp
        src/model/Student.cpp
        src/model/Instructor.cpp
        src/model/Course.cpp
        src/model/DrivingSession.cpp
)

add_library(Library ${SOURCE_FILES})

target_include_directories(Library PUBLIC include)

enable_testing()

find_package(Boost 1.80.0 COMPONENTS "unit_test_framework" "date_time" REQUIRED)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${Boost_INCLUDE_DIRS}
)

target_link_libraries(Library PUBLIC Boost::date_time)
set(SOURCE_TEST_FILES
        test/master.cpp
        # test/AddressTest.cpp tu bedzie reszta
)
add_executable(LibraryTester ${SOURCE_TEST_FILES})

target_link_libraries(LibraryTester
        Library
        Boost::unit_test_framework
        Boost::date_time
)

add_test(LibraryTest LibraryTester
        --report_level=detailed
        --log_level=all
        --color_output=yes
)

add_custom_target(check ${CMAKE_COMMAND} -E env CTEST_OUTPUT_ON_FAILURE=1 BOOST_TEST_LOG_LEVEL=all
        ${CMAKE_CTEST_COMMAND} -C $<CONFIGURATION> --verbose
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR})