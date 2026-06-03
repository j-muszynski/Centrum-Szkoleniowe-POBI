#pragma once
#include "typedefs.h"
#include "repositories/IRepository.h"
#include "model/Vehicle.h"


/**
 * @class VehicleManager
 * @brief Handles business logic for vehicle lifecycles, registrations, and lookup queries.
 */
class VehicleManager {
private:
    /** @brief Reference to the  vehicles repository */
    IRepository<VehiclePtr>& vehicleRepo;

public:
    /**
     * @brief Constructs a VehicleManager with required data repositories.
     * @param vehicleRepo Reference to the repository handling vehicles.
     */
    VehicleManager(IRepository<VehiclePtr>& vehicleRepo);

    /**
     * @brief Destructor for VehicleManager.
     */
    ~VehicleManager() = default;

    /**
     * @brief Filters vehicles using a custom predicate function.
     * @param predicate A callable boolean filter.
     * @return std::vector<VehiclePtr> List of vehicles matching the predicate criteria.
     */
    std::vector<VehiclePtr> findVehicles(const VehiclePredicate& predicate) const;

    /**
     * @brief Retrieves all vehicles currently registered in the system.
     * @return std::vector<VehiclePtr> List containing all vehicles.
     */
    std::vector<VehiclePtr> findAllVehicles() const;

    /**
     * @brief Registers and creates a new vehicle session in the  repository.
     * @param newVehicle Pointer to vehicle which will be registered.
     */
    void registerVehicle(VehiclePtr newVehicle);

    /**
     * @brief Generates a text report of all managed vehicles.
     * @return std::string Formatted report containing details of each vehicle.
     */
    std::string getVehicleReport() const;
};
