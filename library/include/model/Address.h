#ifndef ADDRESS_H
#define ADDRESS_H

#include <string>
#include <boost/serialization/access.hpp>

/**
 * @class Address
 * @brief Reprezentuje dane adresowe uczestników systemu (kursantów oraz instruktorów).
 */
class Address {
private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, [[maybe_unused]] const unsigned int version) {
        ar & city;
        ar & street;
        ar & number;
    }
    std::string city;   // Miasto
    std::string street; // Ulica
    std::string number; // Numer
protected:
    Address() = default;

public:
    /**
     * @brief Konstruktor adresu.
     * @param city Miejscowość.
     * @param street Ulica.
     * @param number Numer bloku/lokalu.
     */
    Address(const std::string& city, const std::string& street, const std::string& number);

    ~Address() = default;

    /** @brief Pobiera przypisane miasto. */
    const std::string& getCity() const;
    /** @brief Pobiera ulicę. */
    const std::string& getStreet() const;
    /** @brief Pobiera numer na ulicy. */
    const std::string& getNumber() const;

    /** @brief Ustawia nową wartość dla miasta. */
    void setCity(const std::string& city);
    /** @brief Ustawia nową wartość dla ulicy. */
    void setStreet(const std::string& street);
    /** @brief Zmienia przypisany numer domu/mieszkania. */
    void setNumber(const std::string& number);

    /**
     * @brief Skleja ciągi znakowe miasta, ulicy i numeru do celów raportów.
     * @return Czysty odczyt pełnego adresu string.
     */
    std::string getAddressInfo() const;
};

#endif