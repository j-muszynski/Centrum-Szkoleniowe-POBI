#ifndef COORDINATOR_H
#define COORDINATOR_H

#include <memory>
#include <vector>
#include <boost/date_time/posix_time/posix_time.hpp>

class CourseManager;
class StudentManager;
class InstructorManager;
class DrivingSessionManager;
class VehicleManager;

#include "model/Category.h"
#include "model/Vehicle.h"
#include "model/Instructor.h"
#include "model/Student.h"
#include "model/Course.h"
#include "model/DrivingSession.h"

struct TimeRange {
    boost::posix_time::ptime startTime;
    boost::posix_time::ptime endTime;
};

class Coordinator {
private:
    std::shared_ptr<CourseManager> courseManager;
    std::shared_ptr<StudentManager> studentManager;
    std::shared_ptr<InstructorManager> instructorManager;
    std::shared_ptr<DrivingSessionManager> drivingSessionManager;
    std::shared_ptr<VehicleManager> vehicleManager;

public:
    Coordinator(std::shared_ptr<CourseManager> courseManager,
                std::shared_ptr<StudentManager> studentManager,
                std::shared_ptr<InstructorManager> instructorManager,
                std::shared_ptr<DrivingSessionManager> drivingSessionManager,
                std::shared_ptr<VehicleManager> vehicleManager);

    ~Coordinator() = default;

    double calculateRevenue();
    double calculateTotalCourseCost(int courseId);
    void completeCourse(int courseId);
    double checkStudentCourseBalance(int studentId);
    double getTotalHoursForCourse(int courseId);
    double getStudentHours(int studentId);

    std::vector<std::shared_ptr<Vehicle>> getAvailableVehicles(Category category, TimeRange range);
    std::vector<std::shared_ptr<Instructor>> getAvailableInstructors(Category category, TimeRange range);

    std::shared_ptr<Student> getStudentById(int id);
    std::shared_ptr<Instructor> getInstructorById(int id);
    std::shared_ptr<Vehicle> getVehicleById(int id);
    std::shared_ptr<Course> getCourseById(int id);
    std::shared_ptr<DrivingSession> getDrivingSessionById(int id);
};

#endif