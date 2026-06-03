#include "managers/VehicleManager.h"

#include "model/Student.h"

VehicleManager::VehicleManager(IRepository<VehiclePtr>& vehicleRepo)
    : vehicleRepo(vehicleRepo) {}

std::vector<VehiclePtr> VehicleManager::findVehicles(const VehiclePredicate& predicate) const {
    return vehicleRepo.findBy(predicate);
}


std::vector<VehiclePtr> VehicleManager::findAllVehicles() const {
    return vehicleRepo.findAll();
}


void VehicleManager::registerVehicle(VehiclePtr newVehicle) {
    if (newVehicle == nullptr) return;

    vehicleRepo.add(newVehicle);
}

std::string VehicleManager::getVehicleReport() const {
    std::string report;
    const auto& activeVehicles = vehicleRepo.findAll();

    if (activeVehicles.empty()) {
        report += "No active vehicles found.\n";
        return report;
    }

    for (const auto& vehicle : activeVehicles) {
        if (vehicle != nullptr) {
            report += vehicle->getVehicleInfo() + "\n";
        }
    }
    return report;
}

