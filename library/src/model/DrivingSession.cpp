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

std::string DrivingSession::getInfo() {
    std::string info = "CourseID: " + std::to_string(getCourseId()) + '\n' +
        "Student: " + getStudent()->getInfo() + '\n' +
        "Instructor: " + getInstructor()->getInfo() + '\n' +
        "Vehicle: " + getVehicle()->getVehicleInfo() + '\n' +
        "Start time: " + boost::posix_time::to_simple_string(getStartTime()) + '\n' +
        "End time: " + boost::posix_time::to_simple_string(getEndTime()) + '\n';
}
