#pragma once
#include <vector>
#include <functional>

/**
 * @file IRepository.h
 * @brief Interface that defines how to manage data in memory.
 */

/**
 * @class IRepository
 * @brief Pure virtual interface for managing data collections.
 * @tparam T The type of items stored in the repository.
 */
template <typename T>
class IRepository {
public:
    /**
     * @brief Type alias for a filter condition function.
     */
    using Predicate = std::function<bool(const T&)>;

    /**
     * @brief Destructor.
     */
    virtual ~IRepository() = default;

    // methods that manage the repo

    /**
     * @brief Gets all items.
     * @return A vector containing all items.
     */
    virtual std::vector<T> findAll() const = 0;

    /**
     * @brief Finds items that match a specific condition.
     * @param predicate The filter function.
     * @return A vector of items that match the predicate.
     */
    virtual std::vector<T> findBy(const Predicate& predicate) const = 0;

    /**
     * @brief Adds an item to the repository.
     * @param item The item to add.
     */
    virtual void add(const T& item) = 0;

    /**
     * @brief Removes an item using its index position.
     * @param id The index of the item to remove.
     */
    virtual void remove(int id) = 0;

    /**
     * @brief Finds a single item by its unique ID.
     * @param id The unique ID number.
     * @return The item if found, or nullptr if not found.
     */
    virtual T getById(int id) const = 0;

    /**
     * @brief Gets the number of items in the repository.
     * @return The total item count.
     */
    [[nodiscard]] virtual int getSize() const = 0;
};