#include <Arduino.h>
#include <FastAccelStepper.h>
#include <config.h>
#include <pins.h>
#include "esp_log.h"

static const char* TAG = "GCODE";

const int stepPins[NUM_JOINTS] = PULSE_PINS;
const int dirPins[NUM_JOINTS]  = DIR_PINS;

// Pull in the per-joint speed/accel arrays from config.h
static const int jointSpeeds[NUM_JOINTS] = JOINT_SPEEDS;
static const int jointAccels[NUM_JOINTS] = JOINT_ACCELS;

FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper* steppers[NUM_JOINTS];

// Interrupt-driven switch state tracking
// Each switch is wired so that idle = LOW, pressed = HIGH
volatile bool switchStates[NUM_JOINTS];
volatile bool switchEvent = false;

void IRAM_ATTR handleSwitch0() {
	switchStates[0] = (digitalRead(LINK0_SWITCH_PIN) == HIGH);
	switchEvent = true;
}
void IRAM_ATTR handleSwitch1() {
	switchStates[1] = (digitalRead(LINK1_SWITCH_PIN) == HIGH);
	switchEvent = true;
}
void IRAM_ATTR handleSwitch2() {
	switchStates[2] = (digitalRead(LINK2_SWITCH_PIN) == HIGH);
	switchEvent = true;
}
void IRAM_ATTR handleSwitch3() {
	switchStates[3] = (digitalRead(LINK3_SWITCH_PIN) == HIGH);
	switchEvent = true;
}

bool prevSwitch[NUM_JOINTS];
bool switchTestMode = false;

void setupSteppers() {
	engine.init();

	for (int i = 0; i < NUM_JOINTS; ++i) {
		steppers[i] = engine.stepperConnectToPin(stepPins[i]);
		if (steppers[i]) {
			steppers[i]->setDirectionPin(dirPins[i]);

			// Use per-joint speed/acceleration
			steppers[i]->setSpeedInHz(jointSpeeds[i]);
			steppers[i]->setAcceleration(jointAccels[i]);

			ESP_LOGI(TAG, "Stepper %d initialized (STEP=%d, DIR=%d, SPEED=%d, ACCEL=%d)",
			         i, stepPins[i], dirPins[i], jointSpeeds[i], jointAccels[i]);
		} else {
			ESP_LOGE(TAG, "Failed to initialize stepper %d", i);
		}
	}
}

void homeJoint(int index, int switchPin) {
	ESP_LOGI(TAG, "Homing joint %d (fast approach)...", index);

	// Clear any previous state
	switchStates[index] = false;
	switchEvent = false;

	// Phase 1: fast ramped motion toward switch
	steppers[index]->setAcceleration(jointAccels[index]);           // use rated accel
	steppers[index]->setSpeedInHz(HOME_FAST_SPEED);                 // fast until first trigger
	steppers[index]->move(-1000000);                                 // large move toward switch

	// Wait until switch triggers (ISR sets switchStates[index])
	while (!switchStates[index]) {
		delay(1); // let ISR run
	}

	// Immediately stop ramped motion
	steppers[index]->stopMove();
	while (steppers[index]->isRunning()) {
		// wait for deceleration to finish
	}

	// Back off a bit at fast speed (no ramp; stepping manually)
	ESP_LOGI(TAG, "Switch triggered. Backing off %d steps at fast speed...", BACKOFF_STEPS);
	steppers[index]->setAcceleration(0);
	steppers[index]->setSpeedInHz(HOME_FAST_SPEED);
	for (int s = 0; s < BACKOFF_STEPS; ++s) {
		steppers[index]->runForward();
	}

	// Phase 2: slow approach without ramp (stepTimed)
	ESP_LOGI(TAG, "Approaching slowly for precise home...");
	steppers[index]->setAcceleration(0);
	steppers[index]->setSpeedInHz(HOME_SLOW_SPEED);
	while (digitalRead(switchPin) == LOW) {
		steppers[index]->runForward();
	}

	// Stop and set zero
	steppers[index]->stopMove();
	steppers[index]->setCurrentPosition(0);
	ESP_LOGI(TAG, "Joint %d homed. Position set to 0.", index);
}

void startSwitchTest() {
	// Read initial states (pressed = HIGH)
	prevSwitch[0] = (digitalRead(LINK0_SWITCH_PIN) == HIGH);
	prevSwitch[1] = (digitalRead(LINK1_SWITCH_PIN) == HIGH);
	prevSwitch[2] = (digitalRead(LINK2_SWITCH_PIN) == HIGH);
	prevSwitch[3] = (digitalRead(LINK3_SWITCH_PIN) == HIGH);

	Serial.println();
	Serial.println("=== SWITCH TEST MODE ===");
	Serial.println("Press any link switch; type EXIT + Enter to stop.");
	Serial.print("SW0=");
	Serial.print(prevSwitch[0] ? "1" : "0");
	Serial.print(" SW1=");
	Serial.print(prevSwitch[1] ? "1" : "0");
	Serial.print(" SW2=");
	Serial.print(prevSwitch[2] ? "1" : "0");
	Serial.print(" SW3=");
	Serial.println(prevSwitch[3] ? "1" : "0");

	ESP_LOGI(TAG, "Switch Test Mode ENTERED");
	switchTestMode = true;
}

void executeGCode(const String& line) {
	ESP_LOGD(TAG, "Received line: %s", line.c_str());

	String cmd = line.substring(0, 2);
	cmd.toUpperCase();

	// G0 / G1: move joints in steps
	if (cmd == "G0" || cmd == "G1") {
		int32_t target[NUM_JOINTS] = {0};
		bool valid[NUM_JOINTS]    = {false, false, false, false};
		float feedrate            = -1;

		for (int i = 0; i < line.length(); ++i) {
			char c = line[i];
			if (c >= 'A' && c <= 'D') {
				int j = i + 1;
				while (j < line.length() && (isDigit(line[j]) || line[j] == '-' || line[j] == '.')) {
					j++;
				}
				target[c - 'A'] = line.substring(i + 1, j).toInt();
				valid[c - 'A']  = true;
				ESP_LOGD(TAG, "Parsed joint %c = %ld steps", c, target[c - 'A']);
				i = j - 1;
			}
			else if (c == 'F') {
				int j = i + 1;
				while (j < line.length() && (isDigit(line[j]) || line[j] == '.' || line[j] == '-')) {
					j++;
				}
				feedrate = line.substring(i + 1, j).toFloat();
				ESP_LOGD(TAG, "Parsed feedrate = %.2f steps/sec", feedrate);
				i = j - 1;
			}
		}

		// Override speeds if feedrate provided
		if (feedrate > 0) {
			for (int i = 0; i < NUM_JOINTS; i++) {
				if (steppers[i]) {
					steppers[i]->setSpeedInHz((uint32_t)feedrate);
				}
			}
		}

		// Issue moves
		for (int i = 0; i < NUM_JOINTS; i++) {
			if (valid[i] && steppers[i]) {
				ESP_LOGI(TAG, "Moving joint %c to %ld steps", 'A' + i, target[i]);
				steppers[i]->moveTo(target[i]);
			}
		}

		// If G1, wait for all to finish
		if (cmd == "G1") {
			for (int i = 0; i < NUM_JOINTS; i++) {
				if (valid[i] && steppers[i]) {
					while (steppers[i]->isRunning()) {
						delay(1);
					}
				}
			}
		}
	}

	// M119: enter switch test mode
	else if (cmd == "M1" && line.startsWith("M119")) {
		startSwitchTest();
	}

	// G28: home all joints using switches
	else if (cmd == "G2" && line.startsWith("G28")) {
		ESP_LOGI(TAG, "Starting homing sequence for all joints...");
		homeJoint(0, LINK0_SWITCH_PIN);
		homeJoint(1, LINK1_SWITCH_PIN);
		homeJoint(2, LINK2_SWITCH_PIN);
		homeJoint(3, LINK3_SWITCH_PIN);
		ESP_LOGI(TAG, "Homing sequence complete.");
	}

	// G92: set current position as 0 (no move)
	else if (cmd == "G9" && line.startsWith("G92")) {
		ESP_LOGI(TAG, "Setting current position as 0 for all joints");
		for (int i = 0; i < NUM_JOINTS; i++) {
			if (steppers[i]) {
				steppers[i]->setCurrentPosition(0);
			}
		}
	}

	else {
		ESP_LOGW(TAG, "Unsupported or unknown command: %s", cmd.c_str());
	}
}

void setup() {
	Serial.begin(115200);

	// Configure limit switch pins as INPUT_PULLDOWN (idle = LOW)
	pinMode(LINK0_SWITCH_PIN, INPUT_PULLDOWN);
	pinMode(LINK1_SWITCH_PIN, INPUT_PULLDOWN);
	pinMode(LINK2_SWITCH_PIN, INPUT_PULLDOWN);
	pinMode(LINK3_SWITCH_PIN, INPUT_PULLDOWN);

	// Attach interrupts for switch changes
	attachInterrupt(digitalPinToInterrupt(LINK0_SWITCH_PIN), handleSwitch0, CHANGE);
	attachInterrupt(digitalPinToInterrupt(LINK1_SWITCH_PIN), handleSwitch1, CHANGE);
	attachInterrupt(digitalPinToInterrupt(LINK2_SWITCH_PIN), handleSwitch2, CHANGE);
	attachInterrupt(digitalPinToInterrupt(LINK3_SWITCH_PIN), handleSwitch3, CHANGE);

	setupSteppers();
	ESP_LOGI(TAG, "Ready for G-code (steps only):");
	ESP_LOGI(TAG, "  G0/G1 A... B... C... D... F...");
	ESP_LOGI(TAG, "  G28 => Home (uses switches), G92 => Set Home to Current, M119 => Switch Test");
	Serial.println();
}

String inputBuffer = "";
bool promptShown = false;

void loop() {
	// --- SWITCH TEST MODE ---
	if (switchTestMode) {
		if (switchEvent) {
			switchEvent = false;

			bool curr[NUM_JOINTS];
			curr[0] = switchStates[0];
			curr[1] = switchStates[1];
			curr[2] = switchStates[2];
			curr[3] = switchStates[3];

			bool changed = false;
			for (int i = 0; i < NUM_JOINTS; i++) {
				if (curr[i] != prevSwitch[i]) {
					changed = true;
					prevSwitch[i] = curr[i];
				}
			}
			if (changed) {
				char statusLine[50];
				snprintf(statusLine, sizeof(statusLine),
				         "SW0=%d SW1=%d SW2=%d SW3=%d",
				         curr[0] ? 1 : 0,
				         curr[1] ? 1 : 0,
				         curr[2] ? 1 : 0,
				         curr[3] ? 1 : 0
				);
				Serial.print("\r");
				Serial.print(statusLine);
				Serial.print("    ");
			}
		}

		if (Serial.available()) {
			char c = Serial.read();
			if (isPrintable(c)) {
				inputBuffer += c;
			}
			if (c == '\r' || c == '\n') {
				inputBuffer.trim();
				if (inputBuffer.equalsIgnoreCase("EXIT")) {
					switchTestMode = false;
					Serial.println();
					ESP_LOGI(TAG, "Exiting switch test mode");
					promptShown = false;
				}
				inputBuffer = "";
			}
		}
		return;
	}

	// --- NORMAL PROMPT MODE ---
	if (!promptShown) {
		Serial.print("> ");
		promptShown = true;
	}

	while (Serial.available()) {
		char c = Serial.read();
		Serial.write(c);

		if (c == '\b' || c == 127) {
			if (inputBuffer.length() > 0) {
				inputBuffer.remove(inputBuffer.length() - 1);
				Serial.print(" \b");
			} else {
				Serial.print(" ");
			}
			continue;
		}

		if (c == '\r' || c == '\n') {
			Serial.println();
			if (inputBuffer.length() > 0) {
				executeGCode(inputBuffer);
			}
			inputBuffer = "";
			promptShown = false;
			continue;
		}

		if (isPrintable(c)) {
			inputBuffer += c;
		}
	}
}