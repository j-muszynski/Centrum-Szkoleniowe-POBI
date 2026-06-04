#ifndef CONSOLEUI_H
#define CONSOLEUI_H

#include "managers/Coordinator.h"

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