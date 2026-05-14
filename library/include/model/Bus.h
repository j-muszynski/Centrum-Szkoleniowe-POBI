#ifndef BUS_H
#define BUS_H

#include "model/HeavyVehicle.h"

class Bus : public HeavyVehicle {
private:
    int numberOfSeats; // Ile siedzeń

    static Category determineCategory(bool hasTrailer); // Wylicza czy D czy DE

public:
    Bus(int id, const std::string& plateNumber, double grossVehicleWeight,
        bool hasTrailer, int numberOfSeats);
    ~Bus() override = default;

    int getNumberOfSeats() const;
    std::string getVehicleInfo() const override;
};

#endif