#define BUZZER_PIN 8

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
  0.2*unit, 0.0*unit, 0.0*unit, 0.0*unit, 0.75*unit, 0.0*unit, 0.0*unit, 0.75*unit,
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

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  startTime = millis();  // Record the start time
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - startTime >= 130000) {  // 130 seconds
    noTone(BUZZER_PIN);  // Ensure the buzzer stops
    isRunning = false;
    return;
  }

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
