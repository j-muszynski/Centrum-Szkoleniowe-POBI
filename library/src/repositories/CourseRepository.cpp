#include "repositories/CourseRepository.h"
#include "model/Course.h"

CoursePtr CourseRepository::getCourse(int id) const {
    for (auto c : courseRepo) {
        if (c->getId() == id) return c;
    }
    return nullptr;
}

int CourseRepository::getSize() const {
    return courseRepo.size();
}

void CourseRepository::addCourse(const CoursePtr& newCourse) {
    if (newCourse != nullptr) courseRepo.push_back(newCourse);
}

std::string CourseRepository::reportCourses() const {
    std::string info = "";
    for (auto c : courseRepo) {
        info += c->getCourseInfo();
    }
    return info;
}

bool CourseRepository::alwaysTrue(const CoursePtr&) {
    return true;
}

std::vector<CoursePtr> CourseRepository::findBy(const CoursePredicate& predicate) const {
    std::vector<CoursePtr> result;
    for (const auto& c : courseRepo) {
        if (c != nullptr && predicate(c)) result.push_back(c);
    }

    return result;
}

std::vector<CoursePtr> CourseRepository::findAll() const {
  return findBy(alwaysTrue);
}

// logic yet to come
bool CourseRepository::saveData() {
    bool success = 1;
    return success;
}

// logic yet to come
bool CourseRepository::loadData() {
    bool success = 1;
    return success;
}
