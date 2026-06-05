/**
 * @file FileStorageTest.cpp
 * @brief Unit tests for the FileStorage class template using the Boost.Test framework.
 */

#include <boost/test/unit_test.hpp>
#include "repositories/FileStorage.h"
#include <vector>
#include <fstream>
#include <string>

/**
 * @struct TestData
 * @brief A simple class to verify Boost serialization mechanism.
 */
struct TestData {
    int id{};              ///< Unique identifier of the test item.
    std::string name;    ///< Name or description of the test item.

    /**
     * @brief Method required by Boost.Serialization to serialize/deserialize the struct members.
     * @tparam Archive Type of the Boost archive (input or output).
     * @param ar Reference to the archive object.
     * @param version Format version number.
     */
    template<class Archive>
    void serialize(Archive & ar, [[maybe_unused]] const unsigned int version) { // dodano [[maybe_unused]]
                                                                                // bo wywalalo bledy przy make
        ar & id;
        ar & name;
    }
};

/**
 * @brief Test suite separating FileStorage components from repository logic.
 */
BOOST_AUTO_TEST_SUITE(FileStorageTestSuite)

/**
 * @test Verifies that FileStorage correctly serializes a vector of objects to a physical file
 * and seamlessly deserializes it back into memory with matching values.
 */
BOOST_AUTO_TEST_CASE(Test_Save_And_Load_Real_File) {
    std::string testFilename = "test_storage_output.txt";

    // deleting file after previous tests
    std::remove(testFilename.c_str());

    // sample data collection to save
    std::vector<TestData> dataToSend;
    dataToSend.push_back({1, "Test1"});
    dataToSend.push_back({2, "Test2"});

    // Instantiate our concrete data storage helper
    FileStorage<TestData> storage(testFilename);

    // saving data to disk
    storage.save(dataToSend);

    // Using BOOST_REQUIRE to check whether file is good to go
    std::ifstream checkFile(testFilename);
    BOOST_REQUIRE(checkFile.good() == true);
    checkFile.close();

    // Load saved data from file into a new container
    std::vector<TestData> loadedData = storage.load();

    // checking if loaded data has the same size as data the file was saved to
    BOOST_TEST(loadedData.size() == dataToSend.size());

    // Verify properties of the first element
    BOOST_TEST(loadedData[0].id == 1);
    BOOST_TEST(loadedData[0].name == "Test1");

    // Verify properties of the second element
    BOOST_TEST(loadedData[1].id == 2);
    BOOST_TEST(loadedData[1].name == "Test2");

    // Deleting file after the test is done
    std::remove(testFilename.c_str());
}

BOOST_AUTO_TEST_SUITE_END()