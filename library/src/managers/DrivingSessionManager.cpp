#include "managers/DrivingSessionManager.h"
#include "repositories/DrivingSessionRepository.h"
#include "model/Exceptions.h"
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

void DrivingSessionManager::scheduleDrivingSession(int studentID, int instructorID, int vehicleID, TimeRange timeRange,
                                                   Coordinator& coordinator) {
    if (timeRange.first >= timeRange.second) {
        throw InvalidInputException("Invalid time range: Start time must be before end time.");
    }

    boost::posix_time::time_duration duration = timeRange.second - timeRange.first;

    // checking if session will be at least an hour long
    if (duration < boost::posix_time::hours(1)) {
        throw ValidationException("Driving session cannot be shorter than 1 hour (60 minutes).");
    }

    // checking if session will be shorter than 8h
    if (duration > boost::posix_time::hours(8)) {
        throw ValidationException("Driving session cannot be longer than 8 hours.");
    }


    // ---- Validation existence of entities ----
    auto studentPtr = coordinator.getStudentById(studentID); // such methods do not exist (yet?)
    auto instructorPtr = coordinator.getInstructorById(instructorID);
    auto vehiclePtr = coordinator.getVehicleById(vehicleID);

    // if one of pointers is nullptr -> abort scheduling
    if (studentPtr == nullptr) {
        throw NotFoundException("Scheduling failed: Student with ID " + std::to_string(studentID) + " not found.");
    }
    if (instructorPtr == nullptr) {
        throw NotFoundException(
            "Scheduling failed: Instructor with ID " + std::to_string(instructorID) + " not found.");
    }
    if (vehiclePtr == nullptr) {
        throw NotFoundException("Scheduling failed: Vehicle with ID " + std::to_string(vehicleID) + " not found.");
    }

    // preparing time period
    boost::posix_time::time_period requestedPeriod(timeRange.first, timeRange.second);

    // getting all sessions to check for conflicts
    const auto& allSessions = drivingSessionRepo.findAll();

    bool conflict = false;
    std::string conflictMessage;
    for (const auto& session : allSessions) {
        if (session != nullptr && session->getStatus() != "Cancelled") {
            // time period for current session (for loop session)
            boost::posix_time::time_period sessionPeriod(session->getStartTime(), session->getEndTime());

            // looking for conflicts
            if (requestedPeriod.intersects(sessionPeriod)) {
                // Instructor conflict
                if (session->getInstructor()->getId() == instructorID) {
                    conflictMessage += "Instructor is already booked. ";
                    conflict = true;
                }

                // Vehicle conflict
                if (session->getVehicle()->getId() == vehicleID) {
                    conflictMessage += "Vehicle is already booked. ";
                    conflict = true;
                }

                // student conflict
                if (session->getStudent()->getId() == studentID) {
                    conflictMessage += "Student already has a scheduled session. ";
                    conflict = true;
                }
            }
        }
    }

    // if conflict appeared, session cannot be scheduled
    if (conflict) {
        throw ValidationException(conflictMessage + "Please choose a different date or time.");
    }

    // getting unique id for new driving session
    int newSessionId = drivingSessionRepo.getNextID();
    int courseID = coordinator.getCourseIDByStudentID(studentID);

    if (courseID == -1) {
        throw NotFoundException("Cannot schedule session: Student with ID " + std::to_string(studentID) + " is not assigned to any active course.");
    }

    // Creating new DrivingSession object
    auto newSession = std::make_shared<DrivingSession>(
        newSessionId,
        timeRange.first,
        timeRange.second,
        instructorPtr,
        studentPtr,
        courseID,
        vehiclePtr
    );

    // adding to repo
    drivingSessionRepo.add(newSession);
}

void DrivingSessionManager::cancelSession(int drivingSessionID, Coordinator& coordinator) {
    if (drivingSessionID < 0) {
        throw InvalidInputException("Driving session ID cannot be negative.");
    }

    auto drivingSessionPtr = coordinator.getDrivingSessionById(drivingSessionID);
    if (drivingSessionPtr == nullptr) {
        throw NotFoundException("Cannot cancel session: Driving session with ID " + std::to_string(drivingSessionID) + " does not exist.");
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

void DrivingSessionManager::setDrivingSessionAsCompleted(int drivingSessionID) {
    if (drivingSessionID < 0) {
        throw InvalidInputException("Driving session ID cannot be negative.");
    }

    DrivingSessionPtr session = getDrivingSessionById(drivingSessionID);
    if (session == nullptr) {
        throw NotFoundException("Cannot complete session: Active driving session with ID " + std::to_string(drivingSessionID) + " not found.");
    }
    session->setStatus("COMPLETED");

    drivingSessionRepo.remove(drivingSessionID);
    archiveDrivingSessionRepo.add(session);
}
