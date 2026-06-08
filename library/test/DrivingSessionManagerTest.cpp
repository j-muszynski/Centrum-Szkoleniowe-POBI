#include <boost/test/unit_test.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

#include "managers/DrivingSessionManager.h"
#include "repositories/DrivingSessionRepository.h"
#include "managers/Coordinator.h"
#include "managers/StudentManager.h"
#include "managers/InstructorManager.h"
#include "managers/VehicleManager.h"
#include "managers/CourseManager.h"
#include "repositories/StudentRepository.h"
#include "repositories/InstructorRepository.h"
#include "repositories/VehicleRepository.h"
#include "repositories/CourseRepository.h"

#include "model/DrivingSession.h"
#include "model/Student.h"
#include "model/Instructor.h"
#include "model/PassengerCar.h"
#include "model/Course.h"
#include "model/Address.h"
#include "model/Category.h"
#include "model/Exceptions.h"

template <typename T>
class FakeStorage : public IDataStorage<T> {
public:
    void save([[maybe_unused]] const std::vector<T>& data) override {}
    std::vector<T> load() override { return std::vector<T>(); }
};

struct DrivingSessionManagerFixture {
    // Storages & Repositories needed for structural components
    FakeStorage<DrivingSessionPtr> dsStorage;
    FakeStorage<StudentPtr> sStorage;
    FakeStorage<InstructorPtr> iStorage;
    FakeStorage<VehiclePtr> vStorage;
    FakeStorage<CoursePtr> cStorage;

    DrivingSessionRepository activeDsRepo;
    DrivingSessionRepository archiveDsRepo;
    StudentRepository studentRepo;
    StudentRepository archiveStudentRepo;
    InstructorRepository instructorRepo;
    InstructorRepository archiveInstructorRepo;
    VehicleRepository vehicleRepo;
    CourseRepository courseRepo;
    CourseRepository archiveCourseRepo;

    // Managers required by Coordinator
    StudentManager studentManager;
    InstructorManager instructorManager;
    DrivingSessionManager drivingSessionManager;
    VehicleManager vehicleManager;
    CourseManager courseManager;

    Coordinator coordinator;

    // Mock entity data pointers
    StudentPtr mockStudent;
    InstructorPtr mockInstructor;
    VehiclePtr mockVehicle;
    CoursePtr mockCourse;

    DrivingSessionManagerFixture()
        : activeDsRepo("fakeDSActive.txt", dsStorage),
          archiveDsRepo("fakeDSArchive.txt", dsStorage),
          studentRepo("fakeStudentActive.txt", sStorage),
          archiveStudentRepo("fakeStudentArchive.txt", sStorage),
          instructorRepo("fakeInsActive.txt", iStorage),
          archiveInstructorRepo("fakeInsArchive.txt", iStorage),
          vehicleRepo("fakeVehivcle.txt", vStorage),
          courseRepo("fakeCourseActive.txt", cStorage),
          archiveCourseRepo("fakeArchiveCourse.txt", cStorage),
          studentManager(studentRepo, archiveStudentRepo),
          instructorManager(instructorRepo, archiveInstructorRepo),
          drivingSessionManager(activeDsRepo, archiveDsRepo),
          vehicleManager(vehicleRepo),
          courseManager(courseRepo, archiveCourseRepo),
          coordinator(instructorManager, studentManager, drivingSessionManager, vehicleManager, courseManager) {
        // Core component construction data
        boost::gregorian::date commonBirthDate(1995, boost::gregorian::Oct, 10);
        auto fakeAddress = std::make_shared<Address>("Lodz", "Politechniki", "1");

        // Initializing concrete mock elements
        mockStudent = std::make_shared<Student>(101, "Jan", "Kowalski", "PESEL1", commonBirthDate, fakeAddress,
                                                "PKK101");
        mockInstructor = std::make_shared<Instructor>(202, "Piotr", "Nowak", "PESEL2", commonBirthDate, fakeAddress);
        mockVehicle = std::make_shared<PassengerCar>(303, "EL-AUTO");

        mockCourse = std::make_shared<Course>(courseRepo.getNextID(), Category::B, mockInstructor, mockStudent);

        // feeding managers with some data
        studentManager.registerStudent(mockStudent);
        instructorManager.registerInstructor(mockInstructor);
        vehicleManager.registerVehicle(mockVehicle);
        courseRepo.add(mockCourse); // direct injection to match coordinator queries
    }
};

/**
 * @test Checks different possibilities and verifies the correctness of the DrivingSessionManager class.
 */
BOOST_FIXTURE_TEST_SUITE(DrivingSessionManagerTestSuite, DrivingSessionManagerFixture)

    /**
     * @test Verifies that a valid session can be successfully scheduled if there are no conflicts.
     */
    BOOST_AUTO_TEST_CASE(ScheduleDrivingSession_Success_TEST) {
        // must be empty at the start
        BOOST_REQUIRE(drivingSessionManager.findAllDrivingSessions().empty());

        // declaring timeRange
        boost::posix_time::ptime start(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(10));
        boost::posix_time::ptime end(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(12));
        TimeRange desiredTime(start, end);

        // scheduling session
        drivingSessionManager.scheduleDrivingSession(101, 202, 303, desiredTime, coordinator);

        // was session scheduled
        BOOST_REQUIRE(drivingSessionManager.findAllDrivingSessions().size() == 1);
        auto session = drivingSessionManager.findAllDrivingSessions()[0];

        BOOST_REQUIRE(session != nullptr);
        BOOST_CHECK(session->getStudent()->getId() == 101);
        BOOST_CHECK(session->getInstructor()->getId() == 202);
        BOOST_CHECK(session->getVehicle()->getId() == 303);
    }

    /**
     * @test Verifies validation within scheduling mechanics.
     * Assures that non-existing Entity IDs cause abortion without component crashes.
     */
    BOOST_AUTO_TEST_CASE(ScheduleDrivingSession_InvalidEntities_TEST) {
        boost::posix_time::ptime start(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(10));
        boost::posix_time::ptime end(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(12));
        TimeRange desiredTime(start, end);

        // Target non-existing student (999)
        BOOST_CHECK_THROW(
            drivingSessionManager.scheduleDrivingSession(999, 202, 303, desiredTime, coordinator),
            NotFoundException
        );
        BOOST_CHECK(drivingSessionManager.findAllDrivingSessions().empty());

        // Target non-existing instructor (999)
        BOOST_CHECK_THROW(
            drivingSessionManager.scheduleDrivingSession(101, 999, 303, desiredTime, coordinator),
            NotFoundException
        );
        BOOST_CHECK(drivingSessionManager.findAllDrivingSessions().empty());

        // Target non-existing vehicle (999)
        BOOST_CHECK_THROW(
            drivingSessionManager.scheduleDrivingSession(101, 202, 999, desiredTime, coordinator),
            NotFoundException
        );
        BOOST_CHECK(drivingSessionManager.findAllDrivingSessions().empty());
    }

    /**
     * @test Checks overlap collision strategy handling for active allocations.
     * Assures that blocking a resource (Instructor / Vehicle / Student) stops double booking during overlapping hours.
     */
    BOOST_AUTO_TEST_CASE(ScheduleDrivingSession_TimeConflict_TEST) {
        boost::posix_time::ptime start1(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(10));
        boost::posix_time::ptime end1(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(12));
        TimeRange sessionOneTime(start1, end1);

        drivingSessionManager.scheduleDrivingSession(101, 202, 303, sessionOneTime, coordinator);
        BOOST_REQUIRE(drivingSessionManager.findAllDrivingSessions().size() == 1);

        // Attempting to schedule a conflicting second slot (11:00 - 13:00 intersects 10:00 - 12:00)
        boost::posix_time::ptime start2(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(11));
        boost::posix_time::ptime end2(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(13));
        TimeRange conflictingTime(start2, end2);

        // Registering a separate alternative student to isolate Instructor/Vehicle collision locks
        auto secondStudent = std::make_shared<Student>(777, "Anna", "Nowak", "PESEL3",
                                                       boost::gregorian::date(1998, 1, 1), mockStudent->getAddress(),
                                                       "PKK777");
        studentManager.registerStudent(secondStudent);
        auto secondCourse = std::make_shared<Course>(505, Category::B, mockInstructor, secondStudent);
        courseRepo.add(secondCourse);

        BOOST_CHECK_THROW(
            drivingSessionManager.scheduleDrivingSession(777, 202, 303, conflictingTime, coordinator),
            ValidationException
        );

        // Verify that the conflicting transaction was aborted
        BOOST_CHECK(drivingSessionManager.findAllDrivingSessions().size() == 1);

        // Attempting to schedule a driving session right after another (12:00 - 14:00 should not intersect 10:00 - 12:00)
        boost::posix_time::ptime start3(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(12));
        boost::posix_time::ptime end3(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(14));
        TimeRange noConflictingTime(start3, end3);

        drivingSessionManager.scheduleDrivingSession(777, 202, 303, noConflictingTime, coordinator);
        // Verify that the conflicting transaction was not aborted
        BOOST_CHECK(drivingSessionManager.findAllDrivingSessions().size() == 2);
    }

    /**
     * @test check if time limitation works properly (driving session can last [1;8]h)
     */
    BOOST_AUTO_TEST_CASE(ScheduleDrivingSession_TimeLimitation_TEST) {
        boost::posix_time::ptime start1(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(10));
        boost::posix_time::ptime end1(boost::gregorian::date(2026, 6, 10),
                                      boost::posix_time::hours(10) + boost::posix_time::minutes(10));
        TimeRange sessionOneTime(start1, end1);

        // session too short (fail)
        BOOST_CHECK_THROW(
            drivingSessionManager.scheduleDrivingSession(101, 202, 303, sessionOneTime, coordinator),
            ValidationException
        );
        BOOST_REQUIRE(drivingSessionManager.findAllDrivingSessions().empty());

        boost::posix_time::ptime start2(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(11));
        boost::posix_time::ptime end2(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(20));
        TimeRange sessionTwoTime(start2, end2);

        // session too long (fail)
        BOOST_CHECK_THROW(
            drivingSessionManager.scheduleDrivingSession(101, 202, 303, sessionTwoTime, coordinator),
            ValidationException
        );
        BOOST_REQUIRE(drivingSessionManager.findAllDrivingSessions().empty());

        boost::posix_time::ptime start3(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(11));
        boost::posix_time::ptime end3(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(15));
        TimeRange sessionThreeTime(start3, end3);

        // correct session time (success)
        drivingSessionManager.scheduleDrivingSession(101, 202, 303, sessionThreeTime, coordinator);
        BOOST_REQUIRE(!drivingSessionManager.findAllDrivingSessions().empty());
    }


    /**
     * @test Verifies implementation criteria matching lookups by unique identifier tokens.
     */
    BOOST_AUTO_TEST_CASE(GetDrivingSessionById_TEST) {
        boost::posix_time::ptime start(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(14));
        boost::posix_time::ptime end(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(15));
        drivingSessionManager.scheduleDrivingSession(101, 202, 303, TimeRange(start, end), coordinator);

        auto generatedSession = drivingSessionManager.findAllDrivingSessions()[0];
        int allocatedId = generatedSession->getId();

        auto foundSession = drivingSessionManager.getDrivingSessionById(allocatedId);
        BOOST_REQUIRE(foundSession != nullptr);
        BOOST_CHECK(foundSession->getId() == allocatedId);

        BOOST_CHECK(drivingSessionManager.getDrivingSessionById(999) == nullptr);
    }

    /**
     * @test Tests active data stack query filtering predicates.
     */
    BOOST_AUTO_TEST_CASE(FindDrivingSessions_WithPredicate_TEST) {
        boost::posix_time::ptime start(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(8));
        boost::posix_time::ptime end(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(9));
        drivingSessionManager.scheduleDrivingSession(101, 202, 303, TimeRange(start, end), coordinator);

        DrivingSessionPredicate linkedToCourse1 = [](const DrivingSessionPtr& session) {
            return session->getCourseId() == 1;
        };

        std::vector<DrivingSessionPtr> findings = drivingSessionManager.findDrivingSessions(linkedToCourse1);
        BOOST_REQUIRE(findings.size() == 1);
        BOOST_CHECK(findings[0]->getCourseId() == 1);
    }

    /**
     * @test Verifies structural removal capabilities inside the cancelSession implementation.
     */
    BOOST_AUTO_TEST_CASE(CancelSession_TEST) {
        boost::posix_time::ptime start(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(8));
        boost::posix_time::ptime end(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(9));
        drivingSessionManager.scheduleDrivingSession(101, 202, 303, TimeRange(start, end), coordinator);

        auto activeSession = drivingSessionManager.findAllDrivingSessions()[0];
        int sessionId = activeSession->getId();

        drivingSessionManager.cancelSession(sessionId, coordinator);
        BOOST_CHECK(drivingSessionManager.findAllDrivingSessions().empty());

        BOOST_CHECK_THROW(
            drivingSessionManager.cancelSession(999, coordinator),
            NotFoundException
        );
        BOOST_CHECK_THROW(
            drivingSessionManager.cancelSession(-5, coordinator),
            InvalidInputException
        );
    }

    /**
     * @test Ensures report strings are generated.
     */
    BOOST_AUTO_TEST_CASE(GetDrivingSessionReports_TEST) {
        std::string emptyReport = drivingSessionManager.getDrivingSessionReport();
        std::string emptyArchiveReport = drivingSessionManager.getArchiveDrivingSessionReport();

        boost::posix_time::ptime start(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(8));
        boost::posix_time::ptime end(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(9));
        drivingSessionManager.scheduleDrivingSession(101, 202, 303, TimeRange(start, end), coordinator);

        BOOST_REQUIRE(!drivingSessionManager.findAllDrivingSessions().empty());

        std::string filledReport = drivingSessionManager.getDrivingSessionReport();
        BOOST_CHECK(!filledReport.empty());

        DrivingSessionPtr session = drivingSessionManager.findAllDrivingSessions()[0];
        int id = session->getId();

        drivingSessionManager.setDrivingSessionAsCompleted(id);

        filledReport = drivingSessionManager.getArchiveDrivingSessionReport();
        BOOST_CHECK(!filledReport.empty());
    }

    /**
     * @test validates that completed driving session is moved to archived repository and changes it's status
     */
    BOOST_AUTO_TEST_CASE(setDrivingSessionAsCompleted_TEST) {
        boost::posix_time::ptime start(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(8));
        boost::posix_time::ptime end(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(9));

        // repos are empty at the start
        BOOST_CHECK(drivingSessionManager.findAllDrivingSessions().empty());
        BOOST_CHECK(drivingSessionManager.findAllArchiveDrivingSessions().empty());

        drivingSessionManager.scheduleDrivingSession(101, 202, 303, TimeRange(start, end), coordinator);

        // making sure that session was successfully scheduled
        BOOST_REQUIRE(!drivingSessionManager.findAllDrivingSessions().empty());

        int drivingSessionID = drivingSessionManager.findAllDrivingSessions()[0]->getId();
        DrivingSessionPtr session = drivingSessionManager.getDrivingSessionById(drivingSessionID);

        // moving session to archive repo
        drivingSessionManager.setDrivingSessionAsCompleted(drivingSessionID);

        BOOST_CHECK(drivingSessionManager.findAllDrivingSessions().empty());
        BOOST_CHECK(!drivingSessionManager.findAllArchiveDrivingSessions().empty());
        session = drivingSessionManager.findAllArchiveDrivingSessions()[0];

        // verifying the report for archive repo
        BOOST_REQUIRE(session != nullptr);
        BOOST_CHECK(session->getStatus() == "COMPLETED");

        BOOST_CHECK_THROW(
            drivingSessionManager.setDrivingSessionAsCompleted(999),
            NotFoundException
        );

        BOOST_CHECK_THROW(
            drivingSessionManager.setDrivingSessionAsCompleted(-10),
            InvalidInputException
        );
    }

BOOST_AUTO_TEST_SUITE_END()
