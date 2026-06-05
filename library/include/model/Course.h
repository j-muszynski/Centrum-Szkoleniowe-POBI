#ifndef COURSE_H
#define COURSE_H

#include "typedefs.h"
#include "model/Category.h"
#include "model/Instructor.h"
#include "model/Student.h"

#include <boost/serialization/access.hpp>


class Course {
private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, [[maybe_unused]] const unsigned int version) {
        ar & id;
        ar & type;
        ar & instructor;
        ar & student;
        ar & isFinished;
    }
    int id;
    Category type; // Kategoria kursu
    InstructorPtr instructor; // Wskaźnik na przypisanego instruktora prowadzącego
    StudentPtr student; // Na kursanta
    bool isFinished; // 1 = godziny wyjeżdżone 0 = wiadomo co
protected:
    Course() = default;
public:
    Course(int id, Category type, InstructorPtr instructor,
           StudentPtr student);
    ~Course() = default;

    int getId() const;
    Category getType() const;
    InstructorPtr getInstructor() const;
    StudentPtr getStudent() const;
    bool getIsFinished() const;

    void setFinished(bool finished);

    std::string getCourseInfo() const;

    // Usunieto *price zgodnie z poprawionym UML
};

#endif