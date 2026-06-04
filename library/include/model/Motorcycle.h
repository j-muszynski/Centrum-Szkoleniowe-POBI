#ifndef MOTORCYCLE_H
#define MOTORCYCLE_H

#include "model/Vehicle.h"

class Motorcycle : public Vehicle {
private:
    int engineCapacity;   // Pojemność silnika
    double power;         // P=W/t

    static Category determineCategory(int engineCapacity, double power); // Liczy poj. i moc
public:
    Motorcycle(int id, const std::string& plateNumber, int engineCapacity, double power);
    ~Motorcycle() override = default;

    int getEngineCapacity() const;
    double getPower() const;

    std::string getVehicleInfo() const override;
    double getBaseHourlyCost() const override; // zgodnie z UML
};

#endif