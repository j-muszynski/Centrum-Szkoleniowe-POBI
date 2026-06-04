/**
 * @file AbstractFileRepositoryTest.cpp
 * @brief Unit tests for the AbstractFileRepository class template using Boost.Test.
 */

#include <boost/test/unit_test.hpp>
#include "repositories/AbstractFileRepository.h"
#include "repositories/IDataStorage.h"
#include <memory>
#include <vector>
#include <string>

/**
 * @class MockStorage
 * @brief A fake storage implementation that keeps data in RAM instead of a physical file.
 * @details This helps us test the repository logic without making real disk operations.
 * @tparam T The type of items stored in the mock memory.
 */
template <typename T>
class MockStorage : public IDataStorage<T> {
public:
    std::vector<T> mockMemory; ///< In-memory container simulating a file.

    /**
     * @brief Saves the given data vector into a vector instead of a file.
     * @param data Vector of items to save.
     */
    void save(const std::vector<T>& data) override {
        mockMemory = data;
    }

    /**
     * @brief Loads the data vector back from the fake file.
     * @return Vector of items saved previously.
     */
    std::vector<T> load() override {
        return mockMemory;
    }
};

/**
 * @class ExampleEntity
 * @brief A simple example class representing an entity with an ID used for testing.
 * @details This class provides the getId() method required by AbstractFileRepository.
 */
class ExampleEntity {
private:
    int id; ///< Unique identifier of the example entity.
public:
    /**
     * @brief Construct a new Example Entity object.
     * @param id Unique ID for this entity.
     */
    ExampleEntity(int id) : id(id) {}

    /**
     * @brief Gets the unique identifier of the entity.
     * @return int The entity ID.
     */
    int getId() const { return id; }
};

/// Type alias for shared pointer to ExampleEntity.
using ExampleEntityPtr = std::shared_ptr<ExampleEntity>;

/**
 * @brief Test suite isolating AbstractFileRepository logic.
 */
BOOST_AUTO_TEST_SUITE(AbstractFileRepositoryTestSuite)

/**
 * @test Verifies that elements can be added to the repository and found by their unique ID.
 */
BOOST_AUTO_TEST_CASE(Test_Repository_Add_And_GetById) {
    auto mockStorage = std::make_shared<MockStorage<ExampleEntityPtr>>();
    AbstractFileRepository<ExampleEntityPtr> repo("path.txt", *mockStorage);

    // Check that the repository is completely empty at the start.
    // If this fails, we stop immediately because the setup is wrong.
    BOOST_REQUIRE(repo.getSize() == 0);

    auto item1 = std::make_shared<ExampleEntity>(1);
    auto item2 = std::make_shared<ExampleEntity>(2);

    repo.add(item1);
    repo.add(item2);

    // Verify the size after adding items
    BOOST_TEST(repo.getSize() == 2);

    // Test finding an existing element by ID.
    ExampleEntityPtr found = repo.getById(1);
    BOOST_REQUIRE(found != nullptr);
    BOOST_TEST(found->getId() == 1);

    // Test searching for a non-existing ID (should return nullptr)
    BOOST_TEST(repo.getById(999) == nullptr);
}

/**
 * @test Verifies that elements can be removed correctly from the repository using their index.
 */
BOOST_AUTO_TEST_CASE(Test_Repository_Remove_By_Index) {
    auto mockStorage = std::make_shared<MockStorage<ExampleEntityPtr>>();
    AbstractFileRepository<ExampleEntityPtr> repo("path.txt", *mockStorage);

    repo.add(std::make_shared<ExampleEntity>(1));
    repo.add(std::make_shared<ExampleEntity>(2));

    BOOST_REQUIRE(repo.getSize() == 2);

    // Remove the element at index 0 (the item with ID 1)
    repo.remove(0);

    // Verify repository state after removal
    BOOST_TEST(repo.getSize() == 1);
    BOOST_TEST(repo.getById(1) == nullptr);  // Item 10 must be gone
    BOOST_TEST(repo.getById(2) != nullptr);  // Item 20 should still exist
}

/**
 * @test Verifies that the findBy method filters items correctly using a custom lambda function.
 */
BOOST_AUTO_TEST_CASE(Test_Repository_FindBy) {
    auto mockStorage = std::make_shared<MockStorage<ExampleEntityPtr>>();
    AbstractFileRepository<ExampleEntityPtr> repo("path.txt", *mockStorage);

    repo.add(std::make_shared<ExampleEntity>(5));
    repo.add(std::make_shared<ExampleEntity>(15));
    repo.add(std::make_shared<ExampleEntity>(25));

    // Filter elements where ID is greater than 10
    auto results = repo.findBy([](const ExampleEntityPtr& item) {
        return item->getId() > 10;
    });

    // Checking filtered results
    BOOST_REQUIRE(results.size() == 2);
    BOOST_TEST(results[0]->getId() == 15);
    BOOST_TEST(results[1]->getId() == 25);
}

/**
 * @test Verifies integration between the repository operations and the underlying data storage.
 */
BOOST_AUTO_TEST_CASE(Test_Repository_ExecuteSave_And_Load_Integration) {
    auto mockStorage = std::make_shared<MockStorage<ExampleEntityPtr>>();
    AbstractFileRepository<ExampleEntityPtr> repo("path.txt", *mockStorage);

    repo.add(std::make_shared<ExampleEntity>(77));

    // Before saving, the mock storage memory must be empty
    BOOST_REQUIRE(mockStorage->mockMemory.empty());

    // Trigger save - repository should copy its content to mockStorage
    repo.executeSave();
    BOOST_TEST(mockStorage->mockMemory.size() == 1);

    // Clear the repository memory (simulate an empty/new repository state)
    repo.remove(0);
    BOOST_REQUIRE(repo.getSize() == 0);

    // Trigger load - repository must pull data back from mockStorage into RAM
    repo.executeLoad();
    BOOST_TEST(repo.getSize() == 1);
    BOOST_TEST(repo.getById(77)->getId() == 77);

    // Checking if findAll method returns the whole repo
    BOOST_TEST(repo.findAll().size() == 1);
}

BOOST_AUTO_TEST_SUITE_END()