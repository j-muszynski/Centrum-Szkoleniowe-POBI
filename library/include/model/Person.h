#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <vector>
#include <memory>
#include "model/Address.h"
#include "model/Category.h"

#include <boost/serialization/access.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/date_time/gregorian/greg_serialize.hpp>

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
    int id;
    std::string firstName;
    std::string lastName;             // Imie nazwisko
    std::string pesel;          // Pesel (usunieto const bo nie dzialalo wtedy zapisywanie do pliku)
    boost::gregorian::date birthDate; // Data urodzenia
    std::shared_ptr<Address> address; // Adres
    std::vector<Category> licenses;   // Jakie prawko już ma

protected:
    Person() = default;

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