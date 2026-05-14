#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include "model/Category.h"

class Vehicle {
private:
    int id;
    std::string plateNumber;  // Rejestracja
    Category category;        // Jakie prawko

public:
    // Konstruktor
    Vehicle(int id, const std::string& plateNumber, Category category);

    // Destruktor
    virtual ~Vehicle() = default;

    // Gettery
    int getId() const;
    const std::string& getPlateNumber() const;
    Category getCategory() const;

    // Settery
    void setPlateNumber(const std::string& plateNumber);
    void setCategory(Category category);

    virtual std::string getVehicleInfo() const = 0;
};

#endif