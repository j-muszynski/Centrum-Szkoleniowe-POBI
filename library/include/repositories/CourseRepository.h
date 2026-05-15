#pragma once

#include "typedefs.h"
#include <fstream>
#include <vector>

class CourseRepository{
private:
    std::vector<CoursePtr> courseRepo;

    CourseRepository() = default;
public:
    [[nodiscard]] CoursePtr getCourse(int id) const;

    [[nodiscard]] int getSize() const;

    void addCourse(const CoursePtr& newCourse);

    void removeCourse(int id);

    [[nodiscard]] std::string reportCourses() const;

    static bool alwaysTrue(const CoursePtr& course);

    [[nodiscard]] std::vector<CoursePtr> findBy(const CoursePredicate& predicate) const;

    [[nodiscard]] std::vector<CoursePtr> findAll() const;

    [[nodiscard]] bool saveData();

    [[nodiscard]] bool loadData();
};