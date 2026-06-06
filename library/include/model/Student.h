#ifndef STUDENT_H
#define STUDENT_H

#include <random>
#include "typedefs.h"
#include "model/Person.h"
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/access.hpp>

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

    Student(int id, const std::string& firstName, const std::string& lastName,
            const std::string& pesel, const boost::gregorian::date& birthDate,
            AddressPtr address, const std::string& pkkNumber);
    ~Student() override = default;

    const std::string& getPkkNumber() const;
    bool getIsReadyForExam() const;

    void setPkkNumber(const std::string& pkkNumber);
    void setReadyForExam(bool isReady);

    std::string getInfo() const override;
};

#endif