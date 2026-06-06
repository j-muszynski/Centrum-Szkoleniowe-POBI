#ifndef TRUCK_H
#define TRUCK_H

#include "model/HeavyVehicle.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

/**
 * @class Truck
 * @brief Klasa reprezentująca pojazd ciężarowy.
 *
 * Dziedziczy po HeavyVehicle. Określa swoją kategorię (C lub CE)
 * na podstawie obecności przyczepy w zestawie.
 */
class Truck : public HeavyVehicle {
private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, [[maybe_unused]] const unsigned int version) {
        ar & boost::serialization::base_object<HeavyVehicle>(*this);
        ar & cargoCapacity;
    }

    double cargoCapacity;  // Ładowność

    /**
     * @brief Ustala czy wymagane jest C, czy C+E.
     * @param hasTrailer Status obecności przyczepy.
     * @return Kategoria z enuma (C lub CE).
     */
    static Category determineCategory(bool hasTrailer); // Liczy czy C czy C+E, jak w busie
protected:
    Truck() = default;

public:
    /**
     * @brief Konstruktor pojazdu ciężarowego.
     * @param id Unikalny ID.
     * @param plateNumber Rejestracja.
     * @param grossVehicleWeight Masa w tonach.
     * @param hasTrailer Czy ciągnie przyczepę.
     * @param cargoCapacity Ładowność.
     */
    Truck(int id, const std::string& plateNumber, double grossVehicleWeight,
          bool hasTrailer, double cargoCapacity);
    ~Truck() override = default;

    /** @brief Zwraca pojemność ładunkową / ładowność ciężarówki. */
    double getCargoCapacity() const;
    std::string getVehicleInfo() const override;
    double getBaseHourlyCost() const override; // zgodnie z UML
};

#endif