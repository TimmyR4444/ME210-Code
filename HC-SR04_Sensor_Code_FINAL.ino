// Select the timers you're using, here ITimer1
#define USE_TIMER_1     true
#define USE_TIMER_2     true
#define USE_TIMER_3     false
#define USE_TIMER_4     false
#define USE_TIMER_5     false

#include <TimerInterrupt.h>  // Hardware timer library
#include <ISR_Timer.h>


#define TRIG_PIN 2   // Trigger pin for HC-SR04
#define ECHO_PIN 3  // Echo pin for HC-SR04

volatile long startTime = 0;  // Stores when echo starts
volatile long duration = 0;   // Stores pulse duration
volatile float distance = 0;  // Stores calculated distance

void startTrigger();
void printDistance();
void echoReceived();

void setup() {
    Serial.begin(9600);

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    // Initialize Timer1
    ITimer1.init();
    ITimer2.init();

    attachInterrupt(digitalPinToInterrupt(ECHO_PIN), echoReceived, CHANGE); // Interrupt for echo signal

    Serial.println("HC-SR04 Non-Blocking Mode Initialized");

    

    // Set up a frequency of 10Hz (every 100ms) for `startTrigger`
    if (ITimer1.setFrequency(10, startTrigger))
        Serial.println("Timer 1: Trigger set at 10Hz");
    else
        Serial.println("Timer 1 failed!");

    
}

void startTrigger() {
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);  // Ensure 10µs pulse
    digitalWrite(TRIG_PIN, LOW);
}

void echoReceived() {
    if (digitalRead(ECHO_PIN) == HIGH) {
        startTime = micros(); // Echo pulse started
        //Serial.println("Echo Start Detected!");
    } else {
        duration = micros() - startTime; // Echo pulse ended
        //Serial.println("Echo End Detected! Duration: " + String(duration) + " µs");
        distance = duration * 0.0343 / 2; // Convert to cm
    }
}

void printDistance() {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
}

void loop() {
  printDistance();
}
