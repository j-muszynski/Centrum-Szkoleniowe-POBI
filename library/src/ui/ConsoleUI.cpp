#include "ui/ConsoleUI.h"
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

void ConsoleUI::start() {
    bool isRunning = true;
    int choice;

    while (isRunning) {
        displayMenu();
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "[BLAD] Nieprawidlowy format! Wpisz liczbe.\n";
            continue;
        }

        switch (choice) {
            case 1: {
                int courseId;
                std::cout << "Podaj ID kursu: ";
                std::cin >> courseId;

                if (std::cin.fail()) {
                    std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "[BLAD] ID kursu musi byc liczba!\n";
                    break;
                }

                double cost = coordinator.calculateTotalCourseCost(courseId);
                std::cout << "-> Calkowity koszt kursu wynosi: " << cost << " PLN\n";
                break;
            }
            case 2: {
                double revenue = coordinator.calculateRevenue();
                std::cout << "-> Calkowity przychod osrodka wynosi: " << revenue << " PLN\n";
                break;
            }
            case 3: {
                std::string firstName, lastName, pesel, pkk;
                std::string city, street, buildingNumber;
                int year, month, day;

                // Podstawowe dane
                std::cout << "Podaj imie: ";
                std::getline(std::cin >> std::ws, firstName);
                std::cout << "Podaj nazwisko: ";
                std::getline(std::cin >> std::ws, lastName);
                std::cout << "Podaj PESEL: ";
                std::cin >> pesel;
                std::cout << "Podaj nr PKK: ";
                std::cin >> pkk;

                // Dane adresowe
                std::cout << "--- Dane adresowe ---\n";
                std::cout << "Podaj miasto: ";
                std::getline(std::cin >> std::ws, city);
                std::cout << "Podaj ulice: ";
                std::getline(std::cin >> std::ws, street);
                std::cout << "Podaj numer budynku/mieszkania: ";
                std::cin >> buildingNumber;

                // Data urodzenia
                std::cout << "--- Data urodzenia ---\n";
                std::cout << "Podaj rok urodzenia (np 2006): ";
                std::cin >> year;
                if (std::cin.fail() || year < 1900 || year > 2026) {
                    std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "[BLAD] Nieprawidlowy rok!\n"; break;
                }

                std::cout << "Podaj miesiac (1-12): ";
                std::cin >> month;
                if (std::cin.fail() || month < 1 || month > 12) {
                    std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "[BLAD] Miesiac musi byc liczba od 1 do 12!\n"; break;
                }

                std::cout << "Podaj dzien (1-31, bez zera na poczatku): ";
                std::cin >> day;
                if (std::cin.fail() || !isDateValid(year, month, day)) {
                    std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "[BLAD] Podana data jest nieprawidlowa w kalendarzu!\n"; break;
                }

                // Przypisujemy ID
                int nextId = 1; // Domyślne ID dla pierwszej osoby
                auto allStudents = coordinator.getStudentManager().findAllStudents();

                // Czy sa kursanci?
                if (!allStudents.empty()) {
                    for (const auto& student : allStudents) {
                        if (student != nullptr && student->getId() >= nextId) {
                            nextId = student->getId() + 1;
                        }
                    }
                }

                // Tworzymy obiekty
                AddressPtr address = std::make_shared<Address>(city, street, buildingNumber);
                boost::gregorian::date birthDate(year, month, day);

                StudentPtr newStudent = std::make_shared<Student>(nextId, firstName, lastName, pesel, birthDate, address, pkk);

                // Zapisujemy
                coordinator.getStudentManager().registerStudent(newStudent);

                std::cout << "-> Kursant " << firstName << " " << lastName << " (ID: " << nextId << ") pomyslnie zarejestrowany w systemie.\n";
                break;
            }
            case 4: {
                int studentId;
                std::cout << "Podaj ID kursanta: ";
                std::cin >> studentId;

                if (std::cin.fail()) {
                    std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "[BLAD] ID kursanta musi byc liczba!\n";
                    break;
                }

                double hours = coordinator.getStudentHours(studentId);
                std::cout << "-> Kursant wyjezdzil juz: " << hours << " godzin.\n";
                break;
            }
            case 5: {
                int studentId, instructorId;
                std::string catStr;

                std::cout << "Podaj ID kursanta: ";
                std::cin >> studentId;
                if (std::cin.fail()) {
                    std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "[BLAD] ID kursanta musi byc liczba!\n";
                    break;
                }

                std::cout << "Podaj ID instruktora: ";
                std::cin >> instructorId;
                if (std::cin.fail()) {
                    std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "[BLAD] ID instruktora musi byc liczba!\n";
                    break;
                }

                std::cout << "Podaj kategorie (AM, A1, A2, A, B, BE, C, CE, D, DE): ";
                std::cin >> catStr;

                // Mapowanie stringa na Enum
                Category cat;
                if (catStr == "AM") cat = Category::AM;
                else if (catStr == "A1") cat = Category::A1;
                else if (catStr == "A2") cat = Category::A2;
                else if (catStr == "A") cat = Category::A;
                else if (catStr == "B") cat = Category::B;
                else if (catStr == "BE") cat = Category::BE;
                else if (catStr == "C") cat = Category::C;
                else if (catStr == "CE") cat = Category::CE;
                else if (catStr == "D") cat = Category::D;
                else if (catStr == "DE") cat = Category::DE;
                else {
                    std::cout << "[BLAD] Podana kategoria (" << catStr << ") nie istnieje w systemie!\n";
                    break;
                }

                try {
                    StudentPtr student = coordinator.getStudentById(studentId);
                    InstructorPtr instructor = coordinator.getInstructorById(instructorId);

                    if (student != nullptr && instructor != nullptr) {
                        coordinator.getCourseManager().registerCourse(student, instructor, cat);
                        std::cout << "-> Kurs na kategorie " << catStr << " pomyslnie utworzony!\n";
                    } else {
                        std::cout << "[BLAD] Nie znaleziono kursanta lub instruktora o podanym ID.\n";
                    }
                } catch (const std::exception& e) {
                    std::cout << "[BLAD] Nie udalo sie utworzyc kursu: " << e.what() << "\n";
                }
            break;
        }
            case 6: {
                int studentId, instructorId, vehicleId;
                int year, month, day, startHour, endHour;

                std::cout << "Podaj ID kursanta: ";
                std::cin >> studentId;
                if (std::cin.fail()) {
                    std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "[BLAD] ID musi byc liczba!\n"; break;
                }

                std::cout << "Podaj ID instruktora: ";
                std::cin >> instructorId;
                if (std::cin.fail()) {
                    std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "[BLAD] ID musi byc liczba!\n"; break;
                }

                std::cout << "Podaj ID pojazdu: ";
                std::cin >> vehicleId;
                if (std::cin.fail()) {
                    std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "[BLAD] ID musi byc liczba!\n"; break;
                }

                std::cout << "--- Data jazdy ---\n";
                std::cout << "Podaj rok (np. 2026): ";
                std::cin >> year;
                if (std::cin.fail() || year < 2024 || year > 2030) {
                    std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "[BLAD] Nieprawidlowy rok!\n"; break;
                }

                std::cout << "Podaj miesiac (1-12, bez zera na poczatku): ";
                std::cin >> month;
                if (std::cin.fail() || month < 1 || month > 12) {
                    std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "[BLAD] Miesiac musi byc liczba od 1 do 12!\n"; break;
                }

                std::cout << "Podaj dzien (1-31, bez zera na poczatku): ";
                std::cin >> day;
                if (std::cin.fail() || !isDateValid(year, month, day)) {
                    std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "[BLAD] Podana data jest nieprawidlowa w kalendarzu!\n"; break;
                }

                std::cout << "--- Godziny jazdy ---\n";
                std::cout << "Podaj godzine rozpoczecia (0-23): ";
                std::cin >> startHour;
                if (std::cin.fail() || startHour < 0 || startHour > 23) {
                    std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "[BLAD] Nieprawidlowa godzina rozpoczecia!\n"; break;
                }

                std::cout << "Podaj godzine zakonczenia (0-23): ";
                std::cin >> endHour;
                if (std::cin.fail() || endHour <= startHour || endHour > 23) {
                    std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "[BLAD] Godzina zakonczenia musi byc pozniejsza niz rozpoczecia (zakres 0-23)!\n"; break;
                }

                try {
                    boost::gregorian::date sessionDate(year, month, day);
                    boost::posix_time::ptime start(sessionDate, boost::posix_time::hours(startHour));
                    boost::posix_time::ptime end(sessionDate, boost::posix_time::hours(endHour));
                    TimeRange timeRange(start, end);

                    coordinator.getDrivingSessionManager().scheduleDrivingSession(studentId, instructorId, vehicleId, timeRange, coordinator);

                    std::cout << "-> Jazda zostala zaplanowana na " << year << "-" << month << "-" << day
                              << " od " << startHour << ":00 do " << endHour << ":00!\n";
                } catch (const std::exception& e) {
                    std::cout << "[BLAD] Nie udalo sie zaplanowac jazdy: " << e.what() << "\n";
                }
                break;
            }
            case 7: {
                std::string lastName;
                std::cout << "Podaj nazwisko kursanta do wyszukania: ";

                std::getline(std::cin >> std::ws, lastName);
                StudentPredicate isName = [&lastName](const StudentPtr& s) { // lambda dla flitrowania po nazwisku
                    return s != nullptr && s->getLastName() == lastName;
                };

                // Manager zwraca liste szukanych studentow
                auto students = coordinator.getStudentManager().findStudents(isName);

                if (students.empty()) {
                    std::cout << "-> Nie znaleziono w bazie nikogo o nazwisku '" << lastName << "'.\n";
                } else {
                    std::cout << "Znaleziono:\n";
                    for(const auto& s : students) {
                        std::cout << " [ID: " << s->getId() << "] " << s->getInfo() << "\n";
                    }
                }
                break;
            }
            case 8: {
                std::cout << "-> Lista wszystkich TRWAJACYCH kursow:\n";
                std::cout << coordinator.getCourseManager().getCourseReport();
                break;
            }
            case 9: {
                std::cout << "-> Lista wszystkich ZAKONCZONYCH kursow:\n";
                std::cout << coordinator.getCourseManager().getArchiveCourseReport();
                break;
            }
            case 10: {
                int sessionId;
                std::cout << "Podaj ID sesji jazd do odwolania: ";
                std::cin >> sessionId;

                // Czy wpisano liczbe?
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "[BLAD] ID sesji musi byc liczba!\n";
                    break; // Wracamy do menu
                }

                // Czy jest w bazie?
                auto session = coordinator.getDrivingSessionById(sessionId);

                if (session == nullptr) {
                    // Nie ma = blad
                    std::cout << "[BLAD] Nie znaleziono sesji jazd o ID: " << sessionId << " w systemie.\n";
                } else {
                    // Jest = usuwamy
                    coordinator.getDrivingSessionManager().cancelSession(sessionId, coordinator);
                    std::cout << "-> Zlecono odwolanie jazdy o ID: " << sessionId << ".\n";
                }
                break;
            }
            case 11: {
                int courseId;
                std::cout << "Podaj ID kursu do zakonczenia: ";
                std::cin >> courseId;

                // Czy liczba
                if (std::cin.fail()) {
                    std::cin.clear(); // Czyścimy flagę błędu
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Wyrzucamy śmieci z bufora
                    std::cout << "[BLAD] ID kursu musi byc liczba!\n";
                    break; // Wychodzimy z tego case'a
                }

                // Czy jest w bazie
                auto course = coordinator.getCourseById(courseId);

                if (course == nullptr) {
                    // Nie ma = blad
                    std::cout << "[BLAD] Nie znaleziono kursu o ID: " << courseId << " w systemie.\n";
                } else {
                    // Jest = usuwamy
                    coordinator.completeCourse(courseId);
                    std::cout << "-> Zmieniono status kursu o ID: " << courseId << " na zakonczony.\n";
                }
                break;
            }
            case 12: {
                std::cout << "-> Baza aktywnych instruktorow:\n";
                std::cout << coordinator.getInstructorManager().getInstructorReport();
                break;
            }
            case 13: {
                int vehicleType;
                std::string plateNumber;

                std::cout << "Jaki rodzaj pojazdu chcesz wprowadzic?\n";
                std::cout << " 1 - Samochod osobowy\n";
                std::cout << " 2 - Motocykl\n";
                std::cout << " 3 - Ciezarowka\n";
                std::cout << " 4 - Autobus\n";
                std::cout << "Wybierz (1-4): ";
                std::cin >> vehicleType;

                // Czy liczba i czy w przedziale 1-4
                if (std::cin.fail() || vehicleType < 1 || vehicleType > 4) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "[BLAD] Nieprawidlowy wybor! Musisz podac liczbe od 1 do 4.\n";
                    break;
                }

                std::cout << "Podaj numer rejestracyjny: ";
                std::getline(std::cin >> std::ws, plateNumber);

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
                    int engineCapacity;
                    double power;

                    std::cout << "Podaj pojemnosc silnika (cm3): ";
                    std::cin >> engineCapacity;
                    if (std::cin.fail()) {
                        std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "[BLAD] Pojemnosc silnika musi byc liczba!\n"; break;
                    }

                    std::cout << "Podaj moc (kW): ";
                    std::cin >> power;
                    if (std::cin.fail()) {
                        std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "[BLAD] Moc musi byc liczba!\n"; break;
                    }

                    newVehicle = std::make_shared<Motorcycle>(nextId, plateNumber, engineCapacity, power);
                    typeName = "Motocykl";
                }
                else if (vehicleType == 3) {
                    double cargoCapacity;
                    char trailerChoice;

                    std::cout << "Podaj ladownosc (kg): ";
                    std::cin >> cargoCapacity;
                    if (std::cin.fail()) {
                        std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "[BLAD] Ladownosc musi byc liczba!\n"; break;
                    }

                    std::cout << "Czy posiada przyczepe? (T/N): ";
                    std::cin >> trailerChoice;
                    bool hasTrailer = (trailerChoice == 'T' || trailerChoice == 't');

                    newVehicle = std::make_shared<Truck>(nextId, plateNumber, 12000, hasTrailer, cargoCapacity);
                    typeName = "Ciezarowka";
                }
                else if (vehicleType == 4) {
                    int seats;
                    char trailerChoice;

                    std::cout << "Podaj liczbe miejsc: ";
                    std::cin >> seats;
                    if (std::cin.fail()) {
                        std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "[BLAD] Liczba miejsc musi byc liczba calkowita!\n"; break;
                    }

                    std::cout << "Czy posiada przyczepe? (T/N): ";
                    std::cin >> trailerChoice;
                    bool hasTrailer = (trailerChoice == 'T' || trailerChoice == 't');

                    newVehicle = std::make_shared<Bus>(nextId, plateNumber, 15000, hasTrailer, seats);
                    typeName = "Autobus";
                }

                // Zapis
                if (newVehicle != nullptr) {
                    coordinator.getVehicleManager().registerVehicle(newVehicle);
                    std::cout << "-> Dodano nowy " << typeName << " (ID: " << nextId << ") o rejestracji [" << plateNumber << "] do floty.\n";
                }
                break;
            }
            case 14: {
                int vehicleId;
                std::cout << "Podaj ID pojazdu do wycofania z floty: ";
                std::cin >> vehicleId;

                // Czy liczba
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "[BLAD] ID pojazdu musi byc liczba!\n";
                    break;
                }

                // Czy pojazd o takim ID istnieje bazie?
                auto vehicle = coordinator.getVehicleById(vehicleId);

                if (vehicle == nullptr) {
                    // Nie = blad
                    std::cout << "[BLAD] Nie znaleziono pojazdu o ID: " << vehicleId << " w systemie floty.\n";
                } else {
                    // Tak = usuwamy
                    coordinator.getVehicleManager().unregisterVehicle(vehicleId);

                    std::cout << "-> Wycofano pojazd o ID: " << vehicleId << " z aktywnej floty osrodka.\n";
                }
                break;
            }
            case 15: {
                std::string firstName, lastName, pesel;
                std::string city, street, buildingNumber;
                int year, month, day;

                // Podstawowe dane
                std::cout << "Podaj imie: ";
                std::getline(std::cin >> std::ws, firstName);
                std::cout << "Podaj nazwisko: ";
                std::getline(std::cin >> std::ws, lastName);
                std::cout << "Podaj PESEL: ";
                std::cin >> pesel;

                // Dane adresowe
                std::cout << "--- Dane adresowe ---\n";
                std::cout << "Podaj miasto: ";
                std::getline(std::cin >> std::ws, city);
                std::cout << "Podaj ulice: ";
                std::getline(std::cin >> std::ws, street);
                std::cout << "Podaj numer budynku/mieszkania: ";
                std::cin >> buildingNumber;

                // Data urodzenia
                std::cout << "--- Data urodzenia ---\n";
                std::cout << "Podaj rok urodzenia (np. 1980): ";
                std::cin >> year;
                if (std::cin.fail() || year < 1900 || year > 2006) {
                    std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "[BLAD] Nieprawidlowy rok (Instruktor musi byc pelnoletni)!\n"; break;
                }

                std::cout << "Podaj miesiac (1-12, bez zera na poczatku): ";
                std::cin >> month;
                if (std::cin.fail() || month < 1 || month > 12) {
                    std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "[BLAD] Miesiac musi byc liczba od 1 do 12!\n"; break;
                }

                std::cout << "Podaj dzien (1-31, bez zera na poczatku): ";
                std::cin >> day;
                if (std::cin.fail() || !isDateValid(year, month, day)) {
                    std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "[BLAD] Podana data jest nieprawidlowa w kalendarzu!\n"; break;
                }

                // Przypisanie ID
                int nextId = 1;
                auto allInstructors = coordinator.getInstructorManager().findAllInstructors();
                if (!allInstructors.empty()) {
                    for (const auto& instructor : allInstructors) {
                        if (instructor != nullptr && instructor->getId() >= nextId) {
                            nextId = instructor->getId() + 1;
                        }
                    }
                }

                // Tworzymy obiekty
                AddressPtr address = std::make_shared<Address>(city, street, buildingNumber);
                boost::gregorian::date birthDate(year, month, day);
                auto newInstructor = std::make_shared<Instructor>(nextId, firstName, lastName, pesel, birthDate, address);

                // Dodawanie kategorii
                std::cout << "--- Kategorie uprawnien ---\n";
                bool addingCategories = true;
                while (addingCategories) {
                    std::string catStr;
                    std::cout << "Podaj kategorie (AM, A1, A2, A, B, BE, C, CE, D, DE) lub wpisz 'X' aby zakonczyc: ";
                    std::cin >> catStr;

                    if (catStr == "X" || catStr == "x") {
                        if (newInstructor->getQualifications().empty()) {
                            std::cout << "[UWAGA] Instruktor nie ma przypisanej zadnej kategorii. Nie bedzie mogl prowadzic kursow!\n";
                        }
                        addingCategories = false;
                        continue;
                    }

                    Category cat;
                    if (catStr == "AM") cat = Category::AM;
                    else if (catStr == "A1") cat = Category::A1;
                    else if (catStr == "A2") cat = Category::A2;
                    else if (catStr == "A") cat = Category::A;
                    else if (catStr == "B") cat = Category::B;
                    else if (catStr == "BE") cat = Category::BE;
                    else if (catStr == "C") cat = Category::C;
                    else if (catStr == "CE") cat = Category::CE;
                    else if (catStr == "D") cat = Category::D;
                    else if (catStr == "DE") cat = Category::DE;
                    else {
                        std::cout << "[BLAD] Nieznana kategoria!\n";
                        continue;
                    }

                    newInstructor->addQualification(cat);
                    std::cout << "-> Dodano uprawnienie kategorii: " << catStr << "\n";
                }

                // Zapis
                coordinator.getInstructorManager().registerInstructor(newInstructor);
                std::cout << "-> Instruktor " << firstName << " " << lastName << " (ID: " << nextId << ") zostal zatrudniony w osrodku.\n";
                break;
            }
            case 16: {
                int instructorId;
                std::cout << "Podaj ID instruktora do zwolnienia: ";
                std::cin >> instructorId;

                // Czy liczba
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "[BLAD] ID instruktora musi byc liczba!\n";
                    break;
                }

                // Sprawdzamy czy istnieje
                auto instructor = coordinator.getInstructorById(instructorId);

                if (instructor == nullptr) {
                    std::cout << "[BLAD] Nie znaleziono instruktora o ID: " << instructorId << " w bazie pracownikow.\n";
                } else {
                    // Usuwamy
                    coordinator.getInstructorManager().unregisterInstructor(instructorId);
                    std::cout << "-> Pracownik o ID: " << instructorId << " zostal zwolniony i usuniety z bazy.\n";
                }
                break;
            }
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
}