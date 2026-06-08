#include <boost/test/unit_test.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

#include "managers/StudentManager.h"
#include "repositories/StudentRepository.h"
#include "model/Student.h"
#include "model/Address.h"
#include "model/Exceptions.h"

// fakeStorage class imitates FileStorage, but it stores data in a vector instead of a file
template <typename T>
class FakeStorage : public IDataStorage<T> {
public:
    void save([[maybe_unused]] const std::vector<T>& data) override {}
    std::vector<T> load() override { return std::vector<T>(); }
};

struct StudentManagerFixture {
    FakeStorage<StudentPtr> storage;
    StudentRepository activeRepo;
    StudentRepository archiveRepo;
    StudentManager manager;

    // shared parameters for object initializations
    boost::gregorian::date birthDate;
    std::shared_ptr<Address> fakeAddress;

    // fake data
    std::shared_ptr<Student> mockStudent;

    StudentManagerFixture()
        : activeRepo("fakeStudentsActive.txt", storage),
          archiveRepo("fakeStudentsArchive.txt", storage),
          manager(activeRepo, archiveRepo),
          birthDate(2002, boost::gregorian::Jan, 15) {
        fakeAddress = std::make_shared<Address>("Lodz", "Piotrkowska", "13/3");

        // Initialization of fake data
        mockStudent = std::make_shared<Student>(1, "Jan", "Kowalski", "PESEL1", birthDate, fakeAddress, "pkk1");
    }
};

/**
 * @test Checks different possibilities and verifies the correctness of the StudentManager class.
 */
BOOST_FIXTURE_TEST_SUITE(StudentManagerTestSuite, StudentManagerFixture)

    /**
     * @test Verifies that a valid student can be successfully registered into the active repository.
     * Also validates that passing a nullptr does not affect the repository.
     */
    BOOST_AUTO_TEST_CASE(RegisterStudent_TEST) {
        BOOST_REQUIRE(manager.findAllStudents().empty());

        // trying to register a nullptr student (should be ignored)
        BOOST_CHECK_THROW(
            manager.registerStudent(nullptr),
            NullPointerException
        );
        BOOST_CHECK(manager.findAllStudents().empty());

        // registering a correct student object
        manager.registerStudent(mockStudent);
        BOOST_REQUIRE(manager.findAllStudents().size() == 1);

        auto registeredStudent = manager.findAllStudents()[0];
        BOOST_REQUIRE(registeredStudent != nullptr);
        BOOST_CHECK(registeredStudent->getId() == 1);
        BOOST_CHECK(registeredStudent->getFirstName() == "Jan");
        BOOST_CHECK(registeredStudent->getLastName() == "Kowalski");
        BOOST_CHECK(!registeredStudent->getIsReadyForExam()); // default status should be false
    }

    /**
     * @test Verifies the unregisterStudent mechanism.
     * Ensures that calling this method removes the student from the active repository
     * and correctly moves the entity to the archive repository.
     */
    BOOST_AUTO_TEST_CASE(UnregisterStudent_TEST) {
        // prepare setup by registering a student
        manager.registerStudent(mockStudent);
        BOOST_REQUIRE(manager.findAllStudents().size() == 1);
        BOOST_REQUIRE(manager.findAllArchiveStudents().empty());

        // unregistering the student
        manager.unregisterStudent(1);

        // checks if the student was safely moved between repositories
        BOOST_CHECK(manager.findAllStudents().empty());
        BOOST_REQUIRE(manager.findAllArchiveStudents().size() == 1);

        auto archivedStudent = manager.findAllArchiveStudents()[0];
        BOOST_REQUIRE(archivedStudent != nullptr);
        BOOST_CHECK(archivedStudent->getId() == 1);

        BOOST_CHECK_THROW(
            manager.unregisterStudent(1),
            NotFoundException
        );
        BOOST_CHECK_THROW(
            manager.unregisterStudent(-1),
            InvalidInputException
        );
    }

    /**
     * @test Verifies the updateExamStatus functionality.
     * Validates that the student's exam readiness bool flag is modified to true,
     * and that the student is automatically transferred into the archive repository.
     */
    BOOST_AUTO_TEST_CASE(UpdateExamStatus_TEST) {
        manager.registerStudent(mockStudent);
        BOOST_REQUIRE(!mockStudent->getIsReadyForExam());

        // updating status
        manager.updateExamStatus(1);

        // the student should now be archived and have modified status
        BOOST_CHECK(manager.findAllStudents().empty());
        BOOST_REQUIRE(manager.findAllArchiveStudents().size() == 1);

        auto archivedStudent = manager.getArchiveStudentById(1);
        BOOST_REQUIRE(archivedStudent != nullptr);
        BOOST_CHECK(archivedStudent->getIsReadyForExam());

        BOOST_CHECK_THROW(
            manager.updateExamStatus(999),
            NotFoundException
        );

        BOOST_CHECK_THROW(
            manager.updateExamStatus(-5),
            InvalidInputException
        );
    }

    /**
     * @test Verifies the lookup mechanism by unique identifiers for both repositories.
     * Ensures that a student can be fetched via getStudentById from active repo,
     * and via getArchiveStudentById from archive repo depending on their current assignment.
     */
    BOOST_AUTO_TEST_CASE(GetStudentById_TEST) {
        manager.registerStudent(mockStudent);

        // active repo lookup checks (success vs fail)
        auto foundActive = manager.getStudentById(1);
        BOOST_REQUIRE(foundActive != nullptr);
        BOOST_CHECK(foundActive->getId() == 1);

        auto foundArchiveFail = manager.getArchiveStudentById(1);
        BOOST_CHECK(foundArchiveFail == nullptr);

        // move student to archive
        manager.unregisterStudent(1);

        // archive repo lookup checks (success vs fail)
        auto foundArchiveSuccess = manager.getArchiveStudentById(1);
        BOOST_REQUIRE(foundArchiveSuccess != nullptr);
        BOOST_CHECK(foundArchiveSuccess->getId() == 1);

        auto foundActiveFail = manager.getStudentById(1);
        BOOST_CHECK(foundActiveFail == nullptr);
    }

    /**
     * @test Tests the active repository filtering mechanism using custom predicate functions.
     * Ensures that the manager filters and returns students based on targeted parameters.
     */
    BOOST_AUTO_TEST_CASE(FindStudents_WithPredicate_TEST) {
        auto mockStudent2 = std::make_shared<Student>(2, "Anna", "Nowak", "PESEL2", birthDate, fakeAddress, "pkk2");
        manager.registerStudent(mockStudent);
        manager.registerStudent(mockStudent2);

        // predicate searching for specific last name
        StudentPredicate isNowak = [](const StudentPtr& student) {
            return student->getLastName() == "Nowak";
        };

        std::vector<StudentPtr> filtered = manager.findStudents(isNowak);

        BOOST_REQUIRE(filtered.size() == 1);
        BOOST_CHECK(filtered[0]->getId() == 2);
        BOOST_CHECK(filtered[0]->getFirstName() == "Anna");
    }

    /**
     * @test Tests the archive repository filtering mechanism using custom predicate functions.
     * Validates that complex conditional filters correctly evaluate archived items.
     */
    BOOST_AUTO_TEST_CASE(FindArchiveStudents_WithPredicate_TEST) {
        auto mockStudent2 = std::make_shared<Student>(2, "Anna", "Nowak", "PESEL2", birthDate, fakeAddress, "pkk2");

        // adding students directly to the archive repo for test isolation
        archiveRepo.add(mockStudent);
        archiveRepo.add(mockStudent2);

        // predicate searching for specific ID
        StudentPredicate isIdOne = [](const StudentPtr& student) {
            return student->getId() == 1;
        };

        std::vector<StudentPtr> filtered = manager.findArchiveStudents(isIdOne);

        BOOST_REQUIRE(filtered.size() == 1);
        BOOST_CHECK(filtered[0]->getFirstName() == "Jan");
    }

    /**
     * @test Validates the text report generation functionality for both active and archive student repositories.
     * Ensures that requesting a report from an empty repository does not cause a crash,
     * and verifies that a filled repository produces a non-empty string report.
     */
    BOOST_AUTO_TEST_CASE(GetStudentReports_TEST) {
        // empty reports validation
        std::string emptyActiveReport = manager.getStudentReport();
        std::string emptyArchiveReport = manager.getArchiveStudentReport();

        // filling repositories
        manager.registerStudent(mockStudent);
        auto mockStudent2 = std::make_shared<Student>(2, "Anna", "Nowak", "PESEL2", birthDate, fakeAddress, "pkk2");
        archiveRepo.add(mockStudent2);

        // filled reports validation
        std::string filledActiveReport = manager.getStudentReport();
        std::string filledArchiveReport = manager.getArchiveStudentReport();

        BOOST_CHECK(!filledActiveReport.empty());
        BOOST_CHECK(!filledArchiveReport.empty());
    }

BOOST_AUTO_TEST_SUITE_END()