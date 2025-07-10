#include <Arduino.h>
#include <unity.h>
#include <pins.h>
#include "Sensors/PresenceSensor/PresenceSensor.h"
#include <freertos/FreeRTOS.h>

/**
 * this test is to see if the sensors are working, when the led is lit press the switch, when it gose off relese the switch.
 * the order is link0 -> link3 
 */

static const char* TAG = "sensor_test";

PresenceSensor s0;
PresenceSensor s1;
PresenceSensor s2;
PresenceSensor s3;

void setUp(void) {
}

void tearDown(void) {
}

void test_PresenceSensors() {
    digitalWrite(BUILTIN_LED, HIGH);
    delay(5000);
    TEST_ASSERT_EQUAL(true, s0.read());

    digitalWrite(BUILTIN_LED, LOW);
    delay(5000);
    TEST_ASSERT_EQUAL(false, s0.read());

    digitalWrite(BUILTIN_LED, HIGH);
    delay(5000);
    TEST_ASSERT_EQUAL(true, s1.read());

    digitalWrite(BUILTIN_LED, LOW);
    delay(5000);
    TEST_ASSERT_EQUAL(false, s1.read());

    digitalWrite(BUILTIN_LED, HIGH);
    delay(5000);
    TEST_ASSERT_EQUAL(true, s2.read());

    digitalWrite(BUILTIN_LED, LOW);
    delay(5000);
    TEST_ASSERT_EQUAL(false, s2.read());

    digitalWrite(BUILTIN_LED, HIGH);
    delay(5000);
    TEST_ASSERT_EQUAL(true, s3.read());

    digitalWrite(BUILTIN_LED, LOW);
    delay(5000);
    TEST_ASSERT_EQUAL(false, s3.read());
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("hello from test");

    pinMode(BUILTIN_LED, OUTPUT);

    s0.begin(LINK0_SWITCH_PIN);
    s1.begin(LINK1_SWITCH_PIN, LOW);
    s2.begin(LINK2_SWITCH_PIN);
    s3.begin(LINK3_SWITCH_PIN);

    UNITY_BEGIN();
    RUN_TEST(test_PresenceSensors);
    UNITY_END();
}

void loop(){}