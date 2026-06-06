#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include "model/Category.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/assume_abstract.hpp>

/**
 * @class Vehicle
 * @brief Abstrakcyjna klasa reprezentująca bazowy pojazd w systemie.
 *
 * Wymusza na klasach pochodnych implementację kalkulacji kosztów
 * oraz generowania informacji tekstowej o konkretnym typie pojazdu.
 */
class Vehicle {
private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, [[maybe_unused]] const unsigned int version) {
        ar & id;
        ar & plateNumber;
        ar & category;
    }

    int id;
    std::string plateNumber;  // Rejestracja
    Category category;        // Jakie prawko

protected:
    Vehicle() = default;
    friend class boost::serialization::access;

public:
    /**
     * @brief Konstruktor obiektu Vehicle.
     * @param id Unikalny identyfikator.
     * @param plateNumber Numer rejestracyjny pojazdu.
     * @param category Wymagana kategoria prawa jazdy.
     */
    Vehicle(int id, const std::string& plateNumber, Category category);


    virtual ~Vehicle() = default;

    /** @brief Zwraca unikalne ID pojazdu. */
    int getId() const;
    /** @brief Zwraca numer rejestracyjny pojazdu. */
    const std::string& getPlateNumber() const;
    /** @brief Zwraca kategorię przypisaną do pojazdu. */
    Category getCategory() const;

    /** @brief Aktualizuje numer rejestracyjny pojazdu. */
    void setPlateNumber(const std::string& plateNumber);
    /** @brief Ustawia nową kategorię wymaganą dla pojazdu. */
    void setCategory(Category category);

    /**
     * @brief Zwraca szczegółowe dane o pojeździe.
     * @return Sformatowany ciąg znaków zależny od typu pojazdu.
     */
    virtual std::string getVehicleInfo() const = 0;

    /**
     * @brief Oblicza i zwraca bazowy koszt wypożyczenia/godziny jazdy.
     * @return Koszt (double).
     */
    virtual double getBaseHourlyCost() const = 0;
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(Vehicle) // Bo boost placze ze klasa abstrakcyjna
#endif