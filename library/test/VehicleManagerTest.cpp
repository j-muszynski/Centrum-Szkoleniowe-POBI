#include <boost/test/unit_test.hpp>

#include "managers/VehicleManager.h"
#include "repositories/VehicleRepository.h"
#include "model/PassengerCar.h"
#include "model/Motorcycle.h"
#include "model/Category.h"

// fakeStorage class imitates FileStorage, but it stores data in a vector instead of a file
template <typename T>
class FakeStorage : public IDataStorage<T> {
public:
    void save([[maybe_unused]] const std::vector<T>& data) override {}
    std::vector<T> load() override { return std::vector<T>(); }
};

struct VehicleManagerFixture {
    FakeStorage<VehiclePtr> storage;
    VehicleRepository vehicleRepo;
    VehicleManager manager;

    // fake data representing a concrete vehicle type
    std::shared_ptr<PassengerCar> mockCar;

    VehicleManagerFixture()
        : vehicleRepo("fakeVehicles.txt", storage),
          manager(vehicleRepo) {
        // Initialization of fake data
        mockCar = std::make_shared<PassengerCar>(1, "EL-12345");
    }
};

/**
 * @test Checks different possibilities and verifies the correctness of the VehicleManager class.
 */
BOOST_FIXTURE_TEST_SUITE(VehicleManagerTestSuite, VehicleManagerFixture)

    /**
     * @test Verifies that a valid vehicle can be successfully registered into the repository.
     * Also validates passing a nullptr does not affect the repository.
     */
    BOOST_AUTO_TEST_CASE(RegisterVehicle_TEST) {
        BOOST_REQUIRE(manager.findAllVehicles().empty());

        // trying to register a nullptr vehicle (should be ignored gracefully)
        manager.registerVehicle(nullptr);
        BOOST_CHECK(manager.findAllVehicles().empty());

        // registering a correct vehicle object
        manager.registerVehicle(mockCar);
        BOOST_REQUIRE(manager.findAllVehicles().size() == 1);

        auto registeredVehicle = manager.findAllVehicles()[0];
        BOOST_REQUIRE(registeredVehicle != nullptr);
        BOOST_CHECK(registeredVehicle->getId() == 1);
        BOOST_CHECK(registeredVehicle->getPlateNumber() == "EL-12345");
    }

    /**
     * @test Verifies the lookup mechanism by unique identifiers.
     * Ensures that a vehicle can be successfully fetched via getVehicleById from the repository,
     * and that searching for a non-existent ID correctly returns a nullptr.
     */
    BOOST_AUTO_TEST_CASE(GetVehicleById_TEST) {
        std::shared_ptr<PassengerCar> mockCar2 = std::make_shared<PassengerCar>(2, "EL-54321");
        manager.registerVehicle(mockCar);
        manager.registerVehicle(mockCar2);

        // looking for the vehicle by correct ID (success)
        auto foundVehicle = manager.getVehicleById(1);
        BOOST_REQUIRE(foundVehicle != nullptr);
        BOOST_CHECK(foundVehicle->getId() == 1);

        // looking for the vehicle by correct ID (success)
        auto foundVehicle2 = manager.getVehicleById(2);
        BOOST_REQUIRE(foundVehicle2 != nullptr);
        BOOST_CHECK(foundVehicle2->getId() == 2);

        // looking for a non-existent vehicle ID (fail)
        auto nonExistentVehicle = manager.getVehicleById(999);
        BOOST_CHECK(nonExistentVehicle == nullptr);
    }

    /**
     * @test Tests the repository filtering mechanism using custom predicate functions.
     * Ensures that the manager correctly filters and returns vehicles based on targeted parameters.
     */
    BOOST_AUTO_TEST_CASE(FindVehicles_WithPredicate_TEST) {
        // predicate searching for Category AM vehicles
        VehiclePredicate isCategoryAM = [](const VehiclePtr& vehicle) {
            return vehicle->getCategory() == Category::AM;
        };

        manager.registerVehicle(mockCar);

        // Adding a second vehicle with different category
        std::shared_ptr<Motorcycle> mockMototorcycle = std::make_shared<Motorcycle>(3, "EL-67890", 50, 25);
        manager.registerVehicle(mockMototorcycle);

        std::vector<VehiclePtr> filtered = manager.findVehicles(isCategoryAM);

        BOOST_REQUIRE(filtered.size() == 1);
        BOOST_CHECK(filtered[0]->getId() == 3);
    }

    /**
     * @test Tests the repository filtering mechanism using custom predicate functions.
     * Ensures that the manager correctly filters and returns vehicles based on targeted parameters.
     * test focuses on showing that predicate does not return anything if condition was not met
     */
    BOOST_AUTO_TEST_CASE(FindVehicle_WithPredicate_NoFindings_TEST) {
        // predicate searching for Category AM vehicles
        VehiclePredicate isCategoryAM = [](const VehiclePtr& vehicle) {
            return vehicle->getCategory() == Category::AM;
        };

        std::vector<VehiclePtr> filtered = manager.findVehicles(isCategoryAM);

        // empty repo = empty result
        BOOST_REQUIRE(filtered.empty());

        manager.registerVehicle(mockCar);
        filtered = manager.findVehicles(isCategoryAM);

        // no data matching critiria = empty result
        BOOST_REQUIRE(filtered.empty());
    }

    /**
     * @test Validates the text report generation functionality for the vehicle repository.
     * Ensures that requesting a report from an empty repository does not cause a crash,
     * and verifies that a filled repository produces a non-empty, formatted string report.
     */
    BOOST_AUTO_TEST_CASE(GetVehicleReport_TEST) {
        // empty report validation
        std::string emptyReport = manager.getVehicleReport();

        // filling repository to trigger report builder loop
        manager.registerVehicle(mockCar);

        // filled report validation
        std::string filledReport = manager.getVehicleReport();

        BOOST_REQUIRE(!manager.findAllVehicles().empty());
        BOOST_CHECK(!filledReport.empty());
    }

BOOST_AUTO_TEST_SUITE_END()
