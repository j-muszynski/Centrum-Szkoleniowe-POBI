#include "model/Vehicle.h"

Vehicle::Vehicle(int id, const std::string& plateNumber, Category category)
    : id(id), plateNumber(plateNumber), category(category) {}

int Vehicle::getId() const { return id; }
const std::string& Vehicle::getPlateNumber() const { return plateNumber; }
Category Vehicle::getCategory() const { return category; }

void Vehicle::setPlateNumber(const std::string& plateNumber) { this->plateNumber = plateNumber; }
void Vehicle::setCategory(Category category) { this->category = category; }