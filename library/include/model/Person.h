#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <vector>
#include <memory>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "model/Address.h"
#include "model/Category.h"

class Person {
private:
    int id;
    std::string firstName;
    std::string lastName;             // Imie nazwisko
    const std::string pesel;          // Pesel
    boost::gregorian::date birthDate; // Data urodzenia
    std::shared_ptr<Address> address; // Adres
    std::vector<Category> licenses;   // Jakie prawko już ma

public:
    Person(int id, const std::string& firstName, const std::string& lastName,
           const std::string& pesel, const boost::gregorian::date& birthDate,
           std::shared_ptr<Address> address);
    virtual ~Person() = default;

    int getId() const;
    const std::string& getFirstName() const;
    const std::string& getLastName() const;
    const std::string& getPesel() const;
    const boost::gregorian::date& getBirthDate() const;
    std::shared_ptr<Address> getAddress() const;
    const std::vector<Category>& getLicenses() const;

    void setFirstName(const std::string& firstName);
    void setLastName(const std::string& lastName);
    void setAddress(std::shared_ptr<Address> address);
    void addLicense(Category category);

    virtual std::string getInfo() const = 0;
};

#endif