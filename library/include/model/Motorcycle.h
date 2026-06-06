#ifndef MOTORCYCLE_H
#define MOTORCYCLE_H

#include "model/Vehicle.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

/**
 * @class Motorcycle
 * @brief Klasa reprezentująca motocykl w zasobach ośrodka.
 *
 * Implementuje logikę przyznawania wymaganej kategorii prawa jazdy na
 * podstawie pojemności silnika i mocy maszyny.
 */
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

    /**
     * @brief Automatycznie determinuje wymaganą kategorię.
     * @param engineCapacity Pojemność skokowa.
     * @param power Moc w kW.
     * @return Odpowiednia kategoria (AM, A1, A2, A).
     */
    static Category determineCategory(int engineCapacity, double power); // Liczy poj. i moc

protected:
    Motorcycle() = default;
public:
    /**
     * @brief Konstruktor motocykla.
     * @param id Unikalny identyfikator.
     * @param plateNumber Numer rejestracyjny.
     * @param engineCapacity Pojemność silnika w cm3.
     * @param power Moc w kW.
     */
    Motorcycle(int id, const std::string& plateNumber, int engineCapacity, double power);
    ~Motorcycle() override = default;

    /** @brief Zwraca pojemność skokową silnika. */
    int getEngineCapacity() const;
    /** @brief Zwraca moc motocykla. */
    double getPower() const;

    std::string getVehicleInfo() const override;
    double getBaseHourlyCost() const override; // zgodnie z UML
};

#endif