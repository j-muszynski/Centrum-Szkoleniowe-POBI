#pragma once

#include "typedefs.h"
#include <vector>

enum class Category;
class InstructorManager;
class StudentManager;
class DrivingSessionManager;
class VehicleManager;
class CourseManager;

class Coordinator {
private:
    InstructorManager& instructorManager;
    StudentManager& studentManager;
    DrivingSessionManager& drivingSessionManager;
    VehicleManager& vehicleManager;
    CourseManager& courseManager;

public:
    Coordinator(InstructorManager& instructorManger, StudentManager& studentManager, DrivingSessionManager& drivingSessionManager,
                VehicleManager& vehicleManger, CourseManager& courseManager);

    double calculateRevenue();
    double calculateTotalCourseCost(int courseId);
    void completeCourse(int courseId);
    double checkStudentCourseBalance(int studentId);
    double getTotalHoursForCourse(int courseId);
    double getStudentHours(int studentId);

    std::vector<VehiclePtr> getAvailableVehicles(Category category, TimeRange range);
    std::vector<InstructorPtr> getAvailableInstructors(Category category, TimeRange range);

    StudentPtr getStudentById(int id);
    InstructorPtr getInstructorById(int id);
    VehiclePtr getVehicleById(int id);
    CoursePtr getCourseById(int id);
    DrivingSessionPtr getDrivingSessionById(int id);

    int getCourseIDByStudentID(int studentID);
};
