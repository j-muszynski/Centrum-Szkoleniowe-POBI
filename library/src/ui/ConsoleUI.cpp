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
                std::string firstName, lastName, pesel;
                std::cout << "Podaj imie: ";
                std::cin >> firstName;
                std::cout << "Podaj nazwisko: ";
                std::cin >> lastName;
                std::cout << "Podaj PESEL: ";
                std::cin >> pesel;

                // DO ZMIANY JAK DOSTANE MANAGERÓW
                std::cout << "-> Kursant " << firstName << " " << lastName << " (PESEL: " << pesel << ") pomyslnie zarejestrowany (symulacja).\n";
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
                std::cin >> lastName;

                // DO ZMIANY JAK DOSTANE MANAGERÓW
                std::cout << "-> Wyszukiwanie kursantow o nazwisku '" << lastName << "' (symulacja)...\n";
                std::cout << "Znaleziono:\n";
                std::cout << " [ID: 101] Jan " << lastName << " | PKK: 100200/30 | PESEL: 1234\n";
                std::cout << " [ID: 102] Piotr " << lastName << " | PKK: 500600/70 | PESEL: 5678\n";
                break;
            }
            }
            case 6: {
                // DO ZMIANY JAK DOSTANE MANAGERÓW
                std::cout << "-> Lista wszystkich TRWAJACYCH kursow (symulacja):\n";
                break;
            }
            case 7: {
                // DO ZMIANY JAK DOSTANE MANAGERÓW
                std::cout << "-> Lista wszystkich ZAKONCZONYCH kursow (symulacja):\n";
                break;
            }
            case 8: {
                int sessionId;
                std::cout << "Podaj ID sesji jazd do odwolania: ";
                std::cin >> sessionId;

                // DO ZMIANY JAK DOSTANE MANAGERÓW
                std::cout << "-> Jazda " << sessionId << " odwolana. Zasoby (Instruktor, Pojazd) zostaly zwolnione (symulacja).\n";
                break;
            }
            case 9: {
                int courseId;
                std::cout << "Podaj ID kursu do zakonczenia: ";
                std::cin >> courseId;
                coordinator.completeCourse(courseId);
                std::cout << "-> Zlecono zakonczenie kursu o ID: " << courseId << "\n";
                break;
            }
            case 10: {
                // DO ZMIANY JAK DOSTANE MANAGERÓW
                std::cout << "-> Baza aktywnych instruktorow (symulacja):\n";
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

                // DO ZMIANY JAK DOSTANE MANAGERÓW
                std::cout << "-> Pojazd " << vehicleId << " wycofany i usuniety ze stanu (symulacja).\n";
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
}