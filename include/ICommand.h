#pragma once
#include <Arduino.h>

/// Command interface (Command Pattern). Each G-code or test is modeled as a ICommand.
class ICommand {
public:
    virtual ~ICommand() = default;

    /// Execute the command. May block until done.
    virtual void execute() = 0;
};