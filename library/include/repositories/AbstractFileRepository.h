#pragma once
#include "IRepository.h"
#include "IDataStorage.h"
#include <string>
#include <vector>
#include <algorithm>
#include <utility>

template <typename T>
class AbstractFileRepository : public IRepository<T> {
private:
    IDataStorage<T>& storage; // Data storage interface

protected:
    std::string filePath;
    std::vector<T> repo;

public:
    AbstractFileRepository(std::string filePath, IDataStorage<T>& storage)
        : filePath(std::move(filePath)), storage(storage) {}

    virtual ~AbstractFileRepository() = default;

    // ---- Implementation of methods from IRepository interface ----

    std::vector<T> findAll() const override {
        return repo;
    }

    std::vector<T> findBy(const std::function<bool(const T&)>& predicate) const override {
        std::vector<T> result;
        for (const auto& item : repo) {
            if (item != nullptr && predicate(item)) {
                result.push_back(item);
            }
        }
        return result;
    }

    void add(const T& item) override {
        if (item != nullptr) {
            repo.push_back(item);
        }
    }

    void remove(int id) override {
        if (id >= 0 && id < getSize()) {
            repo.erase(std::remove(repo.begin(), repo.end(), repo[id]), repo.end());
        }
    }

    T getById(int id) const override {
        for (const auto& item : repo) {
            if (item != nullptr && item->getId() == id) {
                return item;
            }
        }
        return nullptr;
    }

    int getSize() const override {
        return static_cast<int>(repo.size());
    }

    // ---- Delegatino to IDataStorage ----

    void executeSave() {
        storage.save(repo); // Passing vector as arg to be saved
    }

    void executeLoad() {
        repo = storage.load(); // Overriding vector with data stored in file
    }
};
