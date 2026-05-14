#include "model/HeavyVehicle.h"

HeavyVehicle::HeavyVehicle(int id, const std::string& plateNumber, Category category, 
                           double grossVehicleWeight, bool hasTrailer)
    : Vehicle(id, plateNumber, category), 
      grossVehicleWeight(grossVehicleWeight), 
      hasTrailer(hasTrailer) {}

double HeavyVehicle::getGrossVehicleWeight() const { return grossVehicleWeight; }
bool HeavyVehicle::getHasTrailer() const { return hasTrailer; }