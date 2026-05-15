#include <boost/test/unit_test.hpp>
#include "model/PassengerCar.h"
#include "model/Truck.h"
#include "model/Bus.h"
#include "model/Motorcycle.h"

BOOST_AUTO_TEST_SUITE(VehicleTestSuite)

BOOST_AUTO_TEST_CASE(PassengerCarTest) {
    PassengerCar car(1, "EL12345");
    BOOST_TEST(car.getId() == 1);
    BOOST_TEST(car.getPlateNumber() == "EL12345");
    BOOST_TEST(car.getCategory() == Category::B); // Zawsze musi być B
}

BOOST_AUTO_TEST_CASE(HeavyVehicleCategoryLogicTest) {
    // Ciężarówka bez przyczepy -> C
    Truck truckNoTrailer(2, "EL111", 12.5, false, 5.0);
    BOOST_TEST(truckNoTrailer.getCategory() == Category::C);

    // Ciężarówka z przyczepą -> C+E
    Truck truckWithTrailer(3, "EL222", 18.0, true, 10.0);
    BOOST_TEST(truckWithTrailer.getCategory() == Category::CE);

    // Autobus z przyczepą -> D+E
    Bus busWithTrailer(4, "EL333", 15.0, true, 50);
    BOOST_TEST(busWithTrailer.getCategory() == Category::DE);
}

BOOST_AUTO_TEST_CASE(MotorcycleCategoryLogicTest) {
    // Silnik <= 50 -> AM
    Motorcycle scooter(5, "M1", 49, 3.0);
    BOOST_TEST(scooter.getCategory() == Category::AM);

    // Mocny motocykl -> A
    Motorcycle heavyBike(6, "M2", 600, 50.0);
    BOOST_TEST(heavyBike.getCategory() == Category::A);
}

BOOST_AUTO_TEST_SUITE_END()