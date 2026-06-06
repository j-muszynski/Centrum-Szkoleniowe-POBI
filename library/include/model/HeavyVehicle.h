#ifndef HEAVYVEHICLE_H
#define HEAVYVEHICLE_H

#include "model/Vehicle.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/assume_abstract.hpp>
#include <boost/serialization/base_object.hpp>

class HeavyVehicle : public Vehicle {
private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, [[maybe_unused]] const unsigned int version) {
        ar & boost::serialization::base_object<Vehicle>(*this);
        ar & grossVehicleWeight;
        ar & hasTrailer;
    }

    double grossVehicleWeight;  // Dopuszczalna masa w tonach
    bool hasTrailer;            // Czy przyczepa

protected:
    HeavyVehicle() = default;

public:
    HeavyVehicle(int id, const std::string& plateNumber, Category category,
                 double grossVehicleWeight, bool hasTrailer);
    ~HeavyVehicle() override = default;

    double getGrossVehicleWeight() const;
    bool getHasTrailer() const;
    double getBaseHourlyCost() const override = 0; // zgodnie z UML

};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(HeavyVehicle) // Bo boost placze ze abstrakcyjna

#endif