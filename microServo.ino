// Micro Servo and Buzzer

#include <Servo.h>

#define BUZZER_PIN 8
#define SERVO_PIN 9

// Notes (Buzzer Frequencies)
#define NOTE_C  262
#define NOTE_D  294
#define NOTE_Eb 311
#define NOTE_E  330
#define NOTE_F  349
#define NOTE_G  392
#define NOTE_Ab 415

// Melody for Overcooked Theme (simplified)
int melody[] = {
  NOTE_C, NOTE_D, NOTE_Eb, NOTE_F, NOTE_G, NOTE_G, NOTE_Eb, NOTE_F,
  NOTE_G, NOTE_G, NOTE_F, NOTE_Eb, NOTE_C, NOTE_D, NOTE_Eb, NOTE_D, 
  NOTE_G, NOTE_F, NOTE_Eb, NOTE_F, NOTE_G, NOTE_G, NOTE_Ab, NOTE_G, 
  NOTE_Eb, NOTE_C, NOTE_G, NOTE_G, NOTE_F, NOTE_Eb, NOTE_C, NOTE_D,
  NOTE_Eb, NOTE_D, NOTE_G, NOTE_F
};

// Timing variables
float unit = 218.75;
float noteDurations[] = {
  4.0*unit, 0.75*unit, 2.0*unit, 2.0*unit, 4.0*unit, 4.0*unit, 2.0*unit, 5.0*unit,
  4.0*unit, 2.0*unit, 2.0*unit, 4.0*unit, 2.0*unit, 2.0*unit, 2.0*unit, 2.0*unit, 2.0*unit,
  2.0*unit, 4.0*unit, 0.75*unit, unit, 5.0*unit, unit, 5.0*unit, 2.0*unit, 4.0*unit, 3.0*unit,
  2.0*unit, 2.0*unit, 4.0*unit, 2.0*unit, 2.0*unit, 2.0*unit, 2.0*unit, 2.0*unit, 3.0*unit
};

float pauses[] = {
  0.2*unit, 0.0*unit, 0.0*unit, 0.0*unit, 0.75*unit, 0.0*unit, 0.0*unit, 0.0*unit,
  0.0*unit, 0.0*unit, 0.0*unit, 0.0*unit, 0.0*unit, 0.0*unit, 0.0*unit, 0.0*unit, 0.0*unit,
  0.0*unit, 0.0*unit, 0.0*unit, 0.75*unit, 0.0*unit, 0.0*unit, 0.0*unit, 0.0*unit, 2.0*unit, 0.75*unit,
  0.0*unit, 0.0*unit, 0.0*unit, 0.0*unit, 0.0*unit, 0.0*unit, 0.0*unit, 0.0*unit, 0.0*unit
};

// State variables for non-blocking playback
int currentNote = 0;
unsigned long previousMillis = 0;
unsigned long startTime;
bool isPlayingNote = false;
bool isPaused = false;
bool isRunning = true;  // Controls if the melody should play

Servo servo;
int start_angle = 0; // Initial servo position
int end_angle = 160; // Final servo position
bool servoMoved = false; // Track if the servo has moved
bool servoMovedBack = true; // Track if the servo has moved back

void setup() {
  Serial.begin(9600);
  while (!Serial);  // Wait for Serial Monitor

  pinMode(BUZZER_PIN, OUTPUT);
  servo.attach(SERVO_PIN);
  servo.write(start_angle);  // Start position
  startTime = millis();  // Record the start time
}

void loop() {
  unsigned long currentMillis = millis();

  // Stop everything after 130 seconds
  if (currentMillis - startTime >= 130000) {  // 130 seconds
    noTone(BUZZER_PIN);
    isRunning = false;
    return;
  }

  // Move servo after 10 seconds
  if (!servoMoved && (currentMillis - startTime >= 5000) ) {
    // Move the servo once
    for(int angle = start_angle; angle < end_angle; angle++)  
    {                                  
      servo.write(angle);               
    }
    servoMoved = true;
    servoMovedBack = false;
  }

  // Move servo back after 20 seconds
  if (!servoMovedBack && (currentMillis - startTime >= 10000) ) {
    // Move the servo once
    for(int angle = end_angle; angle > start_angle; angle--)  
    {                                  
      servo.write(angle);               
    }
    servoMovedBack = true;
  }

  // Play melody non-blocking
  if (isRunning) {
    playMelody(currentMillis);
  }
}

void playMelody(unsigned long currentMillis) {
  if (!isPlayingNote && !isPaused) {  // Start playing a new note
    tone(BUZZER_PIN, melody[currentNote], noteDurations[currentNote]);
    previousMillis = currentMillis;
    isPlayingNote = true;
  } 
  else if (isPlayingNote && (currentMillis - previousMillis >= noteDurations[currentNote])) {
    noTone(BUZZER_PIN);  // Stop note after duration
    previousMillis = currentMillis;
    isPlayingNote = false;
    isPaused = true;
  } 
  else if (isPaused && (currentMillis - previousMillis >= pauses[currentNote])) {
    currentNote++;  // Move to the next note after the pause
    isPaused = false;

    if (currentNote >= 36) {  // Restart melody when finished
      currentNote = 0;
    }
  }
}
