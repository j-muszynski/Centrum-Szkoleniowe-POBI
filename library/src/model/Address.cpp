#include "model/Address.h"
#include <boost/serialization/string.hpp>

Address::Address(const std::string& city, const std::string& street, const std::string& number)
    : city(city), street(street), number(number) {}

const std::string& Address::getCity() const { return city; }
const std::string& Address::getStreet() const { return street; }
const std::string& Address::getNumber() const { return number; }

void Address::setCity(const std::string& newCity) { this->city = newCity; }
void Address::setStreet(const std::string& newStreet) { this->street = newStreet; }
void Address::setNumber(const std::string& newNumber) { this->number = newNumber; }

std::string Address::getAddressInfo() const {
    return city + ", " + street + " " + number;
}