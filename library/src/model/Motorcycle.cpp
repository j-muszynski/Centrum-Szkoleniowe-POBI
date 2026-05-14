#include "model/Motorcycle.h"

Category Motorcycle::determineCategory(int engineCapacity, double power) {
    if (engineCapacity <= 50) return Category::AM;
    if (engineCapacity <= 125 && power <= 11.0) return Category::A1;
    if (power <= 35.0) return Category::A2;
    return Category::A;
} // Liczy jakei prawko zgodnie z naszym UML

Motorcycle::Motorcycle(int id, const std::string& plateNumber, int engineCapacity, double power)
    : Vehicle(id, plateNumber, determineCategory(engineCapacity, power)),
      engineCapacity(engineCapacity), power(power) {}

int Motorcycle::getEngineCapacity() const { return engineCapacity; }
double Motorcycle::getPower() const { return power; }

std::string Motorcycle::getVehicleInfo() const {
    return "Motocykl [ID: " + std::to_string(getId()) +
           ", Rejestracja: " + getPlateNumber() +
           ", Pojemnosc: " + std::to_string(engineCapacity) + "ccm]";
}