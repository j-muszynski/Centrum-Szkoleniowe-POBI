#ifndef COURSE_H
#define COURSE_H

#include "typedefs.h"
#include "model/Category.h"
#include "model/Instructor.h"
#include "model/Student.h"

#include <boost/serialization/access.hpp>

/**
 * @class Course
 * @brief Łączy kursanta, instruktora i konkretną kategorię prawa jazdy w jedno.
 *
 * Odpowiada za zarządzanie postępem szkolenia i stanowi bazę do naliczania opłat.
 */
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
    /**
     * @brief Inicjalizuje nową instancję szkolenia.
     * @param id Unikalny identyfikator w bazie.
     * @param type Wybrana kategoria prawka (np. Category::B).
     * @param instructor Wskaźnik na instruktora w systemie.
     * @param student Wskaźnik na kursanta w systemie.
     */
    Course(int id, Category type, InstructorPtr instructor,
           StudentPtr student);
    ~Course() = default;
    /** @brief Pobiera unikalne ID. */
    int getId() const;
    /** @brief Zwraca typ kategorii w jakiej realizowany jest kurs. */
    Category getType() const;
    /** @brief Pobiera obiekt instruktora przypisanego do kursu. */
    InstructorPtr getInstructor() const;
    /** @brief Pobiera obiekt przypisanego kursanta. */
    StudentPtr getStudent() const;
    /** @brief Sprawdza, czy kurs dobiegł już końca. */
    bool getIsFinished() const;

    /**
         * @brief Zmienia flagę ukończenia kursu.
         * @param finished Nowy status.
         */
    void setFinished(bool finished);

    /**
     * @brief Generuje sformatowane informacje o danym kursie dla interfejsu.
     * @return Wiersz tekstu z opisem id, typu i stanem kursu.
     */
    std::string getCourseInfo() const;

    // Usunieto *price zgodnie z poprawionym UML
};

#endif