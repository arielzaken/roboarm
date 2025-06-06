#include "../ICommand.h"

class MenuCommand : public ICommand {
public:
    MenuCommand() = default;
    virtual ~MenuCommand() = default;

    /// Execute the command. May block until done.
    void execute() override {
        // Implementation of the menu command execution logic
        // This could involve displaying a menu, handling user input, etc.
        // For now, we will just print a message to indicate execution.
        Serial.println("G0 - A... B... C... D...");
        Serial.println("G1 - A... B... C... D...");
        Serial.println("G28: home all");
        Serial.println("G92: set position to zero");
        Serial.println("M119: switch test");
        Serial.println("H - menu command");
    }
};