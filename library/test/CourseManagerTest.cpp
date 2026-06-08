#include <boost/test/unit_test.hpp>

#include "managers/CourseManager.h"
#include "repositories/CourseRepository.h"
#include "model/Student.h"
#include "model/Instructor.h"
#include "model/Category.h"
#include "model/Exceptions.h"

// fakeStorage class imitates FleStorage, but it stores data in vector instead of a file
template <typename T>
class FakeStorage : public IDataStorage<T> {
public:
    void save([[maybe_unused]] const std::vector<T>& data) override {}
    std::vector<T> load() override { return std::vector<T>(); }
};

struct CourseManagerFixture {
    FakeStorage<CoursePtr> storage;
    CourseRepository activeRepo;
    CourseRepository archiveRepo;
    CourseManager manager;

    // fake data
    boost::gregorian::date birthDate;
    std::shared_ptr<Address> fakeAddress;

    std::shared_ptr<Student> mockStudent;
    std::shared_ptr<Instructor> mockInstructor;

    CourseManagerFixture()
        : activeRepo("fakeActive.txt", storage),
          archiveRepo("fakeArchive.txt", storage),
          manager(activeRepo, archiveRepo),
          birthDate(2002, boost::gregorian::Jan, 15) {
        fakeAddress = std::make_shared<Address>("Lodz", "Piotrkowska", "13/3");

        // Initialization of fake data
        mockStudent = std::make_shared<Student>(1, "Jan", "Kowalski", "PESEL1", birthDate, fakeAddress, "pkk1");
        mockInstructor = std::make_shared<Instructor>(12, "Marcin", "Nowak", "PESEL2", birthDate, fakeAddress);
    }
};

/**
 * @test checks different possibilities and verifies correctness of CourseManager class
 */
BOOST_FIXTURE_TEST_SUITE(CourseManagerTestSuite, CourseManagerFixture)

    /**
     * @test Verifies that registering a course fails if the instructor lacks the required qualifications.
     * Also checks if a successfully registered course correctly stores all injected entity data.
     */
    BOOST_AUTO_TEST_CASE(RegisterCourse_TEST) {
        BOOST_REQUIRE(manager.findAllCourses().empty());

        // trying to register course where instructor does not have licenses (should throw an exception)
        BOOST_CHECK_THROW(
            manager.registerCourse(mockStudent, mockInstructor, Category::B),
            ValidationException
        );
        BOOST_CHECK(manager.findAllCourses().empty());

        // adding required qualifications and registering course
        mockInstructor->addQualification(Category::B);
        manager.registerCourse(mockStudent, mockInstructor, Category::B);

        // the course should appear in repo
        BOOST_CHECK(!manager.findAllCourses().empty());

        auto registeredCourse = manager.findAllCourses()[0];
        BOOST_CHECK(registeredCourse != nullptr);
        BOOST_CHECK(registeredCourse->getType() == Category::B);
        BOOST_CHECK(registeredCourse->getStudent()->getId() == 1);
        BOOST_CHECK(registeredCourse->getInstructor()->getId() == 12);
    }

    /**
     * @test Verifies the lookup mechanism by unique identifiers.
     * Ensures that a course is successfully retrieved from the active repository by its ID,
     * and that searching for an active course.
     */
    BOOST_AUTO_TEST_CASE(GetCourseById_TEST) {
        // registering courses in active course repo
        mockInstructor->addQualification(Category::A);
        manager.registerCourse(mockStudent, mockInstructor, Category::A);
        int expectedId = activeRepo.findAll()[0]->getId();

        // looking for the course in active repo (success)
        auto foundActive = manager.getCourseById(expectedId);
        BOOST_REQUIRE(foundActive != nullptr);
        BOOST_CHECK(foundActive->getId() == expectedId);

        // looking for the course in active repo (fail)
        auto foundArchive = manager.getArchiveCourseById(expectedId);
        BOOST_CHECK(foundArchive == nullptr);
    }

    /**
     * @test Tests the active repository filtering mechanism using custom predicate functions.
     * Ensures that the manager returns only the courses that precisely match the specified criteria.
     */
    BOOST_AUTO_TEST_CASE(FindCourses_TEST) {
        // adding secnod course
        auto mockStudent2 = std::make_shared<Student>(2, "Anna", "Nowak", "PESEL2", birthDate, fakeAddress, "PKK2");

        mockInstructor->addQualification(Category::A);
        mockInstructor->addQualification(Category::B);

        manager.registerCourse(mockStudent, mockInstructor, Category::B); // ID 1
        manager.registerCourse(mockStudent2, mockInstructor, Category::A); // ID 2

        // predicate that look for Category A
        CoursePredicate isCategoryA = [](const CoursePtr& course) {
            return course->getType() == Category::A;
        };

        std::vector<CoursePtr> filtered_A = manager.findCourses(isCategoryA);

        BOOST_REQUIRE(filtered_A.size() == 1);
        BOOST_CHECK(filtered_A[0]->getType() == Category::A);
        BOOST_CHECK(filtered_A[0]->getStudent()->getId() == 2);

        // course with Category C does not exist, should not return anything
        CoursePredicate isCategoryC = [](const CoursePtr& course) {
            return course->getType() == Category::C;
        };

        std::vector<CoursePtr> filtered_C = manager.findCourses(isCategoryC);

        BOOST_CHECK(filtered_C.empty());
    }

    /**
     * @test Validates the text report generation functionality.
     * Ensures that requesting a report from an empty repository doesn't cause a crash,
     * and verifies that a filled repository produces a non-empty string report.
     */
    BOOST_AUTO_TEST_CASE(GetCourseReport_TEST) {
        // empty report should not mess up the program
        std::string emptyReport = manager.getCourseReport();

        // adding course to get report later
        mockInstructor->addQualification(Category::C);
        manager.registerCourse(mockStudent, mockInstructor, Category::C);

        std::string filledReport = manager.getCourseReport();

        BOOST_REQUIRE(!manager.findAllCourses().empty());
        BOOST_CHECK(!filledReport.empty());
    }

    /**
     * @test Validates the of the registration method.
     * Ensures that the system handles and rejects null pointer inputs for critical parameters
     * (Student or Instructor) without crashing the application.
     */
    BOOST_AUTO_TEST_CASE(RegisterCourse_NullPointers_TEST) {
        BOOST_REQUIRE(manager.findAllCourses().empty());
        mockInstructor->addQualification(Category::B);

        // Null student
        BOOST_CHECK_THROW(
            manager.registerCourse(nullptr, mockInstructor, Category::B),
            NullPointerException
        );
        BOOST_CHECK(manager.findAllCourses().empty());

        // null instructor
        BOOST_CHECK_THROW(
            manager.registerCourse(mockStudent, nullptr, Category::B),
            NullPointerException
        );
        BOOST_CHECK(manager.findAllCourses().empty());
    }

    /**
     * @test verifies correct exception throw when no course was found
     */
    BOOST_AUTO_TEST_CASE(CompleteCourse_NotFound_Exception_TEST) {
        BOOST_CHECK_THROW(
            manager.completeCourse(999),
            NotFoundException
        );
    }

BOOST_AUTO_TEST_SUITE_END()
