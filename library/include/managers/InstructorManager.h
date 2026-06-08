#pragma once

#include "typedefs.h"
#include "repositories/IRepository.h"
#include "model/Category.h"
#include <vector>
#include <string>

/**
 * @class InstructorManager
 * @brief Handles business logic for instructor management, qualifications, and reporting.
 */
class InstructorManager {
private:
    /** @brief Reference to the main instructor repository */
    IRepository<InstructorPtr>& instructorRepo;

    /** @brief Reference to the archive instructor repository */
    IRepository<InstructorPtr>& archiveInstructorRepo;

public:
    /**
     * @brief Constructs the InstructorManager with the required repositories.
     * @param repo Reference to an implementation of IRepository managing InstructorPtr.
     * @param archiveRepo Reference to an implementation of IRepository managing InstructorPtr.
     */
    InstructorManager(IRepository<InstructorPtr>& repo, IRepository<InstructorPtr>& archiveRepo);

    ~InstructorManager() = default;

    /**
     * @brief Filters instructors based on a custom predicate function.
     * @param predicate A callable boolean filter evaluation function.
     * @return std::vector<InstructorPtr> List of instructors matching the predicate.
     */
    [[nodiscard]] std::vector<InstructorPtr> findInstructors(const InstructorPredicate& predicate) const;

    /**
     * @brief Filters archive instructors based on a custom predicate function.
     * @param predicate A callable boolean filter evaluation function.
     * @return std::vector<InstructorPtr> List of archive instructors matching the predicate.
     */
    [[nodiscard]] std::vector<InstructorPtr> findArchiveInstructors(const InstructorPredicate& predicate) const;


    /**
     * @brief Retrieves all instructors currently registered in the system.
     * @return std::vector<InstructorPtr> List containing all instructors.
     */
    [[nodiscard]] std::vector<InstructorPtr> findAllInstructors() const;

    /**
     * @brief Retrieves all instructors unregistered in the system.
     * @return std::vector<InstructorPtr> List containing all instructors.
     */
    [[nodiscard]] std::vector<InstructorPtr> findAllArchiveInstructors() const;

    /**
     * @brief Registers a new instructor in the repository using a generated unique ID.
     * @param instructor Instructor shared_pointer, which will be registered into system.
     */
    void registerInstructor(InstructorPtr instructor);

    /**
     * @brief Unregisters instructor which is already in the repository (transfers the instructor to archive repo).
     * @param instructorID id of instructor which is currently in instructorRepo.
     */
    void unregisterInstructor(int instructorID);

    /**
    * @brief Allows for broadening instructors competence.
    * @param instructorID id of instructor which is currently in instructorRepo.
    * @param category category which given instructor will be able to start teaching.
    */
    void assignToCategory(int instructorID, Category category);

    /**
     * @brief Generates a text report of all instructors.
     * @return std::string Formatted report string.
     */
    [[nodiscard]] std::string getInstructorReport() const;

    /**
     * @brief Generates a text report of all archive instructors.
     * @return std::string Formatted report string.
     */
    [[nodiscard]] std::string getArchiveInstructorReport() const;

    /**
     * @brief Retrieves an instructor entity by its unique identifier.
     * @param instructorId The unique ID of the target instructor.
     * @return InstructorPtr A shared pointer to the Instructor object, or nullptr if not found.
     */
    [[nodiscard]] InstructorPtr getInstructorById(int instructorId) const;

    /**
    * @brief Retrieves an archive instructor entity by its unique identifier.
    * @param instructorId The unique ID of the target instructor.
    * @return InstructorPtr A shared pointer to the Instructor object, or nullptr if not found.
    */
    [[nodiscard]] InstructorPtr getArchiveInstructorById(int instructorId) const;
};