#pragma once
#include "AbstractFileRepository.h"
#include "model/Course.h"
#include "typedefs.h"

/**
 * @file CourseRepository.h
 * @brief Repository class for managing Course entities in a file.
 */

/**
 * @class CourseRepository
 * @brief Handles a collection of Course objects.
 * @details This class extends AbstractFileRepository specifically for CoursePtr.
 * It adds a special method to create text reports of all courses.
 */
class CourseRepository : public AbstractFileRepository<CoursePtr> {
public:
    /**
     * @brief Creates a Course repository.
     * @param filePath Path to the file where courses are saved.
     * @param storage The storage interface used to save and load Course data.
     */
    CourseRepository(const std::string& filePath, IDataStorage<CoursePtr>& storage);

    /**
     * @brief Generates a text report of all courses in the repository.
     * @return A string containing detailed information about every course.
     */
    std::string reportCourses() const;
};