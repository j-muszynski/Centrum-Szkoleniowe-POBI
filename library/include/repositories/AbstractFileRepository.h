#pragma once
#include "IRepository.h"
#include "IDataStorage.h"
#include <string>
#include <vector>
#include <algorithm>
#include <utility>

/**
 * @file AbstractFileRepository.h
 * @brief Template base class for repositories using data storage methods.
 */

/**
 * @class AbstractFileRepository
 * @brief Abstract implementation of the IRepository interface that delegates saving & loading data to IDataStorage.
 * @tparam T The type of elements managed by the repository (a shared smart pointer).
 */
template <typename T>
class AbstractFileRepository : public IRepository<T> {
protected:
    std::string filePath; ///< Target file system path for data operations.
    std::vector<T> repo; ///< In-memory storage container holding the active dataset.

private:
    /**
     * @brief Reference to the interface.
     * @details Implements the "uses" relation from the UML specification by delegating save/load actions.
     */
    IDataStorage<T>& storage;

    /** @brief a variable which allows to give entity in repo unique id number */
    int nextID = 0;

public:
    /**
     * @brief Constructs the repository with a specified file destination and data storage interface.
     * @param filePath System path to the text file.
     * @param storage Reference to an implementation of the IDataStorage interface.
     */
    AbstractFileRepository(std::string filePath, IDataStorage<T>& storage)
        : filePath(std::move(filePath)), storage(storage) {}

    /**
     * @brief Virtual destructor
     */
    virtual ~AbstractFileRepository() = default;

    // ---- Implementation of methods from IRepository interface ----

    /**
     * @brief Retrieves all active elements from the repository.
     * @return A vector copy containing all entities currently in memory.
     */
    std::vector<T> findAll() const override {
        return repo;
    }

    /**
     * @brief Filters the repository elements based on a predicate.
     * @param predicate A callable boolean evaluation target function (lambda).
     * @return A vector of elements that successfully passed the predicate filter evaluation.
     */
    std::vector<T> findBy(const std::function<bool(const T&)>& predicate) const override {
        std::vector<T> result;
        for (const auto& item : repo) {
            if (item != nullptr && predicate(item)) {
                result.push_back(item);
            }
        }
        return result;
    }

    /**
     * @brief Appends a valid entity item to the end of the collection.
     * @param item The entity reference to add. Null pointers are ignored.
     */
    void add(const T& item) override {
        if (item != nullptr) {
            repo.push_back(item);
        }
    }

    /**
     * @brief Removes a single item containing a matching unique identifier.
     * @param id The id of object that will be removed.
     */
    void remove(int id) override {
        auto newEnd = std::remove_if(repo.begin(), repo.end(), [id](const T& item) {
            return item != nullptr && item->getId() == id;
        });
        repo.erase(newEnd, repo.end());
    }

    /**
     * @brief Searches for an item containing a matching unique identifier.
     * @param id The unique identifier value to check.
     * @return The matched item if found, otherwise returns nullptr.
     */
    T getById(int id) const override {
        for (const auto& item : repo) {
            if (item != nullptr && item->getId() == id) {
                return item;
            }
        }
        return nullptr;
    }

    /**
     * @brief Computes the total quantity of entities currently loaded into memory.
     * @return Integer count representation of the collection size.
     */
    [[nodiscard]] int getSize() const override {
        return static_cast<int>(repo.size());
    }

    /**
     * @brief helpful function which allows for getting unique id for repo classes.
     * @return Integer that represents unique id number.
     */
    [[nodiscard]] int getNextID() override {
        return ++nextID;
    }

    // ---- Delegation to IDataStorage ----

    /**
     * @brief Saves given repo to text file
     */
    void executeSave() {
        storage.save(repo);
    }

    /**
     * @brief Replaces the current repo dataset with data stored in the file
     */
    void executeLoad() {
        repo = storage.load();
    }
};
