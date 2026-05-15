#include <boost/test/unit_test.hpp>
#include "model/Address.h"

BOOST_AUTO_TEST_SUITE(AddressTestSuite)

BOOST_AUTO_TEST_CASE(ConstructorAndGettersTest) {
    Address address("Lodz", "Piotrkowska", "15");

    BOOST_TEST(address.getCity() == "Lodz");
    BOOST_TEST(address.getStreet() == "Piotrkowska");
    BOOST_TEST(address.getNumber() == "15");
    BOOST_TEST(address.getAddressInfo() == "Lodz, Piotrkowska 15");
}

BOOST_AUTO_TEST_CASE(SettersTest) {
    Address address("Warszawa", "Wiejska", "20");
    address.setCity("Krakow");
    
    BOOST_TEST(address.getCity() == "Krakow");
}

BOOST_AUTO_TEST_SUITE_END()