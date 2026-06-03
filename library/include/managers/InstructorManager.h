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
    std::vector<InstructorPtr> findInstructors(const InstructorPredicate& predicate) const;

    /**
     * @brief Filters archive instructors based on a custom predicate function.
     * @param predicate A callable boolean filter evaluation function.
     * @return std::vector<InstructorPtr> List of archive instructors matching the predicate.
     */
    std::vector<InstructorPtr> findArchiveInstructors(const InstructorPredicate& predicate) const;


    /**
     * @brief Retrieves all instructors currently registered in the system.
     * @return std::vector<InstructorPtr> List containing all instructors.
     */
    std::vector<InstructorPtr> findAllInstructors() const;

    /**
     * @brief Retrieves all instructors unregistered in the system.
     * @return std::vector<InstructorPtr> List containing all instructors.
     */
    std::vector<InstructorPtr> findAllArchiveInstructors() const;
