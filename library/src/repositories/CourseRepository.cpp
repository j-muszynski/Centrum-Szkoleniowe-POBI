#include "repositories/CourseRepository.h"

CourseRepository::CourseRepository(const std::string& filePath, IDataStorage<CoursePtr>& storage)
    : AbstractFileRepository<CoursePtr>(filePath, storage) {}

std::string CourseRepository::reportCourses() const {
    std::string info;
    for (const auto& course : repo) {
        if (course != nullptr) {
            info += course->getCourseInfo();
        }
    }
    return info;
}