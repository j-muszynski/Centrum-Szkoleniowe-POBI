#include "model/Student.h"
#include <boost/serialization/string.hpp>
#include <boost/date_time/gregorian/greg_serialize.hpp>

Student::Student(int id, const std::string& firstName, const std::string& lastName,
                 const std::string& pesel, const boost::gregorian::date& birthDate,
                 AddressPtr address, const std::string& pkkNumber)
    : Person(id, firstName, lastName, pesel, birthDate, address),
      pkkNumber(pkkNumber), isReadyForExam(false) {} // Domyślnie uczeń nie jest gotowy na egzamin

const std::string& Student::getPkkNumber() const { return pkkNumber; }

bool Student::getIsReadyForExam() const { return isReadyForExam; }

void Student::setPkkNumber(const std::string& newPkkNumber) { this->pkkNumber = newPkkNumber; }

void Student::setReadyForExam(bool isReady) { isReadyForExam = isReady; }

std::string Student::getInfo() const {
    return "Kursant: " + getFirstName() + " " + getLastName() + " (PKK: " + pkkNumber + ")";
}
