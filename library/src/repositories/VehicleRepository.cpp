#include "repositories/VehicleRepository.h"

VehicleRepository::VehicleRepository(const std::string& filePath, IDataStorage<VehiclePtr>& storage)
    : AbstractFileRepository<VehiclePtr>(filePath, storage) {}

std::string VehicleRepository::reportVehicles() const {
    std::string info;
    for (const auto& vehicle : repo) {
        if (vehicle != nullptr) {
            info += vehicle->getVehicleInfo();
        }
    }
    return info;
}