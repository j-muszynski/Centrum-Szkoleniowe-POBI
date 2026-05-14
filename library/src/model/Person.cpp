#include "model/Person.h"

Person::Person(int id, const std::string& firstName, const std::string& lastName,
               const std::string& pesel, const boost::gregorian::date& birthDate,
               std::shared_ptr<Address> address)
    : id(id), firstName(firstName), lastName(lastName), pesel(pesel),
      birthDate(birthDate), address(address) {}

int Person::getId() const { return id; }
const std::string& Person::getFirstName() const { return firstName; }
const std::string& Person::getLastName() const { return lastName; }
const std::string& Person::getPesel() const { return pesel; }
const boost::gregorian::date& Person::getBirthDate() const { return birthDate; }
std::shared_ptr<Address> Person::getAddress() const { return address; }
const std::vector<Category>& Person::getLicenses() const { return licenses; }

// Settery

void Person::setFirstName(const std::string& firstName) { this->firstName = firstName; }
void Person::setLastName(const std::string& lastName) { this->lastName = lastName; }
void Person::setAddress(std::shared_ptr<Address> address) { this->address = address; }
void Person::addLicense(Category category) { licenses.push_back(category); }