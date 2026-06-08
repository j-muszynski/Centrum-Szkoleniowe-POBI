#include "managers/CourseManager.h"
#include "repositories/CourseRepository.h"
#include "model/Exceptions.h"

#include "model/Student.h"

CourseManager::CourseManager(IRepository<CoursePtr>& courseRepo, IRepository<CoursePtr>& archiveCourseRepo)
    : courseRepo(courseRepo), archiveCourseRepo(archiveCourseRepo) {}

std::vector<CoursePtr> CourseManager::findCourses(const CoursePredicate& predicate) const {
    return courseRepo.findBy(predicate);
}

std::vector<CoursePtr> CourseManager::findArchiveCourses(const CoursePredicate& predicate) const {
    return archiveCourseRepo.findBy(predicate);
}

std::vector<CoursePtr> CourseManager::findAllCourses() const {
    return courseRepo.findAll();
}

std::vector<CoursePtr> CourseManager::findAllArchiveCourses() const {
    return archiveCourseRepo.findAll();
}

/*
 * calculating the price: when driving session is finished hours of the driving session * cost of the vehicle is added to
 * course price which will allow for easier accounting in coordinator class
 */
void CourseManager::registerCourse(const StudentPtr& studentPtr, const InstructorPtr& instructorPtr,
                                   Category category) {
    if (studentPtr == nullptr || instructorPtr == nullptr) {
        throw NullPointerException("Cannot register course: Student or Instructor pointer is null.");
    }

    std::vector<Category> licences = instructorPtr->getQualifications();

    auto it = std::find(licences.begin(), licences.end(), category);

    if (it != licences.end()) {
        auto newCourse = std::make_shared<Course>(courseRepo.getNextID(), category, instructorPtr, studentPtr);

        courseRepo.add(newCourse);
    }
    else {
        throw ValidationException("Instructor does not have the required driving license category for this course.");
    }
}

std::string CourseManager::getCourseReport() const {
    const auto& concreteRepo = dynamic_cast<const CourseRepository&>(courseRepo);

    return concreteRepo.reportCourses();
}

std::string CourseManager::getArchiveCourseReport() const {
    // casting IRepository to concrete type to be able to use report method
    const auto& concreteArchiveRepo = dynamic_cast<const CourseRepository&>(archiveCourseRepo);

    return concreteArchiveRepo.reportCourses();
}

CoursePtr CourseManager::getCourseById(int courseId) const {
    return courseRepo.getById(courseId);
}

CoursePtr CourseManager::getArchiveCourseById(int courseId) const {
    return archiveCourseRepo.getById(courseId);
}

void CourseManager::completeCourse(int courseId) {
    auto course = courseRepo.getById(courseId);

    if (course == nullptr) {
        throw NotFoundException(
            "Cannot complete course: Active course with ID " + std::to_string(courseId) + " not found.");
    }
    course->setFinished(true);
    archiveCourseRepo.add(course);
    courseRepo.remove(courseId);
}
