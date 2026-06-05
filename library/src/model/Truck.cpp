#include "model/Truck.h"

Category Truck::determineCategory(bool hasTrailer) {
    if (hasTrailer == true) {
        return Category::CE;
    } else {
        return Category::C;
    }
} // Wiadomo o co chodzi

Truck::Truck(int id, const std::string& plateNumber, double grossVehicleWeight, 
             bool hasTrailer, double cargoCapacity)
    : HeavyVehicle(id, plateNumber, determineCategory(hasTrailer), grossVehicleWeight, hasTrailer), 
      cargoCapacity(cargoCapacity) {}

double Truck::getCargoCapacity() const { return cargoCapacity; }

std::string Truck::getVehicleInfo() const {
    return "Ciezarowka [ID: " + std::to_string(getId()) + 
           ", Rejestracja: " + getPlateNumber() + 
           ", Ladownosc: " + std::to_string(cargoCapacity) + "t]";
}
    double Truck::getBaseHourlyCost() const {
        return 150.0; // Stawka dla ciężarówek
}