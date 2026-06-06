#ifndef CATEGORY_H
#define CATEGORY_H

#include <iostream>

/**
 * @enum Category
 * @brief Wylistowanie kategorii prawa jazdy dostępnych w systemie ośrodka.
 */
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
/**
 * @brief Przeciążenie operatora strumieniowego dla enuma Category.
 * @details Pomaga przy testach jednostkowych BOOST_TEST.
 */
 // Nie przechodzil test BOOST_TEST(instructor.getQualifications()[0] == Category::B);, teraz przechodzi
inline std::ostream& operator<<(std::ostream& os, const Category& c) {
    os << static_cast<int>(c);
    return os;
}



#endif