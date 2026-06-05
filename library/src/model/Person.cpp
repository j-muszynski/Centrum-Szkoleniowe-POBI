#include "model/Person.h"
#include <boost/serialization/vector.hpp>
#include <boost/date_time/gregorian/greg_serialize.hpp>

Person::Person(int id, const std::string& firstName, const std::string& lastName,
               const std::string& pesel, const boost::gregorian::date& birthDate,
               AddressPtr address)
    : id(id), firstName(firstName), lastName(lastName), pesel(pesel),
      birthDate(birthDate), address(address) {}

int Person::getId() const { return id; }

const std::string& Person::getFirstName() const { return firstName; }

const std::string& Person::getLastName() const { return lastName; }

const std::string& Person::getPesel() const { return pesel; }

const boost::gregorian::date& Person::getBirthDate() const { return birthDate; }

AddressPtr Person::getAddress() const { return address; }

const std::vector<Category>& Person::getLicenses() const { return licenses; }

// Settery

void Person::setFirstName(const std::string& newFirstName) { this->firstName = newFirstName; }

void Person::setLastName(const std::string& newLastName) { this->lastName = newLastName; }

void Person::setAddress(AddressPtr newAddress) { this->address = newAddress; }

void Person::addLicense(Category category) { licenses.push_back(category); }
