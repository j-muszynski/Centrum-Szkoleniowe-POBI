#ifndef TRUCK_H
#define TRUCK_H

#include "model/HeavyVehicle.h"

class Truck : public HeavyVehicle {
private:
    double cargoCapacity;  // Ładowność

    static Category determineCategory(bool hasTrailer); // Liczy czy C czy C+E, jak w busie

public:
    Truck(int id, const std::string& plateNumber, double grossVehicleWeight,
          bool hasTrailer, double cargoCapacity);
    ~Truck() override = default;

    double getCargoCapacity() const;
    std::string getVehicleInfo() const override;
};

#endif