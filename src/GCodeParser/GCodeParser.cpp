#include "GCodeParser.h"
#include "Command/MoveCommand/MoveCommand.h"
#include "Command/SetPositionCommand/SetPositionCommand.h"
#include "Command/SwitchTestCommand/SwitchTestCommand.h"
#include "Command/MenuCommand/MenuCommand.h"
#include "Command/HomeAllCommand/HomeAllCommand.h"
#include "ESP_LOG.h"

GCodeParser::GCodeParser(const std::array<IJoint*, NUM_JOINTS>& joints,
                         const std::array<LimitSwitchBase*, NUM_JOINTS>& switches)
    : _joints(joints), _switches(switches)
{}

ICommand* GCodeParser::parseLine(const String& raw) {
    String line = raw;
    line.trim();
    if (line.length() == 0) {
        return nullptr;
    }

    line.toUpperCase();
    String cmd = line.substring(0, 2);

    // G0 or G1: MoveCommand
    if (cmd == "G0") {
        std::array<long, NUM_JOINTS> targets = {0,0,0,0};
        std::array<bool, NUM_JOINTS> valid = {false, false, false, false};
        unsigned long feed = 0;

        for (int i = 0; i < line.length(); i++) {
            char c = line[i];
            if (c >= 'A' && c <= 'D') {
                int idx = c - 'A';
                int j = i + 1;
                while (j < line.length() && (isDigit(line[j]) || line[j] == '-' || line[j] == '.')) {
                    j++;
                }
                targets[idx] = line.substring(i + 1, j).toInt();
                valid[idx] = true;
                i = j - 1;
            } else if (c == 'F') {
                int j = i + 1;
                while (j < line.length() && (isDigit(line[j]) || line[j] == '.' || line[j] == '-')) {
                    j++;
                }
                feed = (unsigned long) line.substring(i + 1, j).toFloat();
                i = j - 1;
            }
        }

        return new MoveCommand(_joints, targets, valid, feed);
    }

    // M119: switch test
    else if (cmd == "M1" && line.startsWith("M119")) {
        return new SwitchTestCommand(_switches);
    }

    // G28: home all
    else if (cmd == "G2" && line.startsWith("G28")) {
        return new HomeAllCommand(_joints, _switches);
    }

    // G92: set position to zero
    else if (cmd == "G9" && line.startsWith("G92")) {
        return new SetPositionCommand(_joints);
    }

    else if (cmd == "H") {
        // M2: Menu command
        return new MenuCommand();
    }
    ESP_LOGW("GCODE", "Unsupported command: %s", line.c_str());
    return nullptr;
}