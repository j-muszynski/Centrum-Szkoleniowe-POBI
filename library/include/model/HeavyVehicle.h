#ifndef HEAVYVEHICLE_H
#define HEAVYVEHICLE_H

#include "model/Vehicle.h"

class HeavyVehicle : public Vehicle {
private:
    double grossVehicleWeight;  // Dopuszczalna masa w tonach
    bool hasTrailer;            // Czy przyczepa

public:
    HeavyVehicle(int id, const std::string& plateNumber, Category category,
                 double grossVehicleWeight, bool hasTrailer);
    ~HeavyVehicle() override = default;

    double getGrossVehicleWeight() const;
    bool getHasTrailer() const;

};

#endif