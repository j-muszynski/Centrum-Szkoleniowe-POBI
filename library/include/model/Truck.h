#ifndef TRUCK_H
#define TRUCK_H

#include "model/HeavyVehicle.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
class Truck : public HeavyVehicle {
private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, [[maybe_unused]] const unsigned int version) {
        ar & boost::serialization::base_object<HeavyVehicle>(*this);
        ar & cargoCapacity;
    }

    double cargoCapacity;  // Ładowność

    static Category determineCategory(bool hasTrailer); // Liczy czy C czy C+E, jak w busie
protected:
    Truck() = default;

public:
    Truck(int id, const std::string& plateNumber, double grossVehicleWeight,
          bool hasTrailer, double cargoCapacity);
    ~Truck() override = default;

    double getCargoCapacity() const;
    std::string getVehicleInfo() const override;
    double getBaseHourlyCost() const override; // zgodnie z UML
};

#endif