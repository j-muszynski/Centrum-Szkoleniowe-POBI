#ifndef DRIVINGSESSION_H
#define DRIVINGSESSION_H

#include <string>
#include "typedefs.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include "model/Instructor.h"
#include "model/Student.h"
#include "model/Vehicle.h"
#include <boost/serialization/access.hpp>

/**
 * @class DrivingSession
 * @brief Odzwierciedla pojedynczą jazdę w kalendarzu ośrodka.
 *
 * Zawiera konkretny ramy czasowe, instruktora, pojazd i ucznia na dany wyjazd.
 */
class DrivingSession {
private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, [[maybe_unused]] const unsigned int version) {
        ar & id;
        ar & startTime;
        ar & endTime;
        ar & instructor;
        ar & student;
        ar & courseId;
        ar & vehicle;
        ar & status;
    }
    int id;
    boost::posix_time::ptime startTime;
    boost::posix_time::ptime endTime;        // Rozpoczęcie zakończenie
    InstructorPtr instructor;  // Jaki instruktor
    StudentPtr student;        // Kto jechał
    int courseId;
    VehiclePtr vehicle;        // Czym jechał
    std::string status;

protected:
    DrivingSession() = default;

public:
    /**
         * @brief Tworzy rezerwację na jazdę szkoleniową.
         * @param id Unikalny nr wpisu.
         * @param startTime Data i czas startu jazd.
         * @param endTime Data i czas zakończenia.
         * @param instructor Wykorzystany wskaźnik na Instruktora.
         * @param student Wykorzystany wskaźnik na Ucznia.
         * @param courseId ID trwającego kursu.
         * @param vehicle Wynajmowany Pojazd.
         */
    DrivingSession(int id, const boost::posix_time::ptime& startTime,
                   const boost::posix_time::ptime& endTime,
                   InstructorPtr instructor,
                   StudentPtr student,
                   int courseId,
                   VehiclePtr vehicle);
    ~DrivingSession() = default;
    /** @brief ID sesji. */
    int getId() const;
    /** @brief Start sesji. */
    const boost::posix_time::ptime& getStartTime() const;
    /** @brief Koniec sesji. */
    const boost::posix_time::ptime& getEndTime() const;
    /** @brief Wskaźnik na instruktora prowadzącego. */
    InstructorPtr getInstructor() const;
    /** @brief Wskaźnik na studenta uczestniczącego. */
    StudentPtr getStudent() const;
    /** @brief ID powiązanego kursu. */
    int getCourseId() const;
    /** @brief Pojazd używany podczas tych godzin. */
    VehiclePtr getVehicle() const;
    /** @brief Status rezerwacji (np. anulowane, zakończone, w toku). */
    const std::string& getStatus() const;

    /** @brief Modyfikuje status obecnej sesji. */
    void setStatus(const std::string& status);

    /** @brief Łączy informacje o sesji. */
    std::string getInfo();

    /**
     * @brief Wylicza dynamiczny koszt oparty na baseCost pojazdu * czas.
     * @return Ostateczny koszt tych konkretnych jazd.
     */
    double getSessionCost() const; // zgodnie z UML
};

#endif