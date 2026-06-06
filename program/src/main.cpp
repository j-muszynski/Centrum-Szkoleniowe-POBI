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
#include <boost/serialization/export.hpp>
#include "model/Student.h"
#include "model/Instructor.h"
#include "model/PassengerCar.h"
#include "model/Motorcycle.h"
#include "model/Truck.h"
#include "model/Bus.h"

BOOST_CLASS_EXPORT(Student)
BOOST_CLASS_EXPORT(Instructor)
BOOST_CLASS_EXPORT(PassengerCar)
BOOST_CLASS_EXPORT(Motorcycle)
BOOST_CLASS_EXPORT(Truck)
BOOST_CLASS_EXPORT(Bus)




int main() {
    boost::serialization::void_cast_register<Student, Person>();   // dodane bo przez static lib w UI boost wariowal
    boost::serialization::void_cast_register<Instructor, Person>();
    boost::serialization::void_cast_register<PassengerCar, Vehicle>();
    boost::serialization::void_cast_register<Motorcycle, Vehicle>();
    boost::serialization::void_cast_register<Truck, HeavyVehicle>();
    boost::serialization::void_cast_register<Bus, HeavyVehicle>();
    boost::serialization::void_cast_register<HeavyVehicle, Vehicle>();

    // Poprawka - osobne storage dla aktywnych i archiwalnych baz
    FileStorage<StudentPtr> studentStorage("students.txt");
    FileStorage<StudentPtr> archiveStudentStorage("archive_students.txt");

    FileStorage<InstructorPtr> instructorStorage("instructors.txt");
    FileStorage<InstructorPtr> archiveInstructorStorage("archive_instructors.txt");

    FileStorage<VehiclePtr> vehicleStorage("vehicles.txt");

    FileStorage<CoursePtr> courseStorage("courses.txt");
    FileStorage<CoursePtr> archiveCourseStorage("archive_courses.txt");

    FileStorage<DrivingSessionPtr> sessionStorage("sessions.txt");
    FileStorage<DrivingSessionPtr> archiveSessionStorage("archive_sessions.txt");

    // Repozytoria
    StudentRepository studentRepo("students.txt", studentStorage);
    StudentRepository archiveStudentRepo("archive_students.txt", archiveStudentStorage);

    InstructorRepository instructorRepo("instructors.txt", instructorStorage);
    InstructorRepository archiveInstructorRepo("archive_instructors.txt", archiveInstructorStorage);

    VehicleRepository vehicleRepo("vehicles.txt", vehicleStorage);

    CourseRepository courseRepo("courses.txt", courseStorage);
    CourseRepository archiveCourseRepo("archive_courses.txt", archiveCourseStorage);

    DrivingSessionRepository sessionRepo("sessions.txt", sessionStorage);
    DrivingSessionRepository archiveSessionRepo("archive_sessions.txt", archiveSessionStorage);

    // Wczytanie danycgh
    studentRepo.executeLoad();
    archiveStudentRepo.executeLoad();
    instructorRepo.executeLoad();
    archiveInstructorRepo.executeLoad();
    vehicleRepo.executeLoad();
    courseRepo.executeLoad();
    archiveCourseRepo.executeLoad();
    sessionRepo.executeLoad();
    archiveSessionRepo.executeLoad();

    // Menadzer i koordynaro - inicjalizacja
    StudentManager studentManager(studentRepo, archiveStudentRepo);
    InstructorManager instructorManager(instructorRepo, archiveInstructorRepo);
    VehicleManager vehicleManager(vehicleRepo);
    CourseManager courseManager(courseRepo, archiveCourseRepo);
    DrivingSessionManager sessionManager(sessionRepo, archiveSessionRepo);

    Coordinator coordinator(instructorManager, studentManager, sessionManager, vehicleManager, courseManager);
    ConsoleUI ui(coordinator);

    // Tu byl Jan Kowalski ale sprawial problemy

    ui.start();

    // Zapis
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