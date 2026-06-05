#include "model/Course.h"

Course::Course(int id, Category type, InstructorPtr instructor,
               StudentPtr student)
    : id(id), type(type), instructor(instructor), student(student), 
      isFinished(false) {}

int Course::getId() const { return id; }
Category Course::getType() const { return type; }
InstructorPtr Course::getInstructor() const { return instructor; }
StudentPtr Course::getStudent() const { return student; }
bool Course::getIsFinished() const { return isFinished; }

void Course::setFinished(bool finished) { isFinished = finished; }

std::string Course::getCourseInfo() const {
    std::string info = "Kurs [ID: " + std::to_string(id) + "]";

    // Mapowanie Enum na Stringa (UI wyswietlalo za malo danych)
    std::string catStr = "?";
    if (type == Category::AM) catStr = "AM";
    else if (type == Category::A1) catStr = "A1";
    else if (type == Category::A2) catStr = "A2";
    else if (type == Category::A) catStr = "A";
    else if (type == Category::B) catStr = "B";
    else if (type == Category::BE) catStr = "BE";
    else if (type == Category::C) catStr = "C";
    else if (type == Category::CE) catStr = "CE";
    else if (type == Category::D) catStr = "D";
    else if (type == Category::DE) catStr = "DE";

    info += " | Kategoria: " + catStr;

    if (student != nullptr) {
        info += " | Kursant: " + student->getFirstName() + " " + student->getLastName();
    }
    if (instructor != nullptr) {
        info += " | Instruktor: " + instructor->getFirstName() + " " + instructor->getLastName();
    }

    info += " | Status: " + std::string(isFinished ? "Zakonczony" : "W trakcie");

    return info;
}