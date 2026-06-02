#pragma once
#include "AbstractFileRepository.h"
#include "model/DrivingSession.h"

#include "typedefs.h"

class DrivingSessionRepository : public AbstractFileRepository<DrivingSessionPtr> {
public:
    DrivingSessionRepository(const std::string& filePath, IDataStorage<DrivingSessionPtr>& storage);
    std::string reportDrivingSession() const;
};