#define USE_TIMER_1     false
#define USE_TIMER_2     true
// #define USE_TIMER_3     true

#include <TimerInterrupt.h>  // Hardware timer library
#include <ISR_Timer.h>
#include <Servo.h>

#define TRIG_PIN      2   // Trigger pin for HC-SR04
#define ECHO_PIN      3   // Echo pin for HC-SR04

#define DIR_PIN_1     4
#define EN_PIN_1      5

#define SERVO_PIN     6

#define DIR_PIN_2     8
#define EN_PIN_2      9

#define DIR_PIN_3     12
#define EN_PIN_3      11

typedef enum {
  STATE_GO_FORWARD, STATE_GO_BACK, STATE_GO_LEFT, STATE_GO_RIGHT, STATE_GO_NE, STATE_TURN_CCW, STATE_TURN_CW, STATE_STOP
} States_t;

States_t state;
float speed;
int stopDist = 5;

/*
Servo servo;
unsigned long startTime;
int start_angle = 0; // Initial servo position
int end_angle = 160; // Final servo position
bool servoMoved = false; // Track if the servo has moved
bool servoMovedBack = true; // Track if the servo has moved back
*/

volatile long startEcho = 0;  // Stores when echo starts
volatile long duration = 0;   // Stores pulse duration
volatile float distance = 0;  // Stores calculated distance

void startTrigger();
void printDistance();
void echoReceived();

void setup() {
  Serial.begin(9600);

  // Motor Area
  pinMode(DIR_PIN_1, OUTPUT);
  pinMode(EN_PIN_1, OUTPUT);
  pinMode(DIR_PIN_2, OUTPUT);
  pinMode(EN_PIN_2, OUTPUT);
  pinMode(DIR_PIN_3, OUTPUT);
  pinMode(EN_PIN_3, OUTPUT);

  digitalWrite(DIR_PIN_1, HIGH);
  analogWrite(EN_PIN_1, 255);
  digitalWrite(DIR_PIN_2, HIGH);
  analogWrite(EN_PIN_2, 255);
  digitalWrite(DIR_PIN_3, HIGH);
  analogWrite(EN_PIN_3, 255);

  state = STATE_STOP;
  speed = 0;

  // Ultrasonic Area
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  //ITimer1.init();
  ITimer2.init();
  //ITimer3.init();

  attachInterrupt(digitalPinToInterrupt(ECHO_PIN), echoReceived, CHANGE); // Interrupt for echo signal

  Serial.println("HC-SR04 Non-Blocking Mode Initialized");

  // Set up a frequency of 10Hz (every 100ms) for `startTrigger`
  if (ITimer2.setFrequency(10, startTrigger)) {
    Serial.println("Timer 1: Trigger set at 10Hz");
  } else {
    Serial.println("Timer 1 failed!");
  }


  // Print distance every 500ms (2Hz)
  //if (ITimer3.setFrequency(2, printDistance)) {
  //  Serial.println("Timer 2: Print distance set at 2Hz");
  //} else {
  //  Serial.println("Timer 2 failed!");
  //}
}

void startTrigger() {
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);  // Ensure 10µs pulse
    digitalWrite(TRIG_PIN, LOW);
}

void echoReceived() {
  if (digitalRead(ECHO_PIN) == HIGH) {
    startEcho = micros(); // Echo pulse started
    //Serial.println("Echo Start Detected!");
  } else {
    duration = micros() - startEcho; // Echo pulse ended
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

  // put your main code here, to run repeatedly:
  // Serial.println(state);

  // Specify the following behaviors:
  speed = 0.5;

  if (distance < stopDist) {
    state = STATE_STOP;
  } else {
    state = STATE_GO_FORWARD;
  }

  switch (state) {
    case STATE_GO_FORWARD:
      //handleMotion(0, 255, 0);
      digitalWrite(DIR_PIN_1, LOW);
      analogWrite(EN_PIN_1, 221*speed);
      
      digitalWrite(DIR_PIN_2, HIGH);
      analogWrite(EN_PIN_2, 221*speed);

      analogWrite(EN_PIN_3, 0);
      break;
    case STATE_GO_BACK:
      //handleMotion(0, -255, 0);
      digitalWrite(DIR_PIN_1, HIGH);
      analogWrite(EN_PIN_1, 221*speed);

      digitalWrite(DIR_PIN_2, LOW);
      analogWrite(EN_PIN_2, 221*speed);

      analogWrite(EN_PIN_3, 0);
      break;
    case STATE_GO_LEFT:
      //handleMotion(-255, 0, 0);
      digitalWrite(DIR_PIN_1, LOW);
      analogWrite(EN_PIN_1, 127*speed);

      digitalWrite(DIR_PIN_2, LOW);
      analogWrite(EN_PIN_2, 127*speed);

      digitalWrite(DIR_PIN_3, HIGH);
      analogWrite(EN_PIN_3, 255*speed);
      break;
    case STATE_GO_RIGHT:
      //handleMotion(255, 0, 0);
      digitalWrite(DIR_PIN_1, HIGH);
      analogWrite(EN_PIN_1, 127*speed);

      digitalWrite(DIR_PIN_2, HIGH);
      analogWrite(EN_PIN_2, 127*speed);

      digitalWrite(DIR_PIN_3, LOW);
      analogWrite(EN_PIN_3, 255*speed);
      break;
    case STATE_GO_NE:
      //handleMotion(200, 200, 0);
      digitalWrite(DIR_PIN_1, LOW);
      analogWrite(EN_PIN_1, 66*speed);

      digitalWrite(DIR_PIN_2, HIGH);
      analogWrite(EN_PIN_2, 246*speed);

      digitalWrite(DIR_PIN_3, LOW);
      analogWrite(EN_PIN_3, 180*speed);
      break;
    case STATE_TURN_CCW:
      //handleMotion(0, 0, 255);
      digitalWrite(DIR_PIN_1, LOW);
      analogWrite(EN_PIN_1, 255*speed);

      digitalWrite(DIR_PIN_2, LOW);
      analogWrite(EN_PIN_2, 255*speed);

      digitalWrite(DIR_PIN_3, LOW);
      analogWrite(EN_PIN_3, 255*speed);
      break;
    case STATE_TURN_CW:
      //handleMotion(0, 0, -255);
      digitalWrite(DIR_PIN_1, HIGH);
      analogWrite(EN_PIN_1, 255*speed);

      digitalWrite(DIR_PIN_2, HIGH);
      analogWrite(EN_PIN_2, 255*speed);

      digitalWrite(DIR_PIN_3, HIGH);
      analogWrite(EN_PIN_3, 255*speed);
      break;
    case STATE_STOP:
      //handleMotion(0, 0, 0);
      analogWrite(EN_PIN_1, 0);
      analogWrite(EN_PIN_2, 0);
      analogWrite(EN_PIN_3, 0);
      break;
    default:
      Serial.println("What");
  }
}

void handleMotion(int v_x, int v_y, int Omega) {
  
}
