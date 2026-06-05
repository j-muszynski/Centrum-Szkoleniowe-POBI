#ifndef CATEGORY_H
#define CATEGORY_H

#include <iostream>

enum class Category {
    AM,
    A1,
    A2,
    A,
    B,
    BE,
    C,
    CE,
    D,
    DE
};
 // przeciazenie operatora << dla Category. Nie przechodzil test BOOST_TEST(instructor.getQualifications()[0] == Category::B);, teraz przechodzi
inline std::ostream& operator<<(std::ostream& os, const Category& c) {
    os << static_cast<int>(c);
    return os;
}



#endif