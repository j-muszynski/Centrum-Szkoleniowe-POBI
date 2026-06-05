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
    std::string info = "Instruktor [ID: " + std::to_string(getId()) + "] "
                        + getFirstName() + " " + getLastName() + " | Kategorie: ";

    if (qualifications.empty()) {
        info += "Brak";
    } else {
        for (const auto& cat : qualifications) {
            if (cat == Category::AM) info += "AM ";
            else if (cat == Category::A1) info += "A1 ";
            else if (cat == Category::A2) info += "A2 ";
            else if (cat == Category::A) info += "A ";
            else if (cat == Category::B) info += "B ";
            else if (cat == Category::BE) info += "BE ";
            else if (cat == Category::C) info += "C ";
            else if (cat == Category::CE) info += "CE ";
            else if (cat == Category::D) info += "D ";
            else if (cat == Category::DE) info += "DE ";
        }
    }
    return info;
}