#ifndef INSTRUCTOR_H
#define INSTRUCTOR_H

#include "model/Person.h"
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/split_member.hpp>
#include "typedefs.h"
#include <boost/serialization/access.hpp>

/**
 * @class Instructor
 * @brief Klasa reprezentująca pracownika - instruktora jazdy.
 *
 * Dziedziczy po klasie Person. Posiada wektor kwalifikacji, który określa
 * z jakich kategorii instruktor może prowadzić szkolenia.
 */
class Instructor : public Person {
private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, [[maybe_unused]] const unsigned int version) {
        ar & boost::serialization::base_object<Person>(*this);
        ar & qualifications;
    }

    std::vector<Category> qualifications; // Czego może uczyć

protected:
    Instructor() = default;

public:
    /**
     * @brief Konstruktor instruktora.
     * @param id Unikalny identyfikator.
     * @param firstName Imię.
     * @param lastName Nazwisko.
     * @param pesel Numer PESEL.
     * @param birthDate Data urodzenia.
     * @param address Wskaźnik na adres przypisany do instruktora.
     */
    Instructor(int id, const std::string& firstName, const std::string& lastName,
               const std::string& pesel, const boost::gregorian::date& birthDate,
               AddressPtr address);
    ~Instructor() override = default;

    /**
     * @brief Pobiera listę kwalifikacji instruktora.
     * @return Wektor zawierający przypisane kategorie.
     */
    const std::vector<Category>& getQualifications() const;

    /**
     * @brief Dodaje nową kwalifikację do profilu instruktora.
     * @param category Kategoria, której instruktor będzie mógł nauczać.
     */
    void addQualification(Category category);
    /**
         * @brief Generuje ciąg znaków opisujący instruktora i jego uprawnienia.
         * @return Sformatowany String.
         */
    std::string getInfo() const override;
};

#endif