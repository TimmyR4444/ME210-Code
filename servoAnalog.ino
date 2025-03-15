#define SERVO_PIN 13  // Connect the servo signal wire to pin 9

void moveServo(int angle) {
    int pulseWidth = map(angle, 0, 180, 1000, 2000); // Convert angle to pulse width in microseconds

    unsigned long startTime = millis(); // Track the current time
    while (millis() - startTime < 1000) { // Keep sending pulses for 1 second
        digitalWrite(SERVO_PIN, HIGH);
        delayMicroseconds(pulseWidth); // Keep high for the pulse duration
        digitalWrite(SERVO_PIN, LOW);
        delay(20); // Maintain a 50Hz refresh rate (20ms delay per pulse)
    }
}

void openClose() {
  moveServo(180);  // Move to 90 degrees
  delay(4000);
  moveServo(0);
}

void hitButton() {
  moveServo(360);  // Move to 90 degrees
  delay(1000);
  moveServo(0);
}

void setup() {
    moveServo(0);   // Move to 0 degrees
    pinMode(SERVO_PIN, OUTPUT);
}

void loop() {
  moveServo(0);
  delay(1000);
  moveServo(250);
  delay(1000);
}
