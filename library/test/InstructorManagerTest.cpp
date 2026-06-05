#include <boost/test/unit_test.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

#include "managers/InstructorManager.h"
#include "repositories/InstructorRepository.h"
#include "model/Instructor.h"
#include "model/Address.h"
#include "model/Category.h"

// fakeStorage class imitates FileStorage, but it stores data in a vector instead of a file
template <typename T>
class FakeStorage : public IDataStorage<T> {
public:
    void save([[maybe_unused]] const std::vector<T>& data) override {}
    std::vector<T> load() override { return std::vector<T>(); }
};

struct InstructorManagerFixture {
    FakeStorage<InstructorPtr> storage;
    InstructorRepository activeRepo;
    InstructorRepository archiveRepo;
    InstructorManager manager;

    // shared parameters for object initializations
    boost::gregorian::date birthDate;
    std::shared_ptr<Address> fakeAddress;

    // fake data
    std::shared_ptr<Instructor> mockInstructor;

    InstructorManagerFixture()
        : activeRepo("fakeInstructorsActive.txt", storage),
          archiveRepo("fakeInstructorsArchive.txt", storage),
          manager(activeRepo, archiveRepo),
          birthDate(1985, boost::gregorian::May, 20) {
        fakeAddress = std::make_shared<Address>("Lodz", "Piotrkowska", "13/3");

        // Initialization of fake data based on your Instructor constructor
        mockInstructor = std::make_shared<Instructor>(12, "Marcin", "Nowak", "PESEL2", birthDate, fakeAddress);
    }
};

/**
 * @test Checks different possibilities and verifies the correctness of the InstructorManager class.
 */
BOOST_FIXTURE_TEST_SUITE(InstructorManagerTestSuite, InstructorManagerFixture)

    /**
     * @test Verifies that a valid instructor can be successfully registered into the active repository.
     * Also validates passing a nullptr does not affect the repository.
     */
    BOOST_AUTO_TEST_CASE(RegisterInstructor_TEST) {
        BOOST_REQUIRE(manager.findAllInstructors().empty());

        // trying to register a nullptr instructor (should be ignored)
        manager.registerInstructor(nullptr);
        BOOST_CHECK(manager.findAllInstructors().empty());

        // registering a correct instructor object
        manager.registerInstructor(mockInstructor);
        BOOST_REQUIRE(manager.findAllInstructors().size() == 1);

        auto registeredInstructor = manager.findAllInstructors()[0];
        BOOST_REQUIRE(registeredInstructor != nullptr);
        BOOST_CHECK(registeredInstructor->getId() == 12);
        BOOST_CHECK(registeredInstructor->getFirstName() == "Marcin");
        BOOST_CHECK(registeredInstructor->getLastName() == "Nowak");
        BOOST_CHECK(registeredInstructor->getQualifications().empty()); // should start with no qualifications
    }

    /**
     * @test Verifies the unregisterInstructor mechanism.
     * Ensures that calling this method completely removes the instructor from the active repository
     * and correctly transfers their record to the archive repository.
     */
    BOOST_AUTO_TEST_CASE(UnregisterInstructor_TEST) {
        // prepare setup by registering an instructor
        manager.registerInstructor(mockInstructor);
        BOOST_REQUIRE(manager.findAllInstructors().size() == 1);
        BOOST_REQUIRE(manager.findAllArchiveInstructors().empty());

        // unregistering the instructor
        manager.unregisterInstructor(12);

        // checks if the instructor was safely moved between repositories
        BOOST_CHECK(manager.findAllInstructors().empty());
        BOOST_REQUIRE(manager.findAllArchiveInstructors().size() == 1);

        auto archivedInstructor = manager.findAllArchiveInstructors()[0];
        BOOST_REQUIRE(archivedInstructor != nullptr);
        BOOST_CHECK(archivedInstructor->getId() == 12);

        // unregistering non-existent instructor should not crash a program
        manager.unregisterInstructor(12);
        BOOST_CHECK(manager.findAllInstructors().empty());
        BOOST_REQUIRE(manager.findAllArchiveInstructors().size() == 1);
    }

    /**
     * @test Verifies the assignToCategory functionality.
     * Validates that calling this method correctly updates the instructor's qualifications collection
     * with the assigned category.
     */
    BOOST_AUTO_TEST_CASE(AssignToCategory_TEST) {
        manager.registerInstructor(mockInstructor);
        BOOST_REQUIRE(mockInstructor->getQualifications().empty());

        // assigning Category B competence
        manager.assignToCategory(12, Category::B);

        // check if instructor received the qualification
        auto updatedInstructor = manager.getInstructorById(12);
        BOOST_REQUIRE(updatedInstructor != nullptr);
        BOOST_REQUIRE(updatedInstructor->getQualifications().size() == 1);
        BOOST_CHECK(updatedInstructor->getQualifications()[0] == Category::B);

        // assigning another Category A competence
        manager.assignToCategory(12, Category::A);
        BOOST_REQUIRE(updatedInstructor->getQualifications().size() == 2);
        BOOST_CHECK(updatedInstructor->getQualifications()[1] == Category::A);
    }

    /**
     * @test Verifies the lookup mechanism by unique identifiers for both repositories.
     * Ensures that an instructor can be fetched via getInstructorById from active repo,
     * and via getArchiveInstructorById from archive repo depending on their current assignment.
     */
    BOOST_AUTO_TEST_CASE(GetInstructorById_TEST) {
        manager.registerInstructor(mockInstructor);

        // active repo lookup checks (success vs fail)
        auto foundActive = manager.getInstructorById(12);
        BOOST_REQUIRE(foundActive != nullptr);
        BOOST_CHECK(foundActive->getId() == 12);

        auto foundArchiveFail = manager.getArchiveInstructorById(12);
        BOOST_CHECK(foundArchiveFail == nullptr);

        // move instructor to archive
        manager.unregisterInstructor(12);

        // archive repo lookup checks (success vs fail)
        auto foundArchiveSuccess = manager.getArchiveInstructorById(12);
        BOOST_REQUIRE(foundArchiveSuccess != nullptr);
        BOOST_CHECK(foundArchiveSuccess->getId() == 12);

        auto foundActiveFail = manager.getInstructorById(12);
        BOOST_CHECK(foundActiveFail == nullptr);

        // looking for non-existent ID
        BOOST_CHECK(manager.getInstructorById(999) == nullptr);
        BOOST_CHECK(manager.getArchiveInstructorById(999) == nullptr);
    }

    /**
     * @test Tests the active repository filtering mechanism using custom predicate functions.
     * Ensures that the manager filters and returns instructors based on targeted parameters.
     */
    BOOST_AUTO_TEST_CASE(FindInstructors_WithPredicate_TEST) {
        auto mockInstructor2 = std::make_shared<Instructor>(15, "Adam", "Kowalski", "PESEL3", birthDate, fakeAddress);
        manager.registerInstructor(mockInstructor);
        manager.registerInstructor(mockInstructor2);

        // predicate searching for specific last name
        InstructorPredicate isKowalski = [](const InstructorPtr& instructor) {
            return instructor->getLastName() == "Kowalski";
        };

        std::vector<InstructorPtr> filtered = manager.findInstructors(isKowalski);

        BOOST_REQUIRE(filtered.size() == 1);
        BOOST_CHECK(filtered[0]->getId() == 15);
        BOOST_CHECK(filtered[0]->getFirstName() == "Adam");
        BOOST_CHECK(filtered[0]->getLastName() == "Kowalski");
        BOOST_CHECK(filtered[0]->getPesel() == "PESEL3");
        BOOST_CHECK(filtered[0]->getBirthDate() == birthDate);
        BOOST_CHECK(filtered[0]->getAddress() == fakeAddress);

        // predicate with no matches
        InstructorPredicate isId999 = [](const InstructorPtr& instructor) {
            return instructor->getId() == 999;
        };
        std::vector<InstructorPtr> emptyFiltered = manager.findInstructors(isId999);
        BOOST_CHECK(emptyFiltered.empty());
    }

    /**
     * @test Tests the archive repository filtering mechanism using custom predicate functions.
     * Validates that complex conditional filters correctly evaluate archived items.
     * Also tests condition where no findings match the predicate criteria.
     */
    BOOST_AUTO_TEST_CASE(FindArchiveInstructors_WithPredicate_TEST) {
        auto mockInstructor2 = std::make_shared<Instructor>(15, "Adam", "Kowalski", "PESEL3", birthDate, fakeAddress);
        archiveRepo.add(mockInstructor);
        archiveRepo.add(mockInstructor2);

        // predicate searching for specific ID
        InstructorPredicate isIdTwelve = [](const InstructorPtr& instructor) {
            return instructor->getId() == 12;
        };

        std::vector<InstructorPtr> filtered = manager.findArchiveInstructors(isIdTwelve);

        BOOST_REQUIRE(filtered.size() == 1);
        BOOST_CHECK(filtered[0]->getFirstName() == "Marcin");
        BOOST_CHECK(filtered[0]->getLastName() == "Nowak");
        BOOST_CHECK(filtered[0]->getPesel() == "PESEL2");
        BOOST_CHECK(filtered[0]->getBirthDate() == birthDate);
        BOOST_CHECK(filtered[0]->getAddress() == fakeAddress);

        // predicate with no matches
        InstructorPredicate isId999 = [](const InstructorPtr& instructor) {
            return instructor->getId() == 999;
        };
        std::vector<InstructorPtr> emptyFiltered = manager.findArchiveInstructors(isId999);
        BOOST_CHECK(emptyFiltered.empty());
    }

    /**
     * @test Validates the text report generation functionality for both active and archive instructor repositories.
     * Ensures that requesting a report from an empty repository doesn't cause a crash,
     * and verifies that a filled repository produces a non-empty, formatted string report.
     */
    BOOST_AUTO_TEST_CASE(GetInstructorReports_TEST) {
        // empty reports validation
        std::string emptyActiveReport = manager.getInstructorReport();
        std::string emptyArchiveReport = manager.getArchiveInstructorReport();

        // filling repositories
        manager.registerInstructor(mockInstructor);
        auto mockInstructor2 = std::make_shared<Instructor>(15, "Adam", "Kowalski", "PESEL3", birthDate, fakeAddress);
        archiveRepo.add(mockInstructor2);

        // filled reports validation
        std::string filledActiveReport = manager.getInstructorReport();
        std::string filledArchiveReport = manager.getArchiveInstructorReport();

        BOOST_CHECK(!filledActiveReport.empty());
        BOOST_CHECK(!filledArchiveReport.empty());
    }

BOOST_AUTO_TEST_SUITE_END()
