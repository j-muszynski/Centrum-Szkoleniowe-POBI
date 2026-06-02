#pragma once
#include "AbstractFileRepository.h"
#include "model/Course.h"

#include "typedefs.h"

class CourseRepository : public AbstractFileRepository<CoursePtr> {
public:
    CourseRepository(const std::string& filePath, IDataStorage<CoursePtr>& storage);
    std::string reportCourses() const;
};