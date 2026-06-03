#include "managers/DrivingSessionManager.h"

#include "model/Student.h"

DrivingSessionManager::DrivingSessionManager(IRepository<DrivingSessionPtr>& drivingSessionRepo,
                                             IRepository<DrivingSessionPtr>& archiveDrivingSessionRepo)
    : drivingSessionRepo(drivingSessionRepo), archiveDrivingSessionRepo(archiveDrivingSessionRepo) {}

std::vector<DrivingSessionPtr> DrivingSessionManager::findDrivingSessions(
    const DrivingSessionPredicate& predicate) const {
    return drivingSessionRepo.findBy(predicate);
}

std::vector<DrivingSessionPtr> DrivingSessionManager::findArchiveDrivingSessions(
    const DrivingSessionPredicate& predicate) const {
    return archiveDrivingSessionRepo.findBy(predicate);
}

std::vector<DrivingSessionPtr> DrivingSessionManager::findAllDrivingSessions() const {
    return drivingSessionRepo.findAll();
}

std::vector<DrivingSessionPtr> DrivingSessionManager::findAllArchiveDrivingSessions() const {
    return archiveDrivingSessionRepo.findAll();
}

// logic for scheduling REQUIRED
void DrivingSessionManager::scheduleDrivingSession(int studentID, int instructorID, int vehicleID, TimeRange timeRange,
                                                   Coordinator& coordinator) {
    ;
}

// logic yet to come
void DrivingSessionManager::cancelSession(int drivingSessionID, Coordinator& coordinator) {
    ;
}

std::string DrivingSessionManager::getDrivingSessionReport() const {
    std::string report;
    const auto& activeDrivingSessions = drivingSessionRepo.findAll();

    if (activeDrivingSessions.empty()) {
        report += "No active drivingSessions found.\n";
        return report;
    }

    for (const auto& drivingSession : activeDrivingSessions) {
        if (drivingSession != nullptr) {
            report += drivingSession->getInfo() + "\n";
        }
    }
    return report;
}

std::string DrivingSessionManager::getArchiveDrivingSessionReport() const {
    std::string report;
    const auto& archivedDrivingSessions = archiveDrivingSessionRepo.findAll();

    if (archivedDrivingSessions.empty()) {
        report += "No archived drivingSessions found.\n";
        return report;
    }

    for (const auto& drivingSession : archivedDrivingSessions) {
        if (drivingSession != nullptr) {
            report += drivingSession->getInfo() + "\n";
        }
    }
    return report;
}
