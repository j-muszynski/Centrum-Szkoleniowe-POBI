#include "managers/DrivingSessionManager.h"
#include "repositories/DrivingSessionRepository.h"

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
    const auto& concreteRepo = dynamic_cast<const DrivingSessionRepository&>(drivingSessionRepo);

    return concreteRepo.reportDrivingSession();
}

std::string DrivingSessionManager::getArchiveDrivingSessionReport() const {
    const auto& concreteRepo = dynamic_cast<const DrivingSessionRepository&>(archiveDrivingSessionRepo);

    return concreteRepo.reportDrivingSession();
}

DrivingSessionPtr DrivingSessionManager::getDrivingSessionById(int sessionId) const {
    return drivingSessionRepo.getById(sessionId);
}

DrivingSessionPtr DrivingSessionManager::getArchiveDrivingSessionById(int sessionId) const {
    return archiveDrivingSessionRepo.getById(sessionId);
}
