#pragma once
#include "typedefs.h"
#include "repositories/IRepository.h"
#include "model/DrivingSession.h"
#include "managers/Coordinator.h"


/**
 * @class DrivingSessionManager
 * @brief Handles business logic for drivingSession lifecycles, scheduling, canceling, and lookup queries.
 */
class DrivingSessionManager {
private:
    /** @brief Reference to the active drivingSessions repository */
    IRepository<DrivingSessionPtr>& drivingSessionRepo;

    /** @brief Reference to the archived (completed) drivingSessions repository */
    IRepository<DrivingSessionPtr>& archiveDrivingSessionRepo;

public:
    /**
     * @brief Constructs a DrivingSessionManager with required data repositories.
     * @param drivingSessionRepo Reference to the repository handling active drivingSessions.
     * @param archiveDrivingSessionRepo Reference to the repository handling archived drivingSessions.
     */
    DrivingSessionManager(IRepository<DrivingSessionPtr>& drivingSessionRepo,
                          IRepository<DrivingSessionPtr>& archiveDrivingSessionRepo);

    /**
     * @brief Destructor for DrivingSessionManager.
     */
    ~DrivingSessionManager() = default;

    /**
     * @brief Filters active drivingSessions using a custom predicate function.
     * @param predicate A callable boolean filter.
     * @return std::vector<DrivingSessionPtr> List of active drivingSessions matching the predicate criteria.
     */
    std::vector<DrivingSessionPtr> findDrivingSessions(const DrivingSessionPredicate& predicate) const;

    /**
     * @brief Filters archive drivingSessions using a custom predicate function.
     * @param predicate A callable boolean filter.
     * @return std::vector<DrivingSessionPtr> List of archive drivingSessions matching the predicate criteria.
     */
    std::vector<DrivingSessionPtr> findArchiveDrivingSessions(const DrivingSessionPredicate& predicate) const;

    /**
     * @brief Retrieves all active drivingSessions currently registered in the system.
     * @return std::vector<DrivingSessionPtr> List containing all active drivingSessions.
     */
    std::vector<DrivingSessionPtr> findAllDrivingSessions() const;

    /**
     * @brief Retrieves all archive drivingSessions currently registered in the system.
     * @return std::vector<DrivingSessionPtr> List containing all archive drivingSessions.
     */
    std::vector<DrivingSessionPtr> findAllArchiveDrivingSessions() const;

    /**
     * * @brief Schedules and creates a new drivingSession session in the active repository.
     * @param studentID id of student that is about to drive.
     * @param instructorID id of instructor that is supposed to lead the session.
     * @param vehicleID id of a vehicle that driving session is supposed to be on.
     * @param timeRange a pair of (boost::posix_time::ptime) start and end time of driving session.
     * @param coordinator reference to Coordinator which will check for availability of resources.
     */
    void scheduleDrivingSession(int studentID, int instructorID, int vehicleID, TimeRange timeRange,
                                Coordinator& coordinator);

    /**
     * @brief cancels session and frees the resources
     * @param drivingSessionID id of driving session that is supposed to be canceled.
     * @param coordinator reference to Coordinator which will free the resources.
     */
    void cancelSession(int drivingSessionID, Coordinator& coordinator);

    /**
     * @brief Generates a text report of all managed drivingSessions.
     * @return std::string Formatted report containing details of each drivingSession.
     */
    std::string getDrivingSessionReport() const;

    /**
     * @brief Generates a text report of all managed drivingSessions.
     * @return std::string Formatted report containing details of each drivingSession.
     */
    std::string getArchiveDrivingSessionReport() const;

    /**
     * @brief Retrieves a scheduled or currently lasting driving session entity by its unique identifier.
     * @param sessionId The unique ID of the target driving session.
     * @return DrivingSessionPtr A shared pointer to the DrivingSession object, or nullptr if not found.
     */
    DrivingSessionPtr getDrivingSessionById(int sessionId) const;

    /**
     * @brief Retrieves an archive driving session entity by its unique identifier.
     * @param sessionId The unique ID of the target driving session.
     * @return DrivingSessionPtr A shared pointer to the DrivingSession object, or nullptr if not found.
     */
    DrivingSessionPtr getArchiveDrivingSessionById(int sessionId) const;
};
