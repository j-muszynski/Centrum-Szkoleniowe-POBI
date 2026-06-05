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


class ConsoleUI {
private:
    Coordinator& coordinator;

public:
    ConsoleUI(Coordinator& coordinator);
    ~ConsoleUI() = default;

    void start();
    void displayMenu();
};

#endif