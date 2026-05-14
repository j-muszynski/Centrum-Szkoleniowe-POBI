#ifndef ADDRESS_H
#define ADDRESS_H

#include <string>

class Address {
private:
    std::string city;   // Miasto
    std::string street; // Ulica
    std::string number; // Numer

public:
    // Konstruktor
    Address(const std::string& city, const std::string& street, const std::string& number);

    // Destruktor
    ~Address() = default;

    // Gettery
    const std::string& getCity() const;
    const std::string& getStreet() const;
    const std::string& getNumber() const;

    // Settery
    void setCity(const std::string& city);
    void setStreet(const std::string& street);
    void setNumber(const std::string& number);

    std::string getAddressInfo() const;
};

#endif