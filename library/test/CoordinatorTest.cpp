#include <boost/test/unit_test.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

#include "managers/Coordinator.h"
#include "managers/StudentManager.h"
#include "managers/InstructorManager.h"
#include "managers/VehicleManager.h"
#include "managers/CourseManager.h"
#include "managers/DrivingSessionManager.h"

#include "repositories/StudentRepository.h"
#include "repositories/InstructorRepository.h"
#include "repositories/VehicleRepository.h"
#include "repositories/CourseRepository.h"
#include "repositories/DrivingSessionRepository.h"
#include "repositories/IDataStorage.h"

#include "model/Student.h"
#include "model/Instructor.h"
#include "model/PassengerCar.h"
#include "model/Course.h"
#include "model/Address.h"
#include "model/Category.h"
#include "model/DrivingSession.h"

template <typename T>
class FakeStorageCoord : public IDataStorage<T> {
public:
    void save(const std::vector<T>&) override {}
    std::vector<T> load() override { return std::vector<T>(); }
};

struct CoordinatorFixture {
    FakeStorageCoord<DrivingSessionPtr> dsStorage;
    FakeStorageCoord<StudentPtr> sStorage;
    FakeStorageCoord<InstructorPtr> iStorage;
    FakeStorageCoord<VehiclePtr> vStorage;
    FakeStorageCoord<CoursePtr> cStorage;

    DrivingSessionRepository activeDsRepo;
    DrivingSessionRepository archiveDsRepo;
    StudentRepository studentRepo;
    StudentRepository archiveStudentRepo;
    InstructorRepository instructorRepo;
    InstructorRepository archiveInstructorRepo;
    VehicleRepository vehicleRepo;
    CourseRepository courseRepo;
    CourseRepository archiveCourseRepo;

    StudentManager studentManager;
    InstructorManager instructorManager;
    DrivingSessionManager drivingSessionManager;
    VehicleManager vehicleManager;
    CourseManager courseManager;

    Coordinator coordinator;

    StudentPtr mockStudent;
    InstructorPtr mockInstructor;
    VehiclePtr mockVehicle;
    CoursePtr mockCourse;

    CoordinatorFixture()
        : activeDsRepo("fake1.txt", dsStorage), archiveDsRepo("fake2.txt", dsStorage),
          studentRepo("fake3.txt", sStorage), archiveStudentRepo("fake4.txt", sStorage),
          instructorRepo("fake5.txt", iStorage), archiveInstructorRepo("fake6.txt", iStorage),
          vehicleRepo("fake7.txt", vStorage),
          courseRepo("fake8.txt", cStorage), archiveCourseRepo("fake9.txt", cStorage),
          studentManager(studentRepo, archiveStudentRepo),
          instructorManager(instructorRepo, archiveInstructorRepo),
          drivingSessionManager(activeDsRepo, archiveDsRepo),
          vehicleManager(vehicleRepo),
          courseManager(courseRepo, archiveCourseRepo),
          coordinator(instructorManager, studentManager, drivingSessionManager, vehicleManager, courseManager) 
    {
        boost::gregorian::date birthDate(1995, boost::gregorian::Oct, 10);
        auto fakeAddress = std::make_shared<Address>("Lodz", "Politechniki", "1");

        mockStudent = std::make_shared<Student>(101, "Jan", "Kowalski", "123456789", birthDate, fakeAddress, "PKK101");
        mockInstructor = std::make_shared<Instructor>(202, "Piotr", "Nowak", "987654321", birthDate, fakeAddress);
        mockInstructor->addQualification(Category::B);
        
        mockVehicle = std::make_shared<PassengerCar>(303, "EL-AUTO");

        studentManager.registerStudent(mockStudent);
        instructorManager.registerInstructor(mockInstructor);
        vehicleManager.registerVehicle(mockVehicle);
        
        courseManager.registerCourse(mockStudent, mockInstructor, Category::B);
        mockCourse = courseManager.findAllCourses()[0];
    }
};

BOOST_FIXTURE_TEST_SUITE(CoordinatorTestSuite, CoordinatorFixture)

    BOOST_AUTO_TEST_CASE(CalculateTotalCourseCost_TEST) {
        boost::posix_time::ptime start(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(10));
        boost::posix_time::ptime end(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(12));

        auto session = std::make_shared<DrivingSession>(1, start, end, mockInstructor, mockStudent, mockCourse->getId(), mockVehicle);
        activeDsRepo.add(session);

        double expectedCost = 2.0 * 80.0;
        double actualCost = coordinator.calculateTotalCourseCost(mockCourse->getId());

        BOOST_CHECK_CLOSE(expectedCost, actualCost, 0.01);
    }

    BOOST_AUTO_TEST_CASE(GetStudentHours_TEST) {
        boost::posix_time::ptime start1(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(10));
        boost::posix_time::ptime end1(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(11));
        
        boost::posix_time::ptime start2(boost::gregorian::date(2026, 6, 11), boost::posix_time::hours(10));
        boost::posix_time::ptime end2(boost::gregorian::date(2026, 6, 11), boost::posix_time::hours(12));

        auto session1 = std::make_shared<DrivingSession>(1, start1, end1, mockInstructor, mockStudent, mockCourse->getId(), mockVehicle);
        auto session2 = std::make_shared<DrivingSession>(2, start2, end2, mockInstructor, mockStudent, mockCourse->getId(), mockVehicle);
        
        activeDsRepo.add(session1);
        activeDsRepo.add(session2);

        double hours = coordinator.getStudentHours(101);

        BOOST_CHECK_CLOSE(3.0, hours, 0.01);
    }

    BOOST_AUTO_TEST_CASE(CompleteCourse_TEST) {
        int courseId = mockCourse->getId();
        BOOST_REQUIRE(!mockCourse->getIsFinished());

        coordinator.completeCourse(courseId);

        BOOST_CHECK(mockCourse->getIsFinished());
    }

    BOOST_AUTO_TEST_CASE(CalculateRevenue_TEST) {
            boost::posix_time::ptime start(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(10));
            boost::posix_time::ptime end(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(12));

            // Dodajemy sesję do pierwszego kursu (2 godziny * 80.0 PLN za auto = 160.0 PLN)
            auto session1 = std::make_shared<DrivingSession>(1, start, end, mockInstructor, mockStudent, mockCourse->getId(), mockVehicle);
            activeDsRepo.add(session1);

            double revenue = coordinator.calculateRevenue();
            BOOST_CHECK_CLOSE(160.0, revenue, 0.01);
        }

        BOOST_AUTO_TEST_CASE(CheckStudentCourseBalance_TEST) {
            boost::posix_time::ptime start(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(14));
            boost::posix_time::ptime end(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(15)); // 1 godzina

            auto session = std::make_shared<DrivingSession>(3, start, end, mockInstructor, mockStudent, mockCourse->getId(), mockVehicle);
            activeDsRepo.add(session);

            double balance = coordinator.checkStudentCourseBalance(101);
            BOOST_CHECK_CLOSE(80.0, balance, 0.01);

            BOOST_CHECK_CLOSE(0.0, coordinator.checkStudentCourseBalance(999), 0.01);
        }

        BOOST_AUTO_TEST_CASE(GetTotalHoursForCourse_TEST) {
            boost::posix_time::ptime start(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(8));
            boost::posix_time::ptime end(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(10)); // 2 godziny

            auto session = std::make_shared<DrivingSession>(4, start, end, mockInstructor, mockStudent, mockCourse->getId(), mockVehicle);
            activeDsRepo.add(session);

            double hours = coordinator.getTotalHoursForCourse(mockCourse->getId());
            BOOST_CHECK_CLOSE(2.0, hours, 0.01);
        }

        BOOST_AUTO_TEST_CASE(GetAvailableVehicles_TEST) {
            boost::posix_time::ptime searchStart(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(10));
            boost::posix_time::ptime searchEnd(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(12));
            TimeRange range(searchStart, searchEnd);

            auto mockVehicle2 = std::make_shared<PassengerCar>(304, "EL-FREE");
            vehicleManager.registerVehicle(mockVehicle2);

            auto session = std::make_shared<DrivingSession>(10, searchStart, searchEnd, mockInstructor, mockStudent, mockCourse->getId(), mockVehicle);
            activeDsRepo.add(session);

            auto availableVehicles = coordinator.getAvailableVehicles(Category::B, range);

            BOOST_REQUIRE(availableVehicles.size() == 1);
            BOOST_CHECK(availableVehicles[0]->getId() == 304);
        }

        BOOST_AUTO_TEST_CASE(GetAvailableInstructors_TEST) {
            boost::posix_time::ptime searchStart(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(10));
            boost::posix_time::ptime searchEnd(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(12));
            TimeRange range(searchStart, searchEnd);

            auto fakeAddress = std::make_shared<Address>("Lodz", "Politechniki", "1");
            boost::gregorian::date birthDate(1995, boost::gregorian::Oct, 10);

            // Dodajemy wolnego instruktora Kat B
            auto freeInstructor = std::make_shared<Instructor>(203, "Adam", "Wolny", "111", birthDate, fakeAddress);
            freeInstructor->addQualification(Category::B);
            instructorManager.registerInstructor(freeInstructor);

            auto freeInstructorCatC = std::make_shared<Instructor>(204, "Marek", "Inny", "222", birthDate, fakeAddress);
            freeInstructorCatC->addQualification(Category::C);
            instructorManager.registerInstructor(freeInstructorCatC);

            auto session = std::make_shared<DrivingSession>(11, searchStart, searchEnd, mockInstructor, mockStudent, mockCourse->getId(), mockVehicle);
            activeDsRepo.add(session);

            auto availableInstructors = coordinator.getAvailableInstructors(Category::B, range);

            BOOST_REQUIRE(availableInstructors.size() == 1);
            BOOST_CHECK(availableInstructors[0]->getId() == 203);
        }

        BOOST_AUTO_TEST_CASE(GetEntitiesById_TEST) {
            BOOST_CHECK(coordinator.getStudentById(101) != nullptr);
            BOOST_CHECK(coordinator.getInstructorById(202) != nullptr);
            BOOST_CHECK(coordinator.getVehicleById(303) != nullptr);

            int courseId = mockCourse->getId();
            BOOST_CHECK(coordinator.getCourseById(courseId) != nullptr);

            boost::posix_time::ptime start(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(10));
            boost::posix_time::ptime end(boost::gregorian::date(2026, 6, 10), boost::posix_time::hours(12));
            auto session = std::make_shared<DrivingSession>(99, start, end, mockInstructor, mockStudent, courseId, mockVehicle);
            activeDsRepo.add(session);
            BOOST_CHECK(coordinator.getDrivingSessionById(99) != nullptr);

            BOOST_CHECK(coordinator.getStudentById(999) == nullptr);
            BOOST_CHECK(coordinator.getInstructorById(999) == nullptr);
            BOOST_CHECK(coordinator.getVehicleById(999) == nullptr);
            BOOST_CHECK(coordinator.getCourseById(999) == nullptr);
            BOOST_CHECK(coordinator.getDrivingSessionById(999) == nullptr);
        }

        BOOST_AUTO_TEST_CASE(GetCourseIDByStudentID_TEST) {
            int foundCourseId = coordinator.getCourseIDByStudentID(101);
            BOOST_CHECK(foundCourseId == mockCourse->getId());

            int notFoundId = coordinator.getCourseIDByStudentID(999);
            BOOST_CHECK(notFoundId == -1);
        }

BOOST_AUTO_TEST_SUITE_END()