#include "model/DrivingSession.h"

DrivingSession::DrivingSession(int id, const boost::posix_time::ptime& startTime,
                               const boost::posix_time::ptime& endTime,
                               std::shared_ptr<Instructor> instructor,
                               std::shared_ptr<Student> student,
                               int courseId,
                               std::shared_ptr<Vehicle> vehicle)
    : id(id), startTime(startTime), endTime(endTime), instructor(instructor),
      student(student), courseId(courseId), vehicle(vehicle), status("Scheduled") {}

int DrivingSession::getId() const { return id; }
const boost::posix_time::ptime& DrivingSession::getStartTime() const { return startTime; }
const boost::posix_time::ptime& DrivingSession::getEndTime() const { return endTime; }
std::shared_ptr<Instructor> DrivingSession::getInstructor() const { return instructor; }
std::shared_ptr<Student> DrivingSession::getStudent() const { return student; }
int DrivingSession::getCourseId() const { return courseId; }
std::shared_ptr<Vehicle> DrivingSession::getVehicle() const { return vehicle; }
const std::string& DrivingSession::getStatus() const { return status; }

void DrivingSession::setStatus(const std::string& newStatus) {
    status = newStatus;
}

std::string DrivingSession::getInfo() { // funkcja nic nie zwracala, dodany return
    return "CourseID: " + std::to_string(getCourseId()) + '\n' +
        "Student: " + getStudent()->getInfo() + '\n' +
        "Instructor: " + getInstructor()->getInfo() + '\n' +
        "Vehicle: " + getVehicle()->getVehicleInfo() + '\n' +
        "Start time: " + boost::posix_time::to_simple_string(getStartTime()) + '\n' +
        "End time: " + boost::posix_time::to_simple_string(getEndTime()) + '\n';
}
double DrivingSession::getSessionCost() const {
    if (vehicle == nullptr) {return 0.0;}

    // Oblicza czasu trwania jazdy za pomocą biblioteki Boost
    boost::posix_time::time_duration duration = endTime - startTime;

    // Zwraca czas trwania w godzinach (np. 1.5 = 1h 30m)
    double hours = duration.total_seconds() / 3600.0;

    // Koszt: stawka godzinowa pojazdu * czas
    return vehicle->getBaseHourlyCost() * hours;
}