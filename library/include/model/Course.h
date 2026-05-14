#ifndef COURSE_H
#define COURSE_H

#include <memory>
#include "model/Category.h"
#include "model/Instructor.h"
#include "model/Student.h"

class Course {
private:
    int id;
    Category type; // Kategoria kursu
    std::shared_ptr<Instructor> instructor; // Wskaźnik na przypisanego instruktora prowadzącego
    std::shared_ptr<Student> student; // Na kursanta
    bool isFinished; // 1 = godziny wyjeżdżone 0 = wiadomo co
    double price;    // Cena

public:
    Course(int id, Category type, std::shared_ptr<Instructor> instructor,
           std::shared_ptr<Student> student, double price);
    ~Course() = default;

    int getId() const;
    Category getType() const;
    std::shared_ptr<Instructor> getInstructor() const;
    std::shared_ptr<Student> getStudent() const;
    bool getIsFinished() const;
    double getPrice() const;

    void setFinished(bool finished);

    std::string getCourseInfo() const;
};

#endif