#pragma once
#include "typedefs.h"
#include "repositories/IRepository.h"
#include "model/Category.h"
#include "model/Course.h"


/**
 * @class CourseManager
 * @brief Handles business logic for course lifecycles, registrations, and lookup queries.
 */
class CourseManager {
private:
    /** @brief Reference to the active courses repository */
    IRepository<CoursePtr>& courseRepo;

    /** @brief Reference to the archived (completed) courses repository */
    IRepository<CoursePtr>& archiveCourseRepo;

public:
    /**
     * @brief Constructs a CourseManager with required data repositories.
     * @param courseRepo Reference to the repository handling active courses.
     * @param archiveCourseRepo Reference to the repository handling archived courses.
     */
    CourseManager(IRepository<CoursePtr>& courseRepo, IRepository<CoursePtr>& archiveCourseRepo);

    /**
     * @brief Destructor for CourseManager.
     */
    ~CourseManager() = default;

    /**
     * @brief Filters active courses using a custom predicate function.
     * @param predicate A callable boolean filter.
     * @return std::vector<CoursePtr> List of active courses matching the predicate criteria.
     */
    std::vector<CoursePtr> findCourses(const CoursePredicate& predicate) const;

    /**
     * @brief Filters archive courses using a custom predicate function.
     * @param predicate A callable boolean filter.
     * @return std::vector<CoursePtr> List of archive courses matching the predicate criteria.
     */
    std::vector<CoursePtr> findArchiveCourses(const CoursePredicate& predicate) const;

    /**
     * @brief Retrieves all active courses currently registered in the system.
     * @return std::vector<CoursePtr> List containing all active courses.
     */
    std::vector<CoursePtr> findAllCourses() const;

    /**
     * @brief Retrieves all archive courses currently registered in the system.
     * @return std::vector<CoursePtr> List containing all archive courses.
     */
    std::vector<CoursePtr> findAllArchiveCourses() const;

    /**
     * @brief Registers and creates a new course session in the active repository.
     * @param student Pointer to student whic course will be started for.
     * @param instructor Pointer to instructor that is suppose to lead the student through the driving journey.
     * @param category The driving license category type for the course.
     */
    void registerCourse(const StudentPtr& student, const InstructorPtr& instructor, Category category);

    /**
     * @brief Generates a text report of all managed courses.
     * @return std::string Formatted report containing details of each course.
     */
    std::string getCourseReport() const;

    /**
     * @brief Generates a text report of all managed courses.
     * @return std::string Formatted report containing details of each course.
     */
    std::string getArchiveCourseReport() const;

    /**
     * @brief Retrieves a course entity by its unique identifier from active repository.
     * @param courseId The unique ID of the target course.
     * @return CoursePtr A shared pointer to the Course object, or nullptr if not found.
     */
    CoursePtr getCourseById(int courseId) const;

    /**
     * @brief Retrieves a course entity by its unique identifier from archived repository.
     * @param courseId The unique ID of the target course.
     * @return CoursePtr A shared pointer to the Course object, or nullptr if not found.
     */
    CoursePtr getArchiveCourseById(int courseId) const;
};
