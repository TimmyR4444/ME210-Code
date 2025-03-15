#define TRIG_PIN 4   // Trigger pin for HC-SR04
#define ECHO_PIN 3  // Echo pin for HC-SR04

volatile long startTime = 0;   // Stores when echo starts
volatile long duration = 0;    // Stores pulse duration
volatile float distance = 0;   // Stores calculated distance

unsigned long lastTriggerTime = 0;  // For controlling the trigger pulse
unsigned long lastPrintTime = 0;   // For printing distance periodically
unsigned long triggerDuration = 10; // Pulse duration in microseconds (10µs)

void echoReceived();

void setup() {
    Serial.begin(9600);

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    attachInterrupt(digitalPinToInterrupt(ECHO_PIN), echoReceived, CHANGE); // Interrupt for echo signal

    Serial.println("HC-SR04 Non-Blocking Mode Initialized");
}

void loop() {
    unsigned long currentMillis = millis();

    // Trigger every 100ms (10Hz) to send the ultrasonic pulse
    if (currentMillis - lastTriggerTime >= 100) {
        startTrigger();
        lastTriggerTime = currentMillis;
    }

    printDistance();
}


void startTrigger() {
    unsigned long currentMicros = micros();

    // Trigger the pulse only if it's been more than 10µs since the last trigger
    if (currentMicros - lastTriggerTime >= triggerDuration) {
        digitalWrite(TRIG_PIN, HIGH);  // Start pulse at TRIG_PIN (LOW -> HIGH)
        lastTriggerTime = currentMicros; // Update the last trigger time

        // After 10µs, set the TRIG_PIN to LOW
        digitalWrite(TRIG_PIN, LOW); 
    }
}

void echoReceived() {
    if (digitalRead(ECHO_PIN) == HIGH) {
        startTime = micros(); // Record the time when the echo starts
    } else {
        duration = micros() - startTime; // Measure the time when the echo ends
        distance = duration * 0.0343 / 2; // Convert to centimeters
    }
}

void printDistance() {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
}
