#define USE_TIMER_1   true

#include "TimerInterrupt.h"
#include "ISR_Timer.h"

#define DIR_PIN_1     12
#define EN_PIN_1      11

#define DIR_PIN_2     8
#define EN_PIN_2      9

#define DIR_PIN_3     7
#define EN_PIN_3      6

typedef enum {
  STATE_GO_FORWARD, STATE_GO_BACK, STATE_GO_LEFT, STATE_GO_RIGHT, STATE_GO_NE, STATE_TURN_CCW, STATE_TURN_CW, STATE_STOP
} States_t;

States_t state;
float speed;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(DIR_PIN_1, OUTPUT);
  pinMode(EN_PIN_1, OUTPUT);
  pinMode(DIR_PIN_2, OUTPUT);
  pinMode(EN_PIN_2, OUTPUT);
  pinMode(DIR_PIN_3, OUTPUT);
  pinMode(EN_PIN_3, OUTPUT);

  digitalWrite(DIR_PIN_1, LOW);
  analogWrite(EN_PIN_1, 0);
  digitalWrite(DIR_PIN_2, LOW);
  analogWrite(EN_PIN_2, 0);
  digitalWrite(DIR_PIN_3, LOW);
  analogWrite(EN_PIN_3, 0);

  state = STATE_STOP;
  speed = 0;
}

void loop() {
  // Specify the following behaviors:
  speed = 0.9;
  state = STATE_TURN_CW;

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

// void changeState() {
//   state = state + 1;
// }
