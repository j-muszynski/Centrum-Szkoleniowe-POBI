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

    void completeCourse(int courseID);

    double checkStudentCourseBalance(int courseID);

    double getTotalHoursForCourse(int courseID);

    double getStudentHours(int studentID);

    std::vector<VehiclePtr> getAvailableVehicles(Category category, TimeRange timeRange);

    std::vector<InstructorPtr> getAvailableInstructors(Category category, TimeRange timeRange);


};
