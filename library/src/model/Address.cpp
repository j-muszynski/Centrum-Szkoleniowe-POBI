#include "model/Address.h"

Address::Address(const std::string& city, const std::string& street, const std::string& number)
    : city(city), street(street), number(number) {}

const std::string& Address::getCity() const { return city; }
const std::string& Address::getStreet() const { return street; }
const std::string& Address::getNumber() const { return number; }

void Address::setCity(const std::string& city) { this->city = city; }
void Address::setStreet(const std::string& street) { this->street = street; }
void Address::setNumber(const std::string& number) { this->number = number; }

std::string Address::getAddressInfo() const {
    return city + ", " + street + " " + number;
}