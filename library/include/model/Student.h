#ifndef STUDENT_H
#define STUDENT_H

#include <random>
#include "typedefs.h"
#include "model/Person.h"
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/access.hpp>

/**
 * @class Student
 * @brief Klasa reprezentująca kursanta ośrodka szkolenia kierowców.
 * * Dziedziczy po abstrakcyjnej klasie Person. Przechowuje dodatkowo
 * unikalny numer PKK oraz status gotowości do egzaminu.
 */
class Student : public Person {
private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, [[maybe_unused]] const unsigned int version) {
        ar & boost::serialization::base_object<Person>(*this);
        ar & pkkNumber;
        ar & isReadyForExam;
    }

    std::string pkkNumber;  // Nr Prof Kand na Kierowce
    bool isReadyForExam;    // Czy ma wyjeżdżone h
protected:
    Student() = default;
public:

    /**
     * @brief Konstruktor kursanta.
     * @param id Unikalny identyfikator.
     * @param firstName Imię.
     * @param lastName Nazwisko.
     * @param pesel Numer PESEL.
     * @param birthDate Data urodzenia.
     * @param address Wskaźnik na adres.
     * @param pkkNumber Numer Profilu Kandydata na Kierowcę.
     */
    Student(int id, const std::string& firstName, const std::string& lastName,
            const std::string& pesel, const boost::gregorian::date& birthDate,
            AddressPtr address, const std::string& pkkNumber);
    ~Student() override = default;

    const std::string& getPkkNumber() const;
    bool getIsReadyForExam() const;

    /** @brief Pobiera numer PKK kursanta. */
    void setPkkNumber(const std::string& pkkNumber);

    /**
     * @brief Ustawia status gotowości kursanta do egzaminu państwowego.
     * @param isReady Status gotowości (true/false).
     */
    void setReadyForExam(bool isReady);
    /**
         * @brief Generuje czytelne podsumowanie danych kursanta.
         * @return String zawierający podstawowe informacje o obiekcie.
         */
    std::string getInfo() const override;
};

#endif