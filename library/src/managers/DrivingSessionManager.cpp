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

/*
 * exceptions later on
 * add functionality to propose different vehicle or instructor
 * should student, instructor and vehicle be checked for existence?
 */
void DrivingSessionManager::scheduleDrivingSession(int studentID, int instructorID, int vehicleID, TimeRange timeRange,
                                                   Coordinator& coordinator) {

/*
    // ---- Validation existence of entities ----
    auto studentPtr = coordinator.getStudentById(studentID); // such methods do not exist (yet?)
    auto instructorPtr = coordinator.getInstructorById(instructorID);
    auto vehiclePtr = coordinator.getVehicleById(vehicleID);

    // if one of pointers is nullptr -> abort scheduling
    if (studentPtr == nullptr || instructorPtr == nullptr || vehiclePtr == nullptr) {
        return;
    }
*/

    // preparing time period
    boost::posix_time::time_period requestedPeriod(timeRange.first, timeRange.second);

    // getting all sessions to check for conflicts
    const auto& allSessions = drivingSessionRepo.findAll();

    bool conflict = false;
    for (const auto& session : allSessions) {
        if (session != nullptr && session->getStatus() != "Cancelled") {
            // time period for current session (for loop session)
            boost::posix_time::time_period sessionPeriod(session->getStartTime(), session->getEndTime());

            // looking for conflicts
            if (requestedPeriod.intersects(sessionPeriod)) {
                // Instructor conflict
                if (session->getInstructor()->getId() == instructorID) {
                    std::cout << "Instructor is already booked for this time period.\n";
                    conflict = true;
                }

                // Vehicle conflict
                if (session->getVehicle()->getId() == vehicleID) {
                    std::cout << "Vehicle is already booked for this time period.\n";
                    conflict = true;
                }

                // student conflict
                if (session->getStudent()->getId() == studentID) {
                    std::cout << "Student already has a session scheduled for this time period.\n";
                    conflict = true;
                }
            }
        }
    }

    // if conflict appeared, session cannot be scheduled
    if (conflict == true) {
        std::cout << "Sorry, no available dates. Please try different date\n";
        return;
    }

    // getting unique id for new driving session
    int newSessionId = drivingSessionRepo.getNextID();

    // Creating new DrivingSession object
    auto newSession = std::make_shared<DrivingSession>(
        newSessionId,
        timeRange.first,
        timeRange.second,
        instructorPtr,
        studentPtr,
        vehiclePtr
    );

    // adding to repo
    drivingSessionRepo.add(newSession);
}

void DrivingSessionManager::cancelSession(int drivingSessionID, Coordinator& coordinator) {
    auto drivingSessionPtr = coordinator.getDrivingSessionById(drivingSessionID); // such methods do not exist (yet)
    if (drivingSessionPtr == nullptr) {
        std::cout << "Such driving session does not exist in database!\n";
        return;
    }

    drivingSessionRepo.remove(drivingSessionID);
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
