#pragma once
#include <functional>

class Course;
class DrivingSession;
class Instructor;
class Student;
class Address;
class Vehicle;


typedef std::shared_ptr<Course> CoursePtr;
typedef std::shared_ptr<DrivingSession> DrivingSessionPtr;
typedef std::shared_ptr<Instructor> InstructorPtr;
typedef std::shared_ptr<Student> StudentPtr;
typedef std::shared_ptr<Address> AddressPtr;
typedef std::shared_ptr<Vehicle> VehiclePtr;

typedef std::shared_ptr<const Course> ConstCoursePtr;
typedef std::shared_ptr<const DrivingSession> ConstDrivingSessionPtr;
typedef std::shared_ptr<const Instructor> ConstInstructorPtr;
typedef std::shared_ptr<const Student> ConstStudentPtr;
typedef std::shared_ptr<const Address> ConstAddressPtr;
typedef std::shared_ptr<const Vehicle> ConstVehiclePtr;

typedef std::function<bool(VehiclePtr)> VehiclePredicate;
typedef std::function<bool(InstructorPtr)> InstructorPredicate;
typedef std::function<bool(CoursePtr)> CoursePredicate;
typedef std::function<bool(DrivingSession)> DrivingSessionPredicate;
