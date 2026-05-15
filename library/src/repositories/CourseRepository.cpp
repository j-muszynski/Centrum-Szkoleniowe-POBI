#include "repositories/CourseRepository.h"
#include "model/Course.h"
#include <fstream>
#include <iostream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

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

bool CourseRepository::saveData() {
    try {
        // creating file for saved data
        std::ofstream file("courses_data.txt");
        if (!file.is_open()) return false;

        // creating archive
        boost::archive::text_oarchive archive(file);

        // saving the vector
        archive << courseRepo;

        return true;
    } catch ([[maybe_unused]] boost::archive::archive_exception& e) {
        std::cout << "error while saving course repository!" << std::endl;
        return false;
    }
}

// logic yet to come
bool CourseRepository::loadData() {
    try {
        // opening a file with data to load
        std::ifstream file("courses_data.txt");
        if (!file.is_open()) return false;

        // creating archive conncted to file with data
        boost::archive::text_iarchive archive(file);

        // laoding up the data
        archive >> courseRepo;

        return true;
    } catch (const boost::archive::archive_exception& e) {
        // catching exceptions
        std::cerr << "Błąd podczas wczytywania repozytorium: " << e.what() << std::endl;
        return false;
    }
}

