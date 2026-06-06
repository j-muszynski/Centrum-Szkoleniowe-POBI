#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <vector>
#include <memory>
#include "model/Address.h"
#include "model/Category.h"
#include "typedefs.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>
//#include <boost/date_time/gregorian/greg_serialize.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/serialization/assume_abstract.hpp>

/**
 * @class Person
 * @brief Abstrakcyjna klasa bazowa reprezentująca osobę w systemie.
 * * Zawiera wspólne atrybuty dla kursantów i instruktorów (m.in. dane personalne, adres).
 */
class Person {
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, [[maybe_unused]] const unsigned int version) {
        ar & id;
        ar & firstName;
        ar & lastName;
        ar & pesel;
        ar & birthDate;
        ar & address;
    }
private:
    int id = -1;
    std::string firstName;
    std::string lastName;             // Imie nazwisko
    std::string pesel;                // Pesel (jednak bez const bo nie dziala zapis do pliku tak jak mowiles)
    boost::gregorian::date birthDate; // Data urodzenia
    AddressPtr address; // Adres
    std::vector<Category> licenses;   // Jakie prawko już ma

protected:
    Person() = default;

public:

    /**
     * @brief Konstruktor obiektu Person.
     * @param id Unikalny identyfikator.
     * @param firstName Imię.
     * @param lastName Nazwisko.
     * @param pesel Numer PESEL.
     * @param birthDate Data urodzenia.
     * @param address Wskaźnik na adres (AddressPtr).
     */
    Person(int id, const std::string& firstName, const std::string& lastName,
           const std::string& pesel, const boost::gregorian::date& birthDate,
           AddressPtr address);
    virtual ~Person() = default;

    int getId() const;
    const std::string& getFirstName() const;
    const std::string& getLastName() const;
    const std::string& getPesel() const;
    const boost::gregorian::date& getBirthDate() const;
    AddressPtr getAddress() const;
    const std::vector<Category>& getLicenses() const;

    void setFirstName(const std::string& firstName);
    void setLastName(const std::string& lastName);
    void setAddress(AddressPtr address);
    void addLicense(Category category);

    /**
     * @brief Zwraca sformatowane informacje o osobie (metoda wirtualna).
     * @return String z informacjami.
     */
    virtual std::string getInfo() const = 0;
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(Person) // Bo boost placze ze abstrakcyjna

#endif