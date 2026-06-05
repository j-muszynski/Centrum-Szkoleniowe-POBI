#ifndef DRIVINGSESSION_H
#define DRIVINGSESSION_H

#include <string>
#include "typedefs.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include "model/Instructor.h"
#include "model/Student.h"
#include "model/Vehicle.h"
#include <boost/serialization/access.hpp>


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
    DrivingSession(int id, const boost::posix_time::ptime& startTime,
                   const boost::posix_time::ptime& endTime,
                   InstructorPtr instructor,
                   StudentPtr student,
                   int courseId,
                   VehiclePtr vehicle);
    ~DrivingSession() = default;

    int getId() const;
    const boost::posix_time::ptime& getStartTime() const;
    const boost::posix_time::ptime& getEndTime() const;
    InstructorPtr getInstructor() const;
    StudentPtr getStudent() const;
    int getCourseId() const;
    VehiclePtr getVehicle() const;
    const std::string& getStatus() const;

    void setStatus(const std::string& status);

    std::string getInfo();
    double getSessionCost() const; // zgodnie z UML
};

#endif