#ifndef STUDENT_H
#define STUDENT_H

#include "model/Person.h"

class Student : public Person {
private:
    std::string pkkNumber;  // Nr Prof Kand na Kierowce
    bool isReadyForExam;    // Czy ma wyjeżdżone h

public:
    Student(int id, const std::string& firstName, const std::string& lastName,
            const std::string& pesel, const boost::gregorian::date& birthDate,
            std::shared_ptr<Address> address, const std::string& pkkNumber);
    ~Student() override = default;

    const std::string& getPkkNumber() const;
    bool getIsReadyForExam() const;

    void setPkkNumber(const std::string& pkkNumber);
    void setReadyForExam(bool isReady);

    std::string getInfo() const override;
};

#endif