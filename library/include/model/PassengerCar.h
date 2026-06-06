#ifndef PASSENGERCAR_H
#define PASSENGERCAR_H

#include "model/Vehicle.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

/**
 * @class PassengerCar
 * @brief Reprezentuje standardowy samochód osobowy (kategoria B).
 */
class PassengerCar : public Vehicle {
private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, [[maybe_unused]] const unsigned int version) {
        ar & boost::serialization::base_object<Vehicle>(*this);
    }

protected:
    PassengerCar() = default;

public:
    /**
     * @brief Konstruktor auta osobowego.
     * Kategoria B jest tu przypisywana na stałe z góry.
     * @param id Unikalny identyfikator.
     * @param plateNumber Numer rejestracyjny.
     */
    PassengerCar(int id, const std::string& plateNumber);
    ~PassengerCar() override = default;

    std::string getVehicleInfo() const override;
    double getBaseHourlyCost() const override; // zgodnie z UML
};

#endif