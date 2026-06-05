#include "model/PassengerCar.h"

PassengerCar::PassengerCar(int id, const std::string& plateNumber)
    : Vehicle(id, plateNumber, Category::B) {}

std::string PassengerCar::getVehicleInfo() const {
    return "Samochod osobowy [ID: " + std::to_string(getId()) + 
           ", Rejestracja: " + getPlateNumber() + "]";
}
    double PassengerCar::getBaseHourlyCost() const {
    return 80.0; // Przykładowa stawka za godzinę dla osobowego
}
