#include "managers/VehicleManager.h"
#include "repositories/VehicleRepository.h"
#include "model/Student.h"
#include "model/Exceptions.h"

VehicleManager::VehicleManager(IRepository<VehiclePtr>& vehicleRepo)
    : vehicleRepo(vehicleRepo) {}

std::vector<VehiclePtr> VehicleManager::findVehicles(const VehiclePredicate& predicate) const {
    return vehicleRepo.findBy(predicate);
}

std::vector<VehiclePtr> VehicleManager::findAllVehicles() const {
    return vehicleRepo.findAll();
}

void VehicleManager::registerVehicle(const VehiclePtr& newVehicle) {
    if (newVehicle == nullptr) {
        throw NullPointerException("Cannot register vehicle: Provided pointer is null.");
    }

    vehicleRepo.add(newVehicle);
}

std::string VehicleManager::getVehicleReport() const {
    const auto& concreteRepo = dynamic_cast<const VehicleRepository&>(vehicleRepo);

    return concreteRepo.reportVehicles();
}

VehiclePtr VehicleManager::getVehicleById(int vehicleId) const {
    return vehicleRepo.getById(vehicleId);
}

void VehicleManager::unregisterVehicle(int vehicleId) {
    if (vehicleId < 0) {
        throw InvalidInputException("Vehicle ID cannot be negative.");
    }
    VehiclePtr vehicle = getVehicleById(vehicleId);

    if (vehicle == nullptr) {
        throw NotFoundException("Cannot unregister vehicle: Vehicle with ID " + std::to_string(vehicleId) + " not found.");
    }

    vehicleRepo.remove(vehicleId);
}
