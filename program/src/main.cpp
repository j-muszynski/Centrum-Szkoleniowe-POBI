#include <iostream>
#include <memory>
#include "repositories/CourseRepository.h"
#include "model/Course.h"
#include "model/Student.h"
#include "model/Instructor.h"
#include "model/Address.h"
#include <boost/date_time/gregorian/gregorian.hpp>

using namespace std;
using namespace boost::gregorian;

int main() {
    cout << "Witaj w Centrum Szkoleniowym!" << endl;

    auto testAddress = std::make_shared<Address>("Lodz", "Piotrkowska", "100");

    auto student = std::make_shared<Student>(
        1, "Jan", "Kowalski", "01234567890",
        date(2000, 1, 1), testAddress, "PKK-12345"
    );

    auto instructor = std::make_shared<Instructor>(
        10, "Marek", "Mistrz", "70010199999",
        date(1970, 5, 20), testAddress
    );

    auto course = std::make_shared<Course>(
        101, Category::B, instructor, student, 2500.0
    );

    // 2. First repo - repo which data will be saved from
    CourseRepository repoOut;
    repoOut.addCourse(course);

    std::cout << "--- Saving data ---" << std::endl;
    if (repoOut.saveData()) {
        std::cout << "Success! Data was saved to data.txt" << std::endl;
    } else {
        std::cerr << "Error while saving!" << std::endl;
        return 1;
    }

    std::cout << "\nBefore saving:\n" << repoOut.reportCourses() << std::endl;

    // 3. Second repo - for loading up the data
    CourseRepository repoIn;

    std::cout << "--- Loading data---" << std::endl;
    if (repoIn.loadData()) {
        std::cout << "Success! Data was loaded up from the file." << std::endl;
    } else {
        std::cerr << "Error while trying to load up the data" << std::endl;
        return 1;
    }

    // 4. Validation
    std::cout << "\nAfter loading:\n" << repoIn.reportCourses() << std::endl;

    if (repoIn.getSize() == repoOut.getSize()) {
        std::cout << "\nTEST PASSED" << std::endl;
    } else {
        std::cout << "\nTEST FAILED!" << std::endl;
    }

    return 0;
}

/*
 Zaimplementowałem model .h i ,cpp z naszego UMLa,
CMakeLists wziąłem z warsztatów. U mnie wszytkie testy przechodzą,
nie wiem jak u Ciebie na MACu. Obiecuje postarać się zaimplementować
jeszcze dzisiaj testy do wszystkich klas. Póki w mainie nic nie ma
proponuje pisać tu notki, jeśli uznasz, że w pushu będą za długie.
Pisałem komentarze tam gdzie uznałem za stosowne/tam gdzie mi się
chciało, ale tak patrzę na to teraz i w sumie bez sensu, ale może
lepiej Ci się będzie czytać czy coś

Dodaj dokumetnacje, poporawiaj wszystkie ostrzezenie (jest ich mega duzo)
 */