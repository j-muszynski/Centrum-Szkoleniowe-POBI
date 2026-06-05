#include "model/Bus.h"

Category Bus::determineCategory(bool hasTrailer) {
    if (hasTrailer == true) {
        return Category::DE;
    } else {
        return Category::D;
    }
} // Tu mamy wyliczenie D czy DE

Bus::Bus(int id, const std::string& plateNumber, double grossVehicleWeight, 
         bool hasTrailer, int numberOfSeats)
    : HeavyVehicle(id, plateNumber, determineCategory(hasTrailer), grossVehicleWeight, hasTrailer), 
      numberOfSeats(numberOfSeats) {}

int Bus::getNumberOfSeats() const { return numberOfSeats; }

std::string Bus::getVehicleInfo() const {
    return "Autobus [ID: " + std::to_string(getId()) + 
           ", Rejestracja: " + getPlateNumber() + 
           ", Miejsca: " + std::to_string(numberOfSeats) + "]";
}
    double Bus::getBaseHourlyCost() const {
    return 180.0; // Stawka dla autobusu
}