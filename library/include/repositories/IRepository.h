#pragma once
#include <vector>
#include <functional>

template <typename T>
class IRepository {
public:
    // using alias template for easier use
    using Predicate = std::function<bool(const T&)>;

    virtual ~IRepository() = default;

    // methods that manage the repo
    virtual std::vector<T> findAll() const = 0;
    virtual std::vector<T> findBy(const Predicate& predicate) const = 0;
    virtual void add(const T& item) = 0;
    virtual void remove(int id) = 0;
    virtual T getById(int id) const = 0;
    [[nodiscard]] virtual int getSize() const = 0;
};