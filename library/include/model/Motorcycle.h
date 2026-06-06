#ifndef MOTORCYCLE_H
#define MOTORCYCLE_H

#include "model/Vehicle.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

class Motorcycle : public Vehicle {
private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, [[maybe_unused]] const unsigned int version) {
        ar & boost::serialization::base_object<Vehicle>(*this);
        ar & engineCapacity;
        ar & power;
    }

    int engineCapacity;   // Pojemność silnika
    double power;         // P=W/t

    static Category determineCategory(int engineCapacity, double power); // Liczy poj. i moc

protected:
    Motorcycle() = default;
public:
    Motorcycle(int id, const std::string& plateNumber, int engineCapacity, double power);
    ~Motorcycle() override = default;

    int getEngineCapacity() const;
    double getPower() const;

    std::string getVehicleInfo() const override;
    double getBaseHourlyCost() const override; // zgodnie z UML
};

#endif