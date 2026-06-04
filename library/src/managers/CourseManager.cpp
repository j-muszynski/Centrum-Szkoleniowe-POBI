#include "managers/CourseManager.h"

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
void CourseManager::registerCourse(StudentPtr studentPtr, InstructorPtr instructorPtr, Category category) {

    auto newCourse = std::make_shared<Course>(courseRepo.getNextID(), category, instructorPtr, studentPtr, 0.0);

    courseRepo.add(newCourse);
}

std::string CourseManager::getCourseReport() const {
    std::string report;
    const auto& activeCourses = courseRepo.findAll();

    if (activeCourses.empty()) {
        report += "No active courses found.\n";
        return report;
    }

    for (const auto& course : activeCourses) {
        if (course != nullptr) {
            report += course->getCourseInfo() + "\n";
        }
    }
    return report;
}

std::string CourseManager::getArchiveCourseReport() const {
    std::string report;
    const auto& archivedCourses = archiveCourseRepo.findAll();

    if (archivedCourses.empty()) {
        report += "No archived courses found.\n";
        return report;
    }

    for (const auto& course : archivedCourses) {
        if (course != nullptr) {
            report += course->getCourseInfo() + "\n";
        }
    }
    return report;
}

CoursePtr CourseManager::getCourseById(int courseId) const {
    return courseRepo.getById(courseId);
}

CoursePtr CourseManager::getArchiveCourseById(int courseId) const {
    return archiveCourseRepo.getById(courseId);
}