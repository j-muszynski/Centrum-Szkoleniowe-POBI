#include "managers/VehicleManager.h"
#include "repositories/VehicleRepository.h"

#include "model/Student.h"

VehicleManager::VehicleManager(IRepository<VehiclePtr>& vehicleRepo)
    : vehicleRepo(vehicleRepo) {}

std::vector<VehiclePtr> VehicleManager::findVehicles(const VehiclePredicate& predicate) const {
    return vehicleRepo.findBy(predicate);
}

std::vector<VehiclePtr> VehicleManager::findAllVehicles() const {
    return vehicleRepo.findAll();
}

void VehicleManager::registerVehicle(const VehiclePtr& newVehicle) {
    if (newVehicle == nullptr) return;

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
    VehiclePtr vehicle = getVehicleById(vehicleId);
    if (vehicle != nullptr) {
        vehicleRepo.remove(vehicleId);
    }
}
