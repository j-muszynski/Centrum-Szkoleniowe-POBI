#pragma once
#include "AbstractFileRepository.h"
#include "model/DrivingSession.h"
#include "typedefs.h"

/**
 * @file DrivingSessionRepository.h
 * @brief Repository class for managing DrivingSession entities in a file.
 */

/**
 * @class DrivingSessionRepository
 * @brief Handles a collection of DrivingSession objects.
 * @details This class extends AbstractFileRepository specifically for DrivingSessionPtr.
 * It adds a special method to create text reports of all driving sessions.
 */
class DrivingSessionRepository : public AbstractFileRepository<DrivingSessionPtr> {
public:
    /**
     * @brief Creates a DrivingSession repository.
     * @param filePath Path to the file where driving sessions are saved.
     * @param storage The storage interface used to save and load DrivingSession data.
     */
    DrivingSessionRepository(const std::string& filePath, IDataStorage<DrivingSessionPtr>& storage);

    /**
     * @brief Generates a text report of all driving sessions in the repository.
     * @return [[nodiscard]] A string containing detailed information about every driving session.
     */
    [[nodiscard]] std::string reportDrivingSession() const;
};