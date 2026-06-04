#ifndef INSTRUCTOR_H
#define INSTRUCTOR_H

#include "model/Person.h"
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/split_member.hpp>

class Instructor : public Person {
private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, [[maybe_unused]] const unsigned int version) {
        ar & boost::serialization::base_object<Person>(*this);
        ar & qualifications;
    }

    std::vector<Category> qualifications; // Czego może uczy ć

protected:
    Instructor() = default;

public:
    Instructor(int id, const std::string& firstName, const std::string& lastName,
               const std::string& pesel, const boost::gregorian::date& birthDate,
               std::shared_ptr<Address> address);
    ~Instructor() override = default;

    const std::vector<Category>& getQualifications() const;
    void addQualification(Category category);

    std::string getInfo() const override;
};

#endif