#ifndef BUS_H
#define BUS_H

#include "model/HeavyVehicle.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

class Bus : public HeavyVehicle {
private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, [[maybe_unused]] const unsigned int version) {
        ar & boost::serialization::base_object<HeavyVehicle>(*this);
        ar & numberOfSeats;
    }

    int numberOfSeats; // Ile siedzeń

    static Category determineCategory(bool hasTrailer); // Wylicza czy D czy DE

protected:
    Bus() = default;

public:
    Bus(int id, const std::string& plateNumber, double grossVehicleWeight,
        bool hasTrailer, int numberOfSeats);
    ~Bus() override = default;

    int getNumberOfSeats() const;
    std::string getVehicleInfo() const override;
    double getBaseHourlyCost() const override; // zgodnie z UML
};

#endif