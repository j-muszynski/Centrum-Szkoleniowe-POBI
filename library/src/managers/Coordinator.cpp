#include "managers/Coordinator.h"
#include "managers/InstructorManager.h"
#include "managers/StudentManager.h"
#include "managers/DrivingSessionManager.h"
#include "managers/VehicleManager.h"
#include "managers/CourseManager.h"
#include "model/Category.h"

#include <algorithm>

Coordinator::Coordinator(InstructorManager& instructorManager, StudentManager& studentManager,
                         DrivingSessionManager& drivingSessionManager, VehicleManager& vehicleManager,
                         CourseManager& courseManager) : instructorManager(instructorManager),
                                                         studentManager(studentManager),
                                                         drivingSessionManager(drivingSessionManager),
                                                         vehicleManager(vehicleManager),
                                                         courseManager(courseManager) {}

std::shared_ptr<Student> Coordinator::getStudentById(int id) {
    auto students = studentManager.findAllStudents();
    for (const auto& student : students) {
        if (student->getId() == id) {
            return student;
        }
    }
    return nullptr;
}

std::shared_ptr<Instructor> Coordinator::getInstructorById(int id) {
    auto instructors = instructorManager.findAllInstructors();
    for (const auto& instructor : instructors) {
        if (instructor->getId() == id) {
            return instructor;
        }
    }
    return nullptr;
}

std::shared_ptr<Vehicle> Coordinator::getVehicleById(int id) {
    auto vehicles = vehicleManager.findAllVehicles();
    for (const auto& vehicle : vehicles) {
        if (vehicle != nullptr && vehicle->getId() == id) {
            return vehicle;
        }
    }
    return nullptr;
}

std::shared_ptr<Course> Coordinator::getCourseById(int id) {
    auto courses = courseManager.findAllCourses();
    for (const auto& course : courses) {
        if (course->getId() == id) {
            return course;
        }
    }
    return nullptr;
}

std::shared_ptr<DrivingSession> Coordinator::getDrivingSessionById(int id) {
    auto sessions = drivingSessionManager.findAllDrivingSessions();
    for (const auto& session : sessions) {
        if (session->getId() == id) {
            return session;
        }
    }
    return nullptr;
}

double Coordinator::calculateTotalCourseCost(int courseId) {
    double totalCost = 0.0;
    auto sessions = drivingSessionManager.findAllDrivingSessions();

    for (const auto& session : sessions) {
        if (session->getCourseId() == courseId) {
            totalCost += session->getSessionCost();
        }
    }
    return totalCost;
}

double Coordinator::calculateRevenue() {
    double revenue = 0.0;
    auto courses = courseManager.findAllCourses();

    for (const auto& course : courses) {
        revenue += calculateTotalCourseCost(course->getId());
    }
    return revenue;
}

void Coordinator::completeCourse(int courseId) {
    auto course = getCourseById(courseId);
    if (course != nullptr) {
        course->setFinished(true);
    }
}

double Coordinator::getTotalHoursForCourse(int courseId) {
    double totalHours = 0.0;
    auto sessions = drivingSessionManager.findAllDrivingSessions();

    for (const auto& session : sessions) {
        if (session->getCourseId() == courseId) {
            boost::posix_time::time_duration duration = session->getEndTime() - session->getStartTime();
            totalHours += duration.total_seconds() / 3600.0;
        }
    }
    return totalHours;
}

double Coordinator::getStudentHours(int studentId) {
    double totalHours = 0.0;
    auto sessions = drivingSessionManager.findAllDrivingSessions();

    for (const auto& session : sessions) {
        if (session->getStudent() != nullptr && session->getStudent()->getId() == studentId) {
            boost::posix_time::time_duration duration = session->getEndTime() - session->getStartTime();
            totalHours += duration.total_seconds() / 3600.0;
        }
    }
    return totalHours;
}

double Coordinator::checkStudentCourseBalance(int studentId) {
    double balance = 0.0;
    auto courses = courseManager.findAllCourses();

    for (const auto& course : courses) {
        if (course->getStudent() != nullptr && course->getStudent()->getId() == studentId) {
            balance += calculateTotalCourseCost(course->getId());
        }
    }
    return balance;
}

std::vector<std::shared_ptr<Vehicle>> Coordinator::getAvailableVehicles(Category category, TimeRange range) {
    std::vector<std::shared_ptr<Vehicle>> available;
    auto allVehicles = vehicleManager.findAllVehicles();
    auto allSessions = drivingSessionManager.findAllDrivingSessions();

    for (const auto& vehicle : allVehicles) {
        if (vehicle != nullptr && vehicle->getCategory() == category) {
            bool isBusy = false;
            for (const auto& session : allSessions) {
                if (session->getVehicle() != nullptr && session->getVehicle()->getId() == vehicle->getId()) {
                    if (session->getStartTime() < range.second && session->getEndTime() > range.first) {
                        isBusy = true;
                        break;
                    }
                }
            }
            if (!isBusy) {
                available.push_back(vehicle);
            }
        }
    }
    return available;
}

std::vector<std::shared_ptr<Instructor>> Coordinator::getAvailableInstructors(Category category, TimeRange range) {
    std::vector<std::shared_ptr<Instructor>> available;
    auto allInstructors = instructorManager.findAllInstructors();
    auto allSessions = drivingSessionManager.findAllDrivingSessions();

    for (const auto& instructor : allInstructors) {
        auto quals = instructor->getQualifications();
        bool hasCategory = std::find(quals.begin(), quals.end(), category) != quals.end();

        if (hasCategory) {
            bool isBusy = false;
            for (const auto& session : allSessions) {
                if (session->getInstructor() != nullptr && session->getInstructor()->getId() == instructor->getId()) {
                    if (session->getStartTime() < range.second && session->getEndTime() > range.first) {
                        isBusy = true;
                        break;
                    }
                }
            }
            if (!isBusy) {
                available.push_back(instructor);
            }
        }
    }
    return available;
}


int Coordinator::getCourseIDByStudentID(int studentId) {
    auto courses = courseManager.findAllCourses();

    for (const auto& course : courses) {
        // checking for nullptr
        if (course != nullptr && course->getStudent() != nullptr) {
            if (course->getStudent()->getId() == studentId) {
                return course->getId(); // if found -> return it
            }
        }
    }

    // not found -> -1
    std::cout << "Warning: No active course found for Student ID: " << studentId << "\n";
    return -1;
}