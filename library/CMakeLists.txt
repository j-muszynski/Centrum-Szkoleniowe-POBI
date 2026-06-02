    cmake_minimum_required(VERSION 3.20)
    project(Library)

    if(NOT Boost_FOUND)
        find_package(Boost REQUIRED COMPONENTS date_time serialization unit_test_framework)
    endif()

    set(SOURCE_FILES
            # ------ Header files ------
            include/repositories/IRepository.h
            include/repositories/CourseRepository.h
            include/repositories/DrivingSessionRepository.h
            include/repositories/InstructorRepository.h
            include/repositories/StudentRepository.h
            include/repositories/VehicleRepository.h
            include/repositories/IDataStorage.h
            include/repositories/AbstractFileRepository.h
            include/repositories/FileStorage.h
            include/typedefs.h

            # ------ Source files ------
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
            src/repositories/DrivingSessionRepository.cpp
            src/repositories/InstructorRepository.cpp
            src/repositories/StudentRepository.cpp
            src/repositories/VehicleRepository.cpp
    )

    add_library(Library STATIC ${SOURCE_FILES})

    target_include_directories(Library PUBLIC include)

    if (APPLE)
        target_include_directories(Library PUBLIC "/opt/homebrew/include")
    endif ()
    target_link_libraries(Library PUBLIC Boost::date_time Boost::serialization)

    target_link_libraries(Library PUBLIC
            Boost::date_time
            Boost::serialization
    )

    # Konfiguracja testera
    set(SOURCE_TEST_FILES test/master.cpp
            test/AbstractFileRepositoryTest.cpp
            test/FileStorageTest.cpp)

    add_executable(LibraryTester ${SOURCE_TEST_FILES})

    target_include_directories(LibraryTester PRIVATE
            include
    )
    if(APPLE)
        target_include_directories(LibraryTester PRIVATE "/opt/homebrew/include")
    endif()


    target_link_libraries(LibraryTester PRIVATE
            Library
            Boost::unit_test_framework
            Boost::date_time
            Boost::serialization
    )

    add_test(LibraryTest LibraryTester
            --report_level=detailed
            --log_level=all
            --color_output=yes
    )

    add_custom_target(check ${CMAKE_COMMAND} -E env CTEST_OUTPUT_ON_FAILURE=1 BOOST_TEST_LOG_LEVEL=all
            ${CMAKE_CTEST_COMMAND} -C $<CONFIGURATION> --verbose
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR})