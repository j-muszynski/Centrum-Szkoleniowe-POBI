#pragma once
#include <vector>

/**
 * @file IDataStorage.h
 * @brief Interface for saving and loading data.
 */

/**
 * @class IDataStorage
 * @brief Pure virtual interface for data saving & loading.
 * @tparam T The type of items handled by the storage interface.
 * @details This interface defines how to write data to a file
 * and how to read it back into a vector.
 */
template <typename T>
class IDataStorage {
public:
    /**
     * @brief Destructor.
     */
    virtual ~IDataStorage() = default;

    // Data storage methods

    /**
     * @brief Pure virtual function to save a vector of data.
     * @param data The vector containing items to be saved.
     */
    virtual void save(const std::vector<T>& data) = 0;

    /**
     * @brief Pure virtual function to load data.
     * @return A vector filled with data loaded from the storage.
     */
    virtual std::vector<T> load() = 0;
};