#pragma once
#include "AbstractFileRepository.h"
#include "model/Student.h"
#include "typedefs.h"

/**
 * @file StudentRepository.h
 * @brief Repository class for managing Student entities in a file.
 */

/**
 * @class StudentRepository
 * @brief Handles a collection of Student objects.
 * @details This class extends AbstractFileRepository specifically for StudentPtr.
 * It adds a special method to create text reports of all students.
 */
class StudentRepository : public AbstractFileRepository<StudentPtr> {
public:
    /**
     * @brief Creates a Student repository.
     * @param filePath Path to the file where students are saved.
     * @param storage The storage interface used to save and load Student data.
     */
    StudentRepository(const std::string& filePath, IDataStorage<StudentPtr>& storage);

    /**
     * @brief Generates a text report of all students in the repository.
     * @return A string containing detailed information about every student.
     */
    [[nodiscard]] std::string reportStudents() const;
};