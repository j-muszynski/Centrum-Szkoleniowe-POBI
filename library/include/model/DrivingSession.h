#ifndef DRIVINGSESSION_H
#define DRIVINGSESSION_H

#include <string>
#include <memory>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "model/Instructor.h"
#include "model/Student.h"
#include "model/Vehicle.h"

class DrivingSession {
private:
    int id;
    boost::posix_time::ptime startTime;
    boost::posix_time::ptime endTime;        // Rozpoczęcie zakończenie
    std::shared_ptr<Instructor> instructor;  // Jaki instruktor
    std::shared_ptr<Student> student;        // Kto jechał
    int courseId;
    std::shared_ptr<Vehicle> vehicle;        // Czym jechał
    std::string status;

public:
    DrivingSession(int id, const boost::posix_time::ptime& startTime,
                   const boost::posix_time::ptime& endTime,
                   std::shared_ptr<Instructor> instructor,
                   std::shared_ptr<Student> student,
                   int courseId,
                   std::shared_ptr<Vehicle> vehicle);
    ~DrivingSession() = default;

    int getId() const;
    const boost::posix_time::ptime& getStartTime() const;
    const boost::posix_time::ptime& getEndTime() const;
    std::shared_ptr<Instructor> getInstructor() const;
    std::shared_ptr<Student> getStudent() const;
    int getCourseId() const;
    std::shared_ptr<Vehicle> getVehicle() const;
    const std::string& getStatus() const;

    void setStatus(const std::string& status);

    std::string getInfo();
    double getSessionCost() const; // zgodnie z UML
};

#endif