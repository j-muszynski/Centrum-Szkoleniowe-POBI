#include "ui/ConsoleUI.h"
#include "model/Exceptions.h"
#include <iostream>
#include <limits>
#include <string>

static bool isDateValid(int year, int month, int day) {
    if (year < 1900 || year > 2030) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1) return false;

    int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
        daysInMonth[1] = 29;
    }

    return day <= daysInMonth[month - 1];
}

ConsoleUI::ConsoleUI(Coordinator& coordinator) : coordinator(coordinator) {}

int ConsoleUI::getIntInput(const std::string& text, const std::string& errorMsg) {
    int value;
    std::cout << text;
    std::cin >> value;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw InvalidInputException(errorMsg);
    }
    return value;
}

std::string ConsoleUI::getStringInput(const std::string& text) {
    std::string value;
    std::cout << text;
    std::getline(std::cin >> std::ws, value);
    return value;
}

Category ConsoleUI::parseCategory(const std::string& catStr) {
    if (catStr == "AM") return Category::AM;
    else if (catStr == "A1") return Category::A1;
    else if (catStr == "A2") return Category::A2;
    else if (catStr == "A") return Category::A;
    else if (catStr == "B") return Category::B;
    else if (catStr == "BE") return Category::BE;
    else if (catStr == "C") return Category::C;
    else if (catStr == "CE") return Category::CE;
    else if (catStr == "D") return Category::D;
    else if (catStr == "DE") return Category::DE;
    else {
        throw ValidationException("Podana kategoria (" + catStr + ") nie istnieje w systemie!");
    }
}

void ConsoleUI::handleCalculateCourseCost() {
    int courseId = getIntInput("Podaj ID kursu: ", "ID kursu musi byc liczba!");

    if(coordinator.getCourseById(courseId) == nullptr) {
        throw NotFoundException("Brak kursu o podanym ID!");
    }

    double cost = coordinator.calculateTotalCourseCost(courseId);
    std::cout << "-> Calkowity koszt kursu wynosi: " << cost << " PLN\n";
}

void ConsoleUI::handleCalculateRevenue() {
    double revenue = coordinator.calculateRevenue();
    std::cout << "-> Calkowity przychod osrodka wynosi: " << revenue << " PLN\n";
}

void ConsoleUI::handleRegisterStudent() {
    std::string firstName = getStringInput("Podaj imie: ");
    std::string lastName = getStringInput("Podaj nazwisko: ");

    std::cout << "Podaj PESEL: ";
    std::string pesel;
    std::cin >> pesel;
    if (pesel.length() != 11) {
        throw ValidationException("Podany PESEL jest bledny! Musi miec dokladnie 11 znakow.");
    }

    std::cout << "Podaj nr PKK: ";
    std::string pkk;
    std::cin >> pkk;

    std::cout << "--- Dane adresowe ---\n";
    std::string city = getStringInput("Podaj miasto: ");
    std::string street = getStringInput("Podaj ulice: ");

    std::cout << "Podaj numer budynku/mieszkania: ";
    std::string buildingNumber;
    std::cin >> buildingNumber;

    std::cout << "--- Data urodzenia ---\n";
    int year = getIntInput("Podaj rok urodzenia (np 2006): ", "Nieprawidlowy format roku!");
    if (year < 1900 || year > 2026) {
        throw InvalidInputException("Nieprawidlowy rok urodzenia!");
    }

    int month = getIntInput("Podaj miesiac (1-12): ", "Miesiac musi byc liczba!");
    if (month < 1 || month > 12) {
        throw InvalidInputException("Miesiac musi byc liczba od 1 do 12!");
    }

    int day = getIntInput("Podaj dzien (1-31, bez zera na poczatku): ", "Dzien musi byc liczba!");
    if (!isDateValid(year, month, day)) {
        throw InvalidInputException("Podana data jest nieprawidlowa w kalendarzu!");
    }

    int nextId = 1;
    auto allStudents = coordinator.getStudentManager().findAllStudents();
    if (!allStudents.empty()) {
        for (const auto& student : allStudents) {
            if (student != nullptr && student->getId() >= nextId) {
                nextId = student->getId() + 1;
            }
        }
    }

    AddressPtr address = std::make_shared<Address>(city, street, buildingNumber);
    boost::gregorian::date birthDate(year, month, day);
    StudentPtr newStudent = std::make_shared<Student>(nextId, firstName, lastName, pesel, birthDate, address, pkk);

    coordinator.getStudentManager().registerStudent(newStudent);
    std::cout << "-> Kursant " << firstName << " " << lastName << " (ID: " << nextId << ") pomyslnie zarejestrowany w systemie.\n";
}

void ConsoleUI::handleGetStudentHours() {
    int studentId = getIntInput("Podaj ID kursanta: ", "ID kursanta musi byc liczba!");

    if(coordinator.getStudentById(studentId) == nullptr) {
        throw NotFoundException("Brak kursanta o podanym ID!");
    }

    double hours = coordinator.getStudentHours(studentId);
    std::cout << "-> Kursant wyjezdzil juz: " << hours << " godzin.\n";
}

void ConsoleUI::handleRegisterCourse() {
    int studentId = getIntInput("Podaj ID kursanta: ", "ID kursanta musi byc liczba!");
    StudentPtr student = coordinator.getStudentById(studentId);
    if (student == nullptr) {
        throw NotFoundException("Nie znaleziono kursanta o podanym ID!");
    }

    int instructorId = getIntInput("Podaj ID instruktora: ", "ID instruktora musi byc liczba!");
    InstructorPtr instructor = coordinator.getInstructorById(instructorId);
    if (instructor == nullptr) {
        throw NotFoundException("Nie znaleziono instruktora o podanym ID!");
    }

    std::cout << "Podaj kategorie (AM, A1, A2, A, B, BE, C, CE, D, DE): ";
    std::string catStr;
    std::cin >> catStr;
    Category cat = parseCategory(catStr);

    coordinator.getCourseManager().registerCourse(student, instructor, cat);
    std::cout << "-> Kurs na kategorie " << catStr << " pomyslnie utworzony!\n";
}

void ConsoleUI::handleScheduleDrivingSession() {
    int studentId = getIntInput("Podaj ID kursanta: ", "ID studenta musi byc liczba!");
    StudentPtr studentCheck = coordinator.getStudentById(studentId);
    if (studentCheck == nullptr) {
        throw NotFoundException("Nie znaleziono kursanta w bazie! Przerwanie planowania jazdy.");
    }

    int courseId = coordinator.getCourseIDByStudentID(studentId);
    if (courseId == -1) {
        throw NotFoundException("Kursant nie posiada aktualnie przypisanego kursu!");
    }

    int instructorId = getIntInput("Podaj ID instruktora: ", "ID instruktora musi byc liczba!");
    InstructorPtr instructorCheck = coordinator.getInstructorById(instructorId);
    if (instructorCheck == nullptr) {
        throw NotFoundException("Instruktor o podanym ID nie istnieje w bazie!");
    }

    int vehicleId = getIntInput("Podaj ID pojazdu: ", "ID pojazdu musi byc liczba!");
    VehiclePtr vehicleCheck = coordinator.getVehicleById(vehicleId);
    if (vehicleCheck == nullptr) {
        throw NotFoundException("Pojazd o podanym ID nie istnieje we flocie!");
    }

    std::cout << "--- Data jazdy ---\n";
    int year = getIntInput("Podaj rok (np. 2026): ", "Rok musi byc liczba!");
    if (year < 2026 || year > 2030) {
        throw InvalidInputException("Nieprawidlowy rok jazdy!");
    }

    int month = getIntInput("Podaj miesiac (1-12, bez zera na poczatku): ", "Miesiac musi byc liczba!");
    if (month < 1 || month > 12) {
        throw InvalidInputException("Miesiac jazdy musi byc z przedzialu 1-12!");
    }

    int day = getIntInput("Podaj dzien (1-31, bez zera na poczatku): ", "Dzien musi byc liczba!");
    if (!isDateValid(year, month, day)) {
        throw InvalidInputException("Podana data sesji jest nieprawidlowa!");
    }

    std::cout << "--- Godziny jazdy ---\n";
    int startHour = getIntInput("Podaj godzine rozpoczecia (0-23): ", "Godzina musi byc liczba!");
    if (startHour < 0 || startHour > 23) {
        throw InvalidInputException("Nieprawidlowy zakres godziny rozpoczecia!");
    }

    int endHour = getIntInput("Podaj godzine zakonczenia (0-23): ", "Godzina musi byc liczba!");
    if (endHour <= startHour || endHour > 23) {
        throw InvalidInputException("Godzina zakonczenia musi byc pozniejsza niz rozpoczecia!");
    }

    boost::gregorian::date sessionDate(year, month, day);
    boost::posix_time::ptime start(sessionDate, boost::posix_time::hours(startHour));
    boost::posix_time::ptime end(sessionDate, boost::posix_time::hours(endHour));
    TimeRange timeRange(start, end);

    coordinator.getDrivingSessionManager().scheduleDrivingSession(studentId, instructorId, vehicleId, timeRange, coordinator);

    std::cout << "-> Jazda zostala zaplanowana na " << year << "-" << month << "-" << day
              << " od " << startHour << ":00 do " << endHour << ":00!\n";
}

void ConsoleUI::handleFindStudent() {
    std::string lastName = getStringInput("Podaj nazwisko kursanta do wyszukania: ");
    StudentPredicate isName = [&lastName](const StudentPtr& s) {
        return s != nullptr && s->getLastName() == lastName;
    };

    auto students = coordinator.getStudentManager().findStudents(isName);

    if (students.empty()) {
        std::cout << "-> Nie znaleziono w bazie nikogo o nazwisku '" << lastName << "'.\n";
    } else {
        std::cout << "Znaleziono:\n";
        for(const auto& s : students) {
            std::cout << " [ID: " << s->getId() << "] " << s->getInfo() << "\n";
        }
    }
}

void ConsoleUI::handleDisplayActiveCourses() {
    std::cout << "-> Lista wszystkich TRWAJACYCH kursow:\n";
    std::cout << coordinator.getCourseManager().getCourseReport();
}

void ConsoleUI::handleDisplayArchiveCourses() {
    std::cout << "-> Lista wszystkich ZAKONCZONYCH kursow:\n";
    std::cout << coordinator.getCourseManager().getArchiveCourseReport();
}

void ConsoleUI::handleCancelDrivingSession() {
    int sessionId = getIntInput("Podaj ID sesji jazd do odwolania: ", "ID sesji musi byc liczba!");

    auto session = coordinator.getDrivingSessionById(sessionId);
    if(session == nullptr) {
        throw NotFoundException("Sesja o podanym ID nie istnieje!");
    }
    std::cout << "-> Zlecono odwolanie jazdy o ID: " << sessionId << ".\n";
}

void ConsoleUI::handleCompleteCourse() {
    int courseId = getIntInput("Podaj ID kursu do zakonczenia: ", "ID kursu musi byc liczba!");

    auto course = coordinator.getCourseById(courseId);
    if(course == nullptr) {
        throw NotFoundException("Brak kursu o podanym ID!");
    }

    coordinator.completeCourse(courseId);
    std::cout << "-> Zmieniono status kursu o ID: " << courseId << " na zakonczony.\n";
}

void ConsoleUI::handleDisplayActiveInstructors() {
    std::cout << "-> Baza aktywnych instruktorow:\n";
    std::cout << coordinator.getInstructorManager().getInstructorReport();
}

void ConsoleUI::handleRegisterVehicle() {
    std::cout << "Jaki rodzaj pojazdu chcesz wprowadzic?\n";
    std::cout << " 1 - Samochod osobowy\n";
    std::cout << " 2 - Motocykl\n";
    std::cout << " 3 - Ciezarowka\n";
    std::cout << " 4 - Autobus\n";
    int vehicleType = getIntInput("Wybierz (1-4): ", "Nieprawidlowy format wyboru!");

    if (vehicleType < 1 || vehicleType > 4) {
        throw InvalidInputException("Nieprawidlowy wybor typu pojazdu!");
    }

    std::string plateNumber = getStringInput("Podaj numer rejestracyjny: ");

    int nextId = 1;
    auto allVehicles = coordinator.getVehicleManager().findAllVehicles();
    if (!allVehicles.empty()) {
        for (const auto& v : allVehicles) {
            if (v != nullptr && v->getId() >= nextId) {
                nextId = v->getId() + 1;
            }
        }
    }

    VehiclePtr newVehicle = nullptr;
    std::string typeName = "Nieznany pojazd";

    if (vehicleType == 1) {
        newVehicle = std::make_shared<PassengerCar>(nextId, plateNumber);
        typeName = "Samochod osobowy";
    }
    else if (vehicleType == 2) {
        int engineCapacity = getIntInput("Podaj pojemnosc silnika (cm3): ", "Pojemnosc silnika musi byc liczba!");

        std::cout << "Podaj moc (kW): ";
        double power;
        std::cin >> power;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw InvalidInputException("Moc silnika musi byc liczba!");
        }

        newVehicle = std::make_shared<Motorcycle>(nextId, plateNumber, engineCapacity, power);
        typeName = "Motocykl";
    }
    else if (vehicleType == 3) {
        std::cout << "Podaj ladownosc (kg): ";
        double cargoCapacity;
        std::cin >> cargoCapacity;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw InvalidInputException("Ladownosc musi byc liczba!");
        }

        std::cout << "Czy posiada przyczepe? (T/N): ";
        char trailerChoice;
        std::cin >> trailerChoice;
        bool hasTrailer = (trailerChoice == 'T' || trailerChoice == 't');

        newVehicle = std::make_shared<Truck>(nextId, plateNumber, 12000, hasTrailer, cargoCapacity);
        typeName = "Ciezarowka";
    }
    else if (vehicleType == 4) {
        int seats = getIntInput("Podaj liczbe miejsc: ", "Liczbe miejsc musi byc liczba calkowita!");

        std::cout << "Czy posiada przyczepe? (T/N): ";
        char trailerChoice;
        std::cin >> trailerChoice;
        bool hasTrailer = (trailerChoice == 'T' || trailerChoice == 't');

        newVehicle = std::make_shared<Bus>(nextId, plateNumber, 15000, hasTrailer, seats);
        typeName = "Autobus";
    }

    if (newVehicle != nullptr) {
        coordinator.getVehicleManager().registerVehicle(newVehicle);
        std::cout << "-> Dodano nowy " << typeName << " (ID: " << nextId << ") o rejestracji [" << plateNumber << "] do floty.\n";
    }
}

void ConsoleUI::handleUnregisterVehicle() {
    int vehicleId = getIntInput("Podaj ID pojazdu do wycofania z floty: ", "ID pojazdu musi byc liczba!");

    auto vehicle = coordinator.getVehicleById(vehicleId);
    if(vehicle == nullptr) {
        throw NotFoundException("Pojazd o takim ID nie istnieje!");
    }

    coordinator.getVehicleManager().unregisterVehicle(vehicleId);
    std::cout << "-> Wycofano pojazd o ID: " << vehicleId << " z aktywnej floty osrodka.\n";
}

void ConsoleUI::handleRegisterInstructor() {
    std::string firstName = getStringInput("Podaj imie: ");
    std::string lastName = getStringInput("Podaj nazwisko: ");

    std::cout << "Podaj PESEL: ";
    std::string pesel;
    std::cin >> pesel;
    if (pesel.length() != 11) {
        throw ValidationException("Podany PESEL instruktora jest bledny! Musi miec 11 znakow.");
    }

    std::cout << "--- Dane adresowe ---\n";
    std::string city = getStringInput("Podaj miasto: ");
    std::string street = getStringInput("Podaj ulice: ");

    std::cout << "Podaj numer budynku/mieszkania: ";
    std::string buildingNumber;
    std::cin >> buildingNumber;

    std::cout << "--- Data urodzenia ---\n";
    int year = getIntInput("Podaj rok urodzenia (np. 1980): ", "Nieprawidlowy format roku!");
    if (year < 1900 || year > 2005) {
        throw InvalidInputException("Nieprawidlowy rok! Instruktor musi byc pelnoletni.");
    }

    int month = getIntInput("Podaj miesiac (1-12, bez zera na poczatku): ", "Miesiac musi byc liczba!");
    if (month < 1 || month > 12) {
        throw InvalidInputException("Nieprawidlowy miesiac urodzenia instruktora!");
    }

    int day = getIntInput("Podaj dzien (1-31, bez zera na poczatku): ", "Dzien musi byc liczba!");
    if (!isDateValid(year, month, day)) {
        throw InvalidInputException("Podana data urodzenia instruktora jest bledna!");
    }

    int nextId = 1;
    auto allInstructors = coordinator.getInstructorManager().findAllInstructors();
    if (!allInstructors.empty()) {
        for (const auto& instructor : allInstructors) {
            if (instructor != nullptr && instructor->getId() >= nextId) {
                nextId = instructor->getId() + 1;
            }
        }
    }

    AddressPtr address = std::make_shared<Address>(city, street, buildingNumber);
    boost::gregorian::date birthDate(year, month, day);
    auto newInstructor = std::make_shared<Instructor>(nextId, firstName, lastName, pesel, birthDate, address);

    std::cout << "--- Kategorie uprawnien ---\n";
    bool addingCategories = true;
    while (addingCategories) {
        std::string catStr = getStringInput("Podaj kategorie (AM, A1, A2, A, B, BE, C, CE, D, DE) lub wpisz 'X' aby zakonczyc: ");

        if (catStr == "X" || catStr == "x") {
            if (newInstructor->getQualifications().empty()) {
                std::cout << "[UWAGA] Instruktor nie ma przypisanej zadnej kategorii. Nie bedzie mogl prowadzic kursow!\n";
            }
            addingCategories = false;
            continue;
        }

        try {
            Category cat = parseCategory(catStr);
            newInstructor->addQualification(cat);
            std::cout << "-> Dodano uprawnienie kategorii: " << catStr << "\n";
        } catch (const ValidationException&) {
            std::cout << "[BLAD] Nieznana kategoria!\n";
        }
    }

    coordinator.getInstructorManager().registerInstructor(newInstructor);
    std::cout << "-> Instruktor " << firstName << " " << lastName << " (ID: " << nextId << ") zostal zatrudniony w osrodku.\n";
}

void ConsoleUI::handleUnregisterInstructor() {
    int instructorId = getIntInput("Podaj ID instruktora do zwolnienia: ", "ID instruktora musi byc liczba!");

    auto instructor = coordinator.getInstructorById(instructorId);
    if(instructor == nullptr) {
        throw NotFoundException("Instruktor o podanym ID nie istnieje!");
    }

    coordinator.getInstructorManager().unregisterInstructor(instructorId);
    std::cout << "-> Pracownik o ID: " << instructorId << " zostal zwolniony i usuniety z bazy.\n";
}

void ConsoleUI::start() {
    bool isRunning = true;
    int choice;

    while (isRunning) {
        try {
            displayMenu();
            std::cin >> choice;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw InvalidInputException("Nieprawidlowy format wyboru menu! Wpisz liczbe.");
            }

            switch (choice) {
                case 1:  handleCalculateCourseCost();   break;
                case 2:  handleCalculateRevenue();      break;
                case 3:  handleRegisterStudent();       break;
                case 4:  handleGetStudentHours();       break;
                case 5:  handleRegisterCourse();        break;
                case 6:  handleScheduleDrivingSession(); break;
                case 7:  handleFindStudent();           break;
                case 8:  handleDisplayActiveCourses();  break;
                case 9:  handleDisplayArchiveCourses(); break;
                case 10: handleCancelDrivingSession();  break;
                case 11: handleCompleteCourse();        break;
                case 12: handleDisplayActiveInstructors(); break;
                case 13: handleRegisterVehicle();       break;
                case 14: handleUnregisterVehicle();     break;
                case 15: handleRegisterInstructor();    break;
                case 16: handleUnregisterInstructor();  break;
                case 0: {
                    std::cout << "Zamykanie systemu OSK. Do widzenia!\n";
                    isRunning = false;
                    break;
                }
                default: {
                    std::cout << "[BLAD] Nieznana opcja. Wybierz liczbe od 0 do 16.\n";
                    break;
                }
            }
        }
        catch (const OSKException& e) {
            std::cout << "\n[BLAD SYSTEMU] Przechwycono wyjatek: " << e.what() << "\n";
        }
    }
}

void ConsoleUI::displayMenu() {
    std::cout << "\n=======================================================\n";
    std::cout << "       SYSTEM OSRODKA SZKOLENIA KIEROWCOW (OSK)        \n";
    std::cout << "=======================================================\n";
    std::cout << "--- FINANSE I STATYSTYKI ---\n";
    std::cout << " 1. Sprawdz calkowity koszt kursu (po ID)\n";
    std::cout << " 2. Oblicz calkowity przychod osrodka\n";
    std::cout << "--- ZARZADZANIE KURSANTAMI ---\n";
    std::cout << " 3. Zarejestruj nowego kursanta\n";
    std::cout << " 4. Sprawdz wyjezdzone godziny kursanta (po ID)\n";
    std::cout << " 5. Rozpocznij kurs \n";
    std::cout << " 6. Zaplanuj jazdy \n";
    std::cout << " 7. Wyswietl profil kursanta \n";
    std::cout << " 8. Wyswietl wszystkie trwajace kursy\n";
    std::cout << " 9. Wyswietl wszystkie zakonczone kursy\n";
    std::cout << " 10. Odwolaj jazde \n";
    std::cout << " 11. Zmien status kursu na zakonczony (po ID)\n";
    std::cout << "--- ZASOBY OSRODKA (POJAZDY I INSTRUKTORZY) ---\n";
    std::cout << "12. Wyswietl baze aktywnych instruktorow\n";
    std::cout << "13. Wprowadz nowy pojazd do floty\n";
    std::cout << "14. Wycofaj pojazd z floty (po ID)\n";
    std::cout << "15. Zatrudnij Instrukora\n";
    std::cout << "16. Zwolnij Instruktora\n";
    std::cout << "=======================================================\n";
    std::cout << " 0. Wyjdz z programu\n";
    std::cout << "Wybierz opcje: ";
}