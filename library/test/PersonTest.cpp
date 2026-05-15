#include <boost/test/unit_test.hpp>
#include "model/Student.h"
#include "model/Instructor.h"
#include "model/Address.h"

BOOST_AUTO_TEST_SUITE(PersonTestSuite)

BOOST_AUTO_TEST_CASE(StudentInitializationTest) {
    auto address = std::make_shared<Address>("Lodz", "Politechniki", "1");
    boost::gregorian::date birthDate(2000, 5, 10);
    
    Student student(1, "Jan", "Kowalski", "00251012345", birthDate, address, "PKK123");

    BOOST_TEST(student.getFirstName() == "Jan");
    BOOST_TEST(student.getPkkNumber() == "PKK123");
    // Zgodnie z naszą logiką biznesową, nowy student nie jest gotowy na egzamin
    BOOST_TEST(student.getIsReadyForExam() == false); 
    
    // Sprawdzamy czy powiązanie z adresem jest okej
    BOOST_TEST(student.getAddress()->getCity() == "Lodz");
}

BOOST_AUTO_TEST_CASE(InstructorQualificationsTest) {
    auto address = std::make_shared<Address>("Zgierz", "Dluga", "5");
    boost::gregorian::date birthDate(1980, 1, 1);
    
    Instructor instructor(2, "Anna", "Nowak", "80010112345", birthDate, address);
    
    // Dodajemy uprawnienia instruktorowi
    instructor.addQualification(Category::B);
    instructor.addQualification(Category::A);
    
    BOOST_TEST(instructor.getQualifications().size() == 2);
    BOOST_TEST(instructor.getQualifications()[0] == Category::B);
}

BOOST_AUTO_TEST_SUITE_END()