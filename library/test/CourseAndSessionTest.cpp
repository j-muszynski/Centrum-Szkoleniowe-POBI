#include <boost/test/unit_test.hpp>
#include "model/Course.h"
#include "model/DrivingSession.h"
#include "model/Student.h"
#include "model/Instructor.h"
#include "model/PassengerCar.h"
#include "model/Address.h"

BOOST_AUTO_TEST_SUITE(CourseAndSessionTestSuite)

BOOST_AUTO_TEST_CASE(CourseInitializationTest) {
    auto address = std::make_shared<Address>("Lodz", "Politechniki", "1");
    boost::gregorian::date date(1990, 1, 1);
    
    auto instructor = std::make_shared<Instructor>(1, "Jan", "Instruktor", "111", date, address);
    auto student = std::make_shared<Student>(2, "Tomasz", "Kursant", "222", date, address, "PKK1");
    
    // Tworzymy kurs
    Course course(100, Category::B, instructor, student); // usunelismy cene = 2500.0, bo po usuniecu
                                                                  // *price mamy 5 argumentow a powinny byc 4
    // Sprawdzamy połączenia
    // tu bylo BOOST_TEST(course.getPrice() == 2500.0); ale usunelismy (ten sam powod co linia 19)
    BOOST_TEST(course.getInstructor()->getLastName() == "Instruktor");
    BOOST_TEST(course.getStudent()->getLastName() == "Kursant");
    
    // Kurs domyślnie nie jest zakończony
    BOOST_TEST(course.getIsFinished() == false);
}

BOOST_AUTO_TEST_CASE(DrivingSessionTimeAndStatusTest) {
    auto address = std::make_shared<Address>("Lodz", "A", "1");
    boost::gregorian::date date(1990, 1, 1);
    
    auto instructor = std::make_shared<Instructor>(1, "I", "N", "1", date, address);
    auto student = std::make_shared<Student>(2, "S", "T", "2", date, address, "PKK");
    auto car = std::make_shared<PassengerCar>(1, "EL123");
    
    // Ustawiamy czas jazdy (dzisiaj 14:00 - 16:00) za pomocą posix_time
    boost::posix_time::ptime start(boost::gregorian::day_clock::local_day(), boost::posix_time::hours(14));
    boost::posix_time::ptime end(boost::gregorian::day_clock::local_day(), boost::posix_time::hours(16));

    DrivingSession session(1, start, end, instructor, student, 100, car);

    BOOST_TEST(session.getStatus() == "Scheduled");
    BOOST_TEST(session.getVehicle()->getPlateNumber() == "EL123");
}

BOOST_AUTO_TEST_SUITE_END()