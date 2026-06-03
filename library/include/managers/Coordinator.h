#pragma once

#include "managers/InstructorManager.h"
#include "managers/StudentManager.h"
#include "managers/DrivingSessionManager.h"
#include "managers/VehicleManager.h"
#include "managers/CourseManager.h"

class Coordinator {
private:
    InstructorManager& instructorRepo;
    StudentManager& studentRepo;
    DrivingSession& drivingSessionRepo;
    VehicleManager& vehicleRepo;
    CourseManager& courseRepo;

public:
    Coordinator(InstructorManager& instructorRepo, StudentManager& studentRepo, DrivingSession& drivingSessionRepo,
                VehicleManager& vehicleRepo, CourseManager& courseRepo);

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
