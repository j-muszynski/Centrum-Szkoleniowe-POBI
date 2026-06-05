#include "model/Instructor.h"
#include <boost/serialization/export.hpp>
#include <boost/date_time/gregorian/greg_serialize.hpp>
BOOST_CLASS_EXPORT(Instructor)

Instructor::Instructor(int id, const std::string& firstName, const std::string& lastName,
                       const std::string& pesel, const boost::gregorian::date& birthDate,
                       AddressPtr address)
    : Person(id, firstName, lastName, pesel, birthDate, address) {}

const std::vector<Category>& Instructor::getQualifications() const { return qualifications; }

void Instructor::addQualification(Category category) {
    qualifications.push_back(category);
}

std::string Instructor::getInfo() const {
    return "Instruktor: " + getFirstName() + " " + getLastName();
}