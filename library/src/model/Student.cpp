#include "model/Student.h"

Student::Student(int id, const std::string& firstName, const std::string& lastName,
                 const std::string& pesel, const boost::gregorian::date& birthDate,
                 std::shared_ptr<Address> address, const std::string& pkkNumber)
    : Person(id, firstName, lastName, pesel, birthDate, address),
      pkkNumber(pkkNumber), isReadyForExam(false) {} // Domyślnie uczeń nie jest gotowy na egzamin

const std::string& Student::getPkkNumber() const { return pkkNumber; }
bool Student::getIsReadyForExam() const { return isReadyForExam; }

void Student::setPkkNumber(const std::string& pkkNumber) { this->pkkNumber = pkkNumber; }
void Student::setReadyForExam(bool isReady) { isReadyForExam = isReady; }

std::string Student::getInfo() const {
    return "Kursant: " + getFirstName() + " " + getLastName() + " (PKK: " + pkkNumber + ")";
}