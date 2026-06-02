#pragma once
#include "IDataStorage.h"
#include <string>
#include <vector>
#include <fstream>
#include <utility>

// Boost Serialization headers
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>

// Boost DateTime Serialization headers (fixes date errors)
#include <boost/date_time/gregorian/greg_serialize.hpp>
#include <boost/date_time/posix_time/time_serialize.hpp>

/**
 * @file FileStorage.h
 * @brief Implementation of IDataStorage that works with files using Boost.
 */

/**
 * @class FileStorage
 * @brief Saves and loads vector collections to/from disk using boost serialization.
 * @tparam T The type of items to be serialized.
 */
template <typename T>
class FileStorage : public IDataStorage<T> {
private:
    std::string filename; ///< Name or path of the file on disk.

public:
    /**
     * @brief Creates a FileStorage engine.
     * @param filename Target file name.
     */
    FileStorage(std::string filename) : filename(std::move(filename)) {}

    /**
     * @brief Writes data vector into a text file using Boost Archive.
     * @param data Vector of items to save.
     */
    void save(const std::vector<T>& data) override {
        std::ofstream file(filename);
        if (!file.is_open()) return;

        boost::archive::text_oarchive archive(file);
        archive << data;
    }

    /**
     * @brief Reads data from a text file using Boost Archive.
     * @return Vector of items loaded from the file.
     */
    std::vector<T> load() override {
        std::vector<T> data;
        std::ifstream file(filename);
        if (!file.is_open()) return data;

        boost::archive::text_iarchive archive(file);
        archive >> data;
        return data;
    }
};