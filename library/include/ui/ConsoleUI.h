#ifndef CONSOLEUI_H
#define CONSOLEUI_H

#include "managers/Coordinator.h"
#include "model/Student.h"
#include "model/Address.h"
#include "managers/StudentManager.h"
#include "managers/CourseManager.h"
#include "managers/InstructorManager.h"
#include "managers/DrivingSessionManager.h"
#include "managers/VehicleManager.h"
#include "model/PassengerCar.h"
#include "model/Motorcycle.h"
#include "model/Truck.h"
#include "model/Bus.h"
#include <string>

/**
 * @class ConsoleUI
 * @brief Interfejs użytkownika w konsoli.
 * * Klasa odpowiada za komunikację z użytkownikiem, wyświetlanie menu
 * oraz przekazywanie poleceń do warstwy logiki biznesowej (Coordinator).
 */
class ConsoleUI {
private:
    Coordinator& coordinator;

    /**
     * @brief Pobiera od użytkownika liczbę całkowitą (z walidacją).
     * @param text Komunikat wyświetlany użytkownikowi.
     * @param errorMsg Komunikat błędu rzucany w przypadku błędnego wejścia.
     * @return Wprowadzona liczba całkowita.
     * @throws InvalidInputException Jeśli wejście nie jest poprawną liczbą.
     */
    int getIntInput(const std::string& text, const std::string& errorMsg);

    /**
     * @brief Pobiera od użytkownika ciąg znaków.
     * @param text Komunikat wyświetlany użytkownikowi.
     * @return Wprowadzony ciąg znaków.
     */
    std::string getStringInput(const std::string& text);
    Category parseCategory(const std::string& catStr);

    /** @brief Obsługuje opcję: Sprawdź całkowity koszt kursu. */
    void handleCalculateCourseCost();
    /** @brief Obsługuje opcję: Oblicz całkowity przychód ośrodka. */
    void handleCalculateRevenue();
    /** @brief Obsługuje opcję: Zarejestruj nowego kursanta. */
    void handleRegisterStudent();
    /** @brief Obsługuje opcję: Sprawdź wyjeżdżone godziny kursanta. */
    void handleGetStudentHours();
    /** @brief Obsługuje opcję: Rozpocznij kurs. */
    void handleRegisterCourse();
    /** @brief Obsługuje opcję: Zaplanuj jazdy. */
    void handleScheduleDrivingSession();
    /** @brief Obsługuje opcję: Wyświetl profil kursanta. */
    void handleFindStudent();
    /** @brief Obsługuje opcję: Wyświetl wszystkie trwające kursy. */
    void handleDisplayActiveCourses();
    /** @brief Obsługuje opcję: Wyświetl wszystkie zakończone kursy. */
    void handleDisplayArchiveCourses();
    /** @brief Obsługuje opcję: Odwołaj jazdę. */
    void handleCancelDrivingSession();
    /** @brief Obsługuje opcję: Zmień status kursu na zakończony. */
    void handleCompleteCourse();
    /** @brief Obsługuje opcję: Wyświetl bazę aktywnych instruktorów. */
    void handleDisplayActiveInstructors();
    /** @brief Obsługuje opcję: Wprowadź nowy pojazd do floty. */
    void handleRegisterVehicle();
    /** @brief Obsługuje opcję: Wycofaj pojazd z floty. */
    void handleUnregisterVehicle();
    /** @brief Obsługuje opcję: Zatrudnij instruktora. */
    void handleRegisterInstructor();
    /** @brief Obsługuje opcję: Zwolnij instruktora. */
    void handleUnregisterInstructor();

public:
    /**
     * @brief Konstruktor interfejsu konsolowego.
     * @param coordinator Referencja do głównego koordynatora systemu.
     */
    ConsoleUI(Coordinator& coordinator);
    ~ConsoleUI() = default;

    /**
     * @brief Uruchamia główną pętlę programu obsługującą wejście użytkownika.
     */
    void start();

    /**
     * @brief Wyświetla opcje menu w konsoli.
     */
    void displayMenu();
};

#endif