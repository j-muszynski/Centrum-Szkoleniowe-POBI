#include "model/Course.h"

Course::Course(int id, Category type, std::shared_ptr<Instructor> instructor, 
               std::shared_ptr<Student> student)
    : id(id), type(type), instructor(instructor), student(student), 
      isFinished(false) {}

int Course::getId() const { return id; }
Category Course::getType() const { return type; }
std::shared_ptr<Instructor> Course::getInstructor() const { return instructor; }
std::shared_ptr<Student> Course::getStudent() const { return student; }
bool Course::getIsFinished() const { return isFinished; }

void Course::setFinished(bool finished) { isFinished = finished; }

std::string Course::getCourseInfo() const {
    return "Kurs ID: " + std::to_string(id) + 
           " | Status: " + (isFinished ? "Zakonczony" : "W trakcie");  // {warunek ? prawda : fałsz}, ale to napewno wiesz
}
// usuniecie *price