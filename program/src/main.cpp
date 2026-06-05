#include "ui/ConsoleUI.h"
#include "managers/Coordinator.h"
#include "managers/StudentManager.h"
#include "managers/InstructorManager.h"
#include "managers/VehicleManager.h"
#include "managers/CourseManager.h"
#include "managers/DrivingSessionManager.h"
#include "repositories/StudentRepository.h"
#include "repositories/InstructorRepository.h"
#include "repositories/VehicleRepository.h"
#include "repositories/CourseRepository.h"
#include "repositories/DrivingSessionRepository.h"
#include "repositories/FileStorage.h"

int main() {
    FileStorage<StudentPtr> studentStorage("students.txt");
    FileStorage<InstructorPtr> instructorStorage("instructors.txt");
    FileStorage<VehiclePtr> vehicleStorage("vehicles.txt");
    FileStorage<CoursePtr> courseStorage("courses.txt");
    FileStorage<DrivingSessionPtr> sessionStorage("sessions.txt");

    StudentRepository studentRepo("students.txt", studentStorage);
    StudentRepository archiveStudentRepo("archive_students.txt", studentStorage);
    InstructorRepository instructorRepo("instructors.txt", instructorStorage);
    InstructorRepository archiveInstructorRepo("archive_instructors.txt", instructorStorage);
    VehicleRepository vehicleRepo("vehicles.txt", vehicleStorage);
    CourseRepository courseRepo("courses.txt", courseStorage);
    CourseRepository archiveCourseRepo("archive_courses.txt", courseStorage);
    DrivingSessionRepository sessionRepo("sessions.txt", sessionStorage);
    DrivingSessionRepository archiveSessionRepo("archive_sessions.txt", sessionStorage);

    studentRepo.executeLoad();
    archiveStudentRepo.executeLoad();
    instructorRepo.executeLoad();
    archiveInstructorRepo.executeLoad();
    vehicleRepo.executeLoad();
    courseRepo.executeLoad();
    archiveCourseRepo.executeLoad();
    sessionRepo.executeLoad();
    archiveSessionRepo.executeLoad();

    StudentManager studentManager(studentRepo, archiveStudentRepo);
    InstructorManager instructorManager(instructorRepo, archiveInstructorRepo);
    VehicleManager vehicleManager(vehicleRepo);
    CourseManager courseManager(courseRepo, archiveCourseRepo);
    DrivingSessionManager sessionManager(sessionRepo, archiveSessionRepo);

    Coordinator coordinator(instructorManager, studentManager, sessionManager, vehicleManager, courseManager);

    ConsoleUI ui(coordinator);
    ui.start();

    studentRepo.executeSave();
    archiveStudentRepo.executeSave();
    instructorRepo.executeSave();
    archiveInstructorRepo.executeSave();
    vehicleRepo.executeSave();
    courseRepo.executeSave();
    archiveCourseRepo.executeSave();
    sessionRepo.executeSave();
    archiveSessionRepo.executeSave();

    return 0;
}