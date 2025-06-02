#include <Arduino.h>
#include <FastAccelStepper.h>
#include <config.h>
#include <pins.h>
#include "esp_log.h"
static const char* TAG = "GCODE";


const int stepPins[NUM_JOINTS] = PULSE_PINS;
const int dirPins[NUM_JOINTS]  = DIR_PINS;

FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper* steppers[NUM_JOINTS];

void setupSteppers() {
  engine.init();

  for (int i = 0; i < NUM_JOINTS; ++i) {
    steppers[i] = engine.stepperConnectToPin(stepPins[i]);
    if (steppers[i]) {
		steppers[i]->setDirectionPin(dirPins[i]);
		steppers[i]->setSpeedInHz(DEFAULT_SPEED);     // default speed
		steppers[i]->setAcceleration(DEFAULT_ACCEL);  // default acceleration
    }
  }
}

void executeGCode(const String& line) {
	ESP_LOGD(TAG, "Received line: %s", line.c_str());

	if (!line.startsWith("G1")) {
		ESP_LOGW(TAG, "Unsupported or missing G-code command.");
		return;
	}

	int32_t target[4] = {0};
	bool valid[4] = {false, false, false, false};
	float feedrate = -1;

	for (int i = 0; i < line.length(); ++i) {
		char c = line[i];
		if (c >= 'A' && c <= 'D') {
			int j = i + 1;
			while (j < line.length() && (isDigit(line[j]) || line[j] == '-' || line[j] == '.')) j++;
			target[c - 'A'] = line.substring(i + 1, j).toInt();
			valid[c - 'A'] = true;
			ESP_LOGD(TAG, "Parsed joint %c = %ld steps", c, target[c - 'A']);
			i = j - 1;
		} else if (c == 'F') {
			int j = i + 1;
			while (j < line.length() && (isDigit(line[j]) || line[j] == '.' || line[j] == '-')) j++;
			feedrate = line.substring(i + 1, j).toFloat();
			ESP_LOGD(TAG, "Parsed feedrate = %.2f steps/sec", feedrate);
			i = j - 1;
		}
	}

	if (feedrate > 0) {
		for (int i = 0; i < 4; i++) {
			if (steppers[i]) {
				steppers[i]->setSpeedInHz(feedrate);
			}
		}
	}

	for (int i = 0; i < 4; i++) {
		if (valid[i] && steppers[i]) {
			ESP_LOGI(TAG, "Moving joint %c to %ld steps", 'A' + i, target[i]);
			steppers[i]->moveTo(target[i]);
		}
	}
}


void setup() {
  Serial.begin(115200);
  setupSteppers();
  Serial.println("Ready for G-code (steps only): G1 A... B... C... D... F...\nfor example: G1 A3200 B1600 D0 F2000");
}

String inputBuffer = "";
bool promptShown = false;

void loop() {
	if (!promptShown) {
		Serial.print("> ");
		promptShown = true;
	}

  	while (Serial.available()) {
    	char c = Serial.read();

    	// Echo typed character
    	Serial.write(c);

    	// Handle backspace
		if (c == '\b' || c == 127) {
			if (inputBuffer.length() > 0) {
				inputBuffer.remove(inputBuffer.length() - 1);
				Serial.print(" \b");
			} else {
				Serial.print(" ");
			}
			continue;
		}

		// End of command
		if (c == '\r' || c == '\n') {
			Serial.println();

			if (inputBuffer.length() > 0) {
				executeGCode(inputBuffer);  // 👈 Plug G-code executor here
			}

			inputBuffer = "";
			promptShown = false;
			continue;
		}

		// Normal character
		if (isPrintable(c)) {
			inputBuffer += c;
		}
	}
}
