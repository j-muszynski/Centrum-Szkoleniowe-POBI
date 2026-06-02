#pragma once
#include "AbstractFileRepository.h"
#include "model/Vehicle.h"
#include "typedefs.h"

/**
 * @file VehicleRepository.h
 * @brief Repository class for managing Vehicle entities in a file.
 */

/**
 * @class VehicleRepository
 * @brief Handles a collection of Vehicle objects.
 * @details This class extends AbstractFileRepository specifically for VehiclePtr.
 * It adds a special method to create text reports of all vehicles.
 */
class VehicleRepository : public AbstractFileRepository<VehiclePtr> {
public:
    /**
     * @brief Creates a Vehicle repository.
     * @param filePath Path to the file where vehicles are saved.
     * @param storage The storage interface used to save and load Vehicle data.
     */
    VehicleRepository(const std::string& filePath, IDataStorage<VehiclePtr>& storage);

    /**
     * @brief Generates a text report of all vehicles in the repository.
     * @return A string containing detailed information about every vehicle.
     */
    std::string reportVehicles() const;
};