#ifndef PASSENGERCAR_H
#define PASSENGERCAR_H

#include "model/Vehicle.h"

class PassengerCar : public Vehicle {
public:
    PassengerCar(int id, const std::string& plateNumber);
    ~PassengerCar() override = default;

    std::string getVehicleInfo() const override;
    double getBaseHourlyCost() const override; // zgodnie z UML
};

#endif