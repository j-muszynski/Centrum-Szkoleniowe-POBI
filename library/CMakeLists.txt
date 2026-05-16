cmake_minimum_required(VERSION 3.20)
project(Library)

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
        src/repositories/CourseRepository.cpp
        src/repositories/DrvingSessionRepository.cpp
        src/repositories/InstructorRepository.cpp
        src/repositories/StudentRepository.cpp
        src/repositories/VehicleRepository.cpp
)

add_library(Library STATIC ${SOURCE_FILES})

target_include_directories(Library PUBLIC include)
target_include_directories(Library PRIVATE ${Boost_INCLUDE_DIRS})

target_link_libraries(Library PUBLIC
        Boost::date_time
        Boost::serialization
)

# Konfiguracja testera
set(SOURCE_TEST_FILES test/master.cpp)
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