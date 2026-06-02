#include "repositories/DrvingSessionRepository.h"

DrivingSessionRepository::DrivingSessionRepository(const std::string& filePath, IDataStorage<DrivingSessionPtr>& storage)
    : AbstractFileRepository<DrivingSessionPtr>(filePath, storage) {}

std::string DrivingSessionRepository::reportDrivingSession() const {
    std::string info;
    for (const auto& session : repo) {
        if (session != nullptr) {
            info += session->getInfo();
        }
    }
    return info;
}