#include <Servo.h>

#define SERVO_PIN1 10
#define SERVO_PIN2 13

Servo servo;
int angle = 0;

void smoothMove(Servo &s, int startAngle, int endAngle, int stepDelay) {
  if (startAngle < endAngle) {
    for (int pos = startAngle; pos <= endAngle; pos++) {
      s.write(pos);
      delay(stepDelay);  // Small delay for smooth motion
    }
  } else {
    for (int pos = startAngle; pos >= endAngle; pos--) {
      s.write(pos);
      delay(stepDelay);
    }
  }
}

void setup() {
  // Attach to first pin and move the servo
  // servo.attach(SERVO_PIN1);
  // smoothMove(servo, 0, 140, 20);  // Move from 0° to 140° slowly
  // delay(2000);

  // smoothMove(servo, 140, 0, 20);  // Move back from 140° to 0° slowly
  // delay(2000);

  // // Detach first pin before switching
  // servo.detach();
  // delay(500);

  // Attach to second pin and move the servo
  servo.attach(SERVO_PIN2);
  smoothMove(servo, 0, 120, 10);  // Move from 0° to 110° slowly
  delay(1000);
  smoothMove(servo, 120, 0, 10);  // Move back from 110° to 0° slowly
  delay(4000);
  smoothMove(servo, 0, 100, 10);  // Move from 0° to 110° slowly
  delay(1000);
  smoothMove(servo, 100, 0, 10);  // Move back from 110° to 0° slowly
}

void loop() {
}
