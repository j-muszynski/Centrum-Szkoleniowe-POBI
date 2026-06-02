#pragma once
#include <vector>

template <typename T>
class IDataStorage {
public:
    virtual ~IDataStorage() = default;

    // Data storage methods
    virtual void save(const std::vector<T>& data) = 0;
    virtual std::vector<T> load() = 0;
};