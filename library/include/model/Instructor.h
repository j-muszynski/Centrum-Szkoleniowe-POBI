#ifndef INSTRUCTOR_H
#define INSTRUCTOR_H

#include "model/Person.h"

class Instructor : public Person {
private:
    std::vector<Category> qualifications; // Czego może uczy ć

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