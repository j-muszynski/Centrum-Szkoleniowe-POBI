#pragma once
#include "AbstractFileRepository.h"
#include "model/Vehicle.h"

#include "typedefs.h"

class VehicleRepository : public AbstractFileRepository<VehiclePtr> {
public:
    VehicleRepository(const std::string& filePath, IDataStorage<VehiclePtr>& storage);
    std::string reportVehicles() const;
};