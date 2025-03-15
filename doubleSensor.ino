#define SERVO_PIN1    10  // Igniter presser servo pin
#define SERVO_PIN2    13  // Ingredient gate servo pin

#define ECHO_PIN_1    2   // Echo pin for first HC-SR04
#define TRIG_PIN_1    5   // Trigger pin for first HC-SR04

#define ECHO_PIN_2    3   // Echo pin for second HC-SR04
#define TRIG_PIN_2    4   // Trigger pin for second HC-SR04

#define DIR_PIN_1     12  // Motor 1 direction pin
#define EN_PIN_1      11  // Motor 1 enable pin

#define DIR_PIN_2     8   // Motor 2 direction pin
#define EN_PIN_2      9   // Motor 2 enable pin

#define DIR_PIN_3     7   // Motor 3 direction pin
#define EN_PIN_3      6   // Motor 3 enable pin


volatile long startTime_1 = 0;   // Stores when echo starts for sensor 1
volatile long duration_1 = 0;    // Stores pulse duration for sensor 1
volatile float distance_1 = 0;   // Stores calculated distance for sensor 1

volatile long startTime_2 = 0;   // Stores when echo starts for sensor 2
volatile long duration_2 = 0;    // Stores pulse duration for sensor 2
volatile float distance_2 = 0;   // Stores calculated distance for sensor 2

unsigned long lastTriggerTime_1 = 0;  // For controlling the trigger pulse for sensor 1
unsigned long lastTriggerTime_2 = 0;  // For controlling the trigger pulse for sensor 2
unsigned long triggerDuration = 10;   // Pulse duration in microseconds (10µs)

volatile float min_distance = 1000.0;   // Initialize minimum distance measured by ultrasonic sensors
volatile float total_distance = 0.0;    // Initialize combined distance measured by ultrasonic sensors

float speed = 1;    // Motor speed (ranges from 0 to 1)

void echoReceived_1();
void echoReceived_2();

void setup() {
    Serial.begin(9600);
    // ---------- //
    // True Setup //
    // ---------- //

    // Assign motor pins and set to low
    pinMode(DIR_PIN_1, OUTPUT);
    pinMode(EN_PIN_1, OUTPUT);
    digitalWrite(DIR_PIN_1, LOW);
    digitalWrite(EN_PIN_1, LOW);
    
    pinMode(DIR_PIN_2, OUTPUT);
    pinMode(EN_PIN_2, OUTPUT);
    digitalWrite(DIR_PIN_2, LOW);
    digitalWrite(EN_PIN_2, LOW);

    pinMode(DIR_PIN_3, OUTPUT);
    pinMode(EN_PIN_3, OUTPUT);
    digitalWrite(DIR_PIN_3, LOW);
    digitalWrite(EN_PIN_3, LOW);

    // Assign ultrasonic sensor pins
    pinMode(TRIG_PIN_1, OUTPUT);
    pinMode(ECHO_PIN_1, INPUT);
    pinMode(TRIG_PIN_2, OUTPUT);
    pinMode(ECHO_PIN_2, INPUT);

    // Attach interrupts for both ultrasonic sensors
    attachInterrupt(digitalPinToInterrupt(ECHO_PIN_1), echoReceived_1, CHANGE); // Interrupt for echo signal of sensor 1
    attachInterrupt(digitalPinToInterrupt(ECHO_PIN_2), echoReceived_2, CHANGE); // Interrupt for echo signal of sensor 2

    // ------------------- //
    // Hard Code Movements //
    // ------------------- //

    // Set both servos to intial angle
    moveServo1(0);
    moveServo2(0);
    delay(200);
    // Competition starting flag
    moveServo1(250);
    delay(200);
    moveServo1(0);

    unsigned long startMillis = millis();

    // Start wheels at max speed to overcome any friction
    while ( (millis() - startMillis) < 100) {
      digitalWrite(DIR_PIN_1, LOW);
      analogWrite(EN_PIN_1, 255);

      digitalWrite(DIR_PIN_2, LOW);
      analogWrite(EN_PIN_2, 255);
    
      digitalWrite(DIR_PIN_3, LOW);
      analogWrite(EN_PIN_3, 255);
    }

    startMillis = millis();

    // Record combined distance measured by ultrasonics
    while ( (millis() - startMillis) < 6000) {
      measureTurn();
    }

    // Continue turning until measured distance is within minimum distance threshold
    while (total_distance > (min_distance + 5)) {
      compareTurn();
    }

    startMillis = millis();

    // Start aligned with left wall
    while ( (millis() - startMillis) < 200) {
      moveLeft();
    }

    startMillis = millis();

    // Move forward to pass barrier between start and pantry
    while ( (millis() - startMillis) < 400) {
      moveForward();
    }

    startMillis = millis();

    // Move right until reach other side of track
    while ( (millis() - startMillis) < 3500) {
      moveRight();
    }

    startMillis = millis();

    // Stop wheels to prepare to revers
    while ( (millis() - startMillis) < 500) {
      makeStop();
    }

    startMillis = millis();

    // Reverse into pantry
    while ( (millis() - startMillis) < 1500) {
      moveBack();
    }

    startMillis = millis();

    // Stop in pantry while loading ingredients
    while ( (millis() - startMillis) < 1500) {
      makeStop();
    }
    
    startMillis = millis();

    // Move forward until we hit the customer window
    while ( (millis() - startMillis) < 2500) {
      moveForward();
    }

    startMillis = millis();

    // Move back from the wall
    while ( (millis() - startMillis) < 200) {
      moveBack();
    }

    startMillis = millis();

    // Move left until pot is moved to burner
    while ( (millis() - startMillis) < 4000) {
      moveLeft();
    }

    startMillis = millis();

    // Stop pushing pot
    while ( (millis() - startMillis) < 100) {
      makeStop();
    }

    startMillis = millis();

    // Move forward to correct orientation
    while ( (millis() - startMillis) < 1000) {
      moveForward();
    }

    startMillis = millis();

    // Stop to prepare to reverse
    while ( (millis() - startMillis) < 1000) {
      makeStop();
    }

    startMillis = millis();

    // Move back to align with igniter block
    while ( (millis() - startMillis) < 1000) {
      moveBack();
    }

    startMillis = millis();

    // Move beside igniter block
    while ( (millis() - startMillis) < 1000) {
      moveLeft();
    }

    // Stop while hitting igniter block
    makeStop();
    delay(1000);

    // Move servo arm to hit block
    moveServo1(250);
    delay(1000);

    startMillis = millis();

    // Move right to clear pot arm
    while ( (millis() - startMillis) < 25) {
      moveRight();
    }

    startMillis = millis();

    // Reverse to align with back wall
    while ( (millis() - startMillis) < 1500) {
      moveBack();
    }

    // Move servo arm back to inital position
    moveServo1(0);

    startMillis = millis();

    // Move bot in front of the pot
    while ( (millis() - startMillis) < 1200) {
      moveForward();
    }

    // Stop to load ingredients
    makeStop();

    // Raise and lower servo arm to drop ingredients in pot
    delay(500);
    moveServo2(200);
    delay(1500);
    moveServo2(0);
    delay(1000);

    startMillis = millis();

    // Move back from pot until aligned with igniter block
    while ( (millis() - startMillis) < 500) {
      moveBack();
    }

    startMillis = millis();

    // Move next to igniter block
    while ( (millis() - startMillis) < 500) {
      moveLeft();
    }

    // Stop while turning off burner
    makeStop();

    // Move servo arm to turn off burner
    delay(1000);
    moveServo1(100);
    delay(250);
    moveServo1(0);


    startMillis = millis();

    // Move right to clear pot arm
    while ( (millis() - startMillis) < 50) {
      moveRight();
    }

    startMillis = millis();

    // Reverse to align with back wall
    while ( (millis() - startMillis) < 1500) {
      moveBack();
    }

    startMillis = millis();

    // Move forward to get inside pot arms
    while ( (millis() - startMillis) < 1500) {
      moveForward();
    }
    makeStop();

    startMillis = millis();

    // Back up from pot wall
    while ( (millis() - startMillis) < 100) {
      moveBack();
    }

    startMillis = millis();

    // Move right until pot is over customer window
    while ( (millis() - startMillis) < 7000) {
      digitalWrite(DIR_PIN_1, HIGH);
      analogWrite(EN_PIN_1, 130*speed);

      digitalWrite(DIR_PIN_2, HIGH);
      analogWrite(EN_PIN_2, 130*speed);

      digitalWrite(DIR_PIN_3, LOW);
      analogWrite(EN_PIN_3, 255*speed);
    }

    // Stop moving once ingredients delivered
    makeStop();

    // End of competition flag
    delay(1000);
    moveServo2(200);
    delay(250);
    moveServo2(0);
    delay(1000);
}

void loop() {
  // Everything handled in setup
}

// Rotates bot and compares combined distance measured by ultrasonic sensors
// to the minimum distance. If the recorded distance is less than the minimum
// distance, the minimum distance is updated to that value.
void measureTurn() {
    // Rotate bot
    digitalWrite(DIR_PIN_1, LOW);
    analogWrite(EN_PIN_1, 255*0.3);

    digitalWrite(DIR_PIN_2, LOW);
    analogWrite(EN_PIN_2, 255*0.3);
    
    digitalWrite(DIR_PIN_3, LOW);
    analogWrite(EN_PIN_3, 255*0.3);

    unsigned long currentMillis = millis();

    // Trigger sensor 1 every 100ms (10Hz)
    if (currentMillis - lastTriggerTime_1 >= 100) {
        startTrigger_1();
        lastTriggerTime_1 = currentMillis;
    }

    // Trigger sensor 2 every 100ms (10Hz)
    if (currentMillis - lastTriggerTime_2 >= 100) {
        startTrigger_2();
        lastTriggerTime_2 = currentMillis;
    }

    total_distance = distance_1 + distance_2;

    // Update minimum distance
    if ((total_distance < min_distance) && total_distance > 0.5) {
      min_distance = total_distance;
    }
}

// Rotates bot and updates total distance variable based on current
// total distance measured
void compareTurn() {
    // Rotate robot
    digitalWrite(DIR_PIN_1, LOW);
    analogWrite(EN_PIN_1, 255*0.3);

    digitalWrite(DIR_PIN_2, LOW);
    analogWrite(EN_PIN_2, 255*0.3);
    
    digitalWrite(DIR_PIN_3, LOW);
    analogWrite(EN_PIN_3, 255*0.3);

    unsigned long currentMillis = millis();

    // Trigger sensor 1 every 100ms (10Hz)
    if (currentMillis - lastTriggerTime_1 >= 100) {
        startTrigger_1();
        lastTriggerTime_1 = currentMillis;
    }

    // Trigger sensor 2 every 100ms (10Hz)
    if (currentMillis - lastTriggerTime_2 >= 100) {
        startTrigger_2();
        lastTriggerTime_2 = currentMillis;
    }

    total_distance = distance_1 + distance_2;
}

// Stop robot
void makeStop() {
  analogWrite(EN_PIN_1, 0);
  analogWrite(EN_PIN_2, 0);
  analogWrite(EN_PIN_3, 0);
}

// Move robot forward
void moveForward() {
  digitalWrite(DIR_PIN_1, HIGH);
  analogWrite(EN_PIN_1, 221*speed);
      
  digitalWrite(DIR_PIN_2, LOW);
  analogWrite(EN_PIN_2, 221*speed);

  analogWrite(EN_PIN_3, 0);
}

// Move robot right
void moveRight() {
  digitalWrite(DIR_PIN_1, HIGH);
  analogWrite(EN_PIN_1, 135*speed);

  digitalWrite(DIR_PIN_2, HIGH);
  analogWrite(EN_PIN_2, 135*speed);

  digitalWrite(DIR_PIN_3, LOW);
  analogWrite(EN_PIN_3, 255*speed);
}

// Move robot left
void moveLeft() {
  digitalWrite(DIR_PIN_1, LOW);
  analogWrite(EN_PIN_1, 130*speed);

  digitalWrite(DIR_PIN_2, LOW);
  analogWrite(EN_PIN_2, 130*speed);

  digitalWrite(DIR_PIN_3, HIGH);
  analogWrite(EN_PIN_3, 255*speed);
}

// Move robot in reverse
void moveBack() {
  digitalWrite(DIR_PIN_1, LOW);
  analogWrite(EN_PIN_1, 221*speed);

  digitalWrite(DIR_PIN_2, HIGH);
  analogWrite(EN_PIN_2, 221*speed);

  analogWrite(EN_PIN_3, 0);
}

// Start ultrasonic sensor 1 trigger
void startTrigger_1() {
    unsigned long currentMicros = micros();

    // Trigger pulse for sensor 1
    if (currentMicros - lastTriggerTime_1 >= triggerDuration) {
        digitalWrite(TRIG_PIN_1, HIGH);  // Start pulse at TRIG_PIN_1 (LOW -> HIGH)
        lastTriggerTime_1 = currentMicros; // Update the last trigger time

        // After 10µs, set the TRIG_PIN_1 to LOW
        digitalWrite(TRIG_PIN_1, LOW); 
    }
}

// Start ultrasonic sensor 2 trigger
void startTrigger_2() {
    unsigned long currentMicros = micros();

    // Trigger pulse for sensor 2
    if (currentMicros - lastTriggerTime_2 >= triggerDuration) {
        digitalWrite(TRIG_PIN_2, HIGH);  // Start pulse at TRIG_PIN_2 (LOW -> HIGH)
        lastTriggerTime_2 = currentMicros; // Update the last trigger time

        // After 10µs, set the TRIG_PIN_2 to LOW
        digitalWrite(TRIG_PIN_2, LOW); 
    }
}

// Receive ultrasonic sensor 1 echo
void echoReceived_1() {
    if (digitalRead(ECHO_PIN_1) == HIGH) {
        startTime_1 = micros(); // Record the time when the echo starts for sensor 1
    } else {
        duration_1 = micros() - startTime_1; // Measure the time when the echo ends for sensor 1
        distance_1 = duration_1 * 0.0343 / 2; // Convert to centimeters for sensor 1
    }
}

// Receive ultrasonic sensor 2 echo
void echoReceived_2() {
    if (digitalRead(ECHO_PIN_2) == HIGH) {
        startTime_2 = micros(); // Record the time when the echo starts for sensor 2
    } else {
        duration_2 = micros() - startTime_2; // Measure the time when the echo ends for sensor 2
        distance_2 = duration_2 * 0.0343 / 2; // Convert to centimeters for sensor 2
    }
}

// Move servo arm 1 to specified angle without using Timer1 or Servo.h
void moveServo1(int angle) {
    int pulseWidth = map(angle, 0, 180, 1000, 2000); // Convert angle to pulse width in microseconds

    unsigned long startTime = millis(); // Track the current time
    while (millis() - startTime < 1000) { // Keep sending pulses for 1 second
        digitalWrite(SERVO_PIN1, HIGH);
        delayMicroseconds(pulseWidth); // Keep high for the pulse duration
        digitalWrite(SERVO_PIN1, LOW);
        delay(20); // Maintain a 50Hz refresh rate (20ms delay per pulse)
    }
}

// Move servo arm 2 to specified angle without using Timer1 or Servo.h
void moveServo2(int angle) {
    int pulseWidth = map(angle, 0, 180, 1000, 2000); // Convert angle to pulse width in microseconds

    unsigned long startTime = millis(); // Track the current time
    while (millis() - startTime < 1000) { // Keep sending pulses for 1 second
        digitalWrite(SERVO_PIN2, HIGH);
        delayMicroseconds(pulseWidth); // Keep high for the pulse duration
        digitalWrite(SERVO_PIN2, LOW);
        delay(20); // Maintain a 50Hz refresh rate (20ms delay per pulse)
    }
}