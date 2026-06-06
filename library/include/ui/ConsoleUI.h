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

/**
 * @class ConsoleUI
 * @brief Interfejs użytkownika w konsoli.
 * * Klasa odpowiada za komunikację z użytkownikiem, wyświetlanie menu
 * oraz przekazywanie poleceń do warstwy logiki biznesowej (Coordinator).
 */
class ConsoleUI {
private:
    Coordinator& coordinator;

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