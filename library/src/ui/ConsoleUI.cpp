#include "ui/ConsoleUI.h"
#include <iostream>
#include <limits>
#include <string>

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
    std::cout << " 5. Wyswietl profil kursanta \n";
    std::cout << "--- ZARZADZANIE KURSAMI I JAZDAMI ---\n";
    std::cout << " 6. Wyswietl wszystkie trwajace kursy\n";
    std::cout << " 7. Wyswietl wszystkie zakonczone kursy\n";
    std::cout << " 8. Odwolaj jazde \n";
    std::cout << " 9. Zmien status kursu na zakonczony (po ID)\n";
    std::cout << "--- ZASOBY OSRODKA (POJAZDY I INSTRUKTORZY) ---\n";
    std::cout << "10. Wyswietl baze aktywnych instruktorow\n";
    std::cout << "11. Wprowadz nowy pojazd do floty\n";
    std::cout << "12. Wycofaj pojazd z floty (po ID)\n";
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
                std::cout << "--- Dataa urodzeni ---\n";
                std::cout << "Podaj rok urodzenia (np 2006): "; std::cin >> year;
                std::cout << "Podaj miesiac (1-12): "; std::cin >> month;
                std::cout << "Podaj dzien (1-31): "; std::cin >> day;

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
                double hours = coordinator.getStudentHours(studentId);
                std::cout << "-> Kursant wyjezdzil juz: " << hours << " godzin.\n";
                break;
            }
            case 5: {
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
            }
            case 6: {
                std::cout << "-> Lista wszystkich TRWAJACYCH kursow:\n";
                std::cout << coordinator.getCourseManager().getCourseReport();
                break;
             }
            case 7: {
                std::cout << "-> Lista wszystkich ZAKONCZONYCH kursow:\n";
                std::cout << coordinator.getCourseManager().getArchiveCourseReport();
                break;
            }
            case 8: {
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
            case 9: {
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
            case 10: {
                std::cout << "-> Baza aktywnych instruktorow:\n";
                std::cout << coordinator.getInstructorManager().getInstructorReport();
                break;
            }
        case 11: {
            int vehicleType;
            std::string plateNumber;

            std::cout << "Jaki rodzaj pojazdu chcesz wprowadzic?\n";
            std::cout << " 1 - Samochod osobowy\n";
            std::cout << " 2 - Motocykl\n";
            std::cout << " 3 - Ciezarowka\n";
            std::cout << " 4 - Autobus\n";
            std::cout << "Wybierz (1-4): ";
            std::cin >> vehicleType;

            std::cout << "Podaj numer rejestracyjny: ";
            std::cin >> plateNumber;

            // DO ZMIANY JAK DOSTANE MANAGERÓW
            std::string typeName = "Nieznany pojazd";
            if (vehicleType == 1) typeName = "Samochod osobowy";
            else if (vehicleType == 2) typeName = "Motocykl";
            else if (vehicleType == 3) typeName = "Ciezarowka";
            else if (vehicleType == 4) typeName = "Autobus";

            // DO ZMIANY JAK DOSTANE MANAGERÓW
            std::cout << "-> Dodano nowy " << typeName << " o rejestracji [" << plateNumber << "] do floty (symulacja).\n";
            break;
        }
            case 12: {
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
            case 0: {
                std::cout << "Zamykanie systemu OSK. Do widzenia!\n";
                isRunning = false;
                break;
            }
            default: {
                std::cout << "[BLAD] Nieznana opcja. Wybierz liczbe od 0 do 12.\n";
                break;
            }
        }
    }
