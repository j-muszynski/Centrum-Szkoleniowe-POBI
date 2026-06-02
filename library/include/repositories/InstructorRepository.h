#pragma once
#include "AbstractFileRepository.h"
#include "model/Instructor.h"
#include "typedefs.h"

/**
 * @file InstructorRepository.h
 * @brief Repository class for managing Instructor entities in a file.
 */

/**
 * @class InstructorRepository
 * @brief Handles a collection of Instructor objects.
 * @details This class extends AbstractFileRepository specifically for InstructorPtr.
 * It adds a special method to create text reports of all instructors.
 */
class InstructorRepository : public AbstractFileRepository<InstructorPtr> {
public:
    /**
     * @brief Creates an Instructor repository.
     * @param filePath Path to the file where instructors are saved.
     * @param storage The storage interface used to save and load Instructor data.
     */
    InstructorRepository(const std::string& filePath, IDataStorage<InstructorPtr>& storage);

    /**
     * @brief Generates a text report of all instructors in the repository.
     * @return A string containing detailed information about every instructor.
     */
    std::string reportInstructors() const;
};