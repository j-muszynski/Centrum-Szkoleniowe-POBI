#define BOOST_TEST_MODULE CentrumSzkolenioweTests
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

struct TestConfig {
    TestConfig() {
    }
    ~TestConfig() {
    }
};

BOOST_GLOBAL_FIXTURE(TestConfig);