#define DIR_PIN_1     12
#define EN_PIN_1      11

#define DIR_PIN_2     8
#define EN_PIN_2      9

#define DIR_PIN_3     7
#define EN_PIN_3      6

void setup() {
  pinMode(DIR_PIN_1, OUTPUT);
  pinMode(EN_PIN_1, OUTPUT);
  pinMode(DIR_PIN_2, OUTPUT);
  pinMode(EN_PIN_2, OUTPUT);
  pinMode(DIR_PIN_3, OUTPUT);
  pinMode(EN_PIN_3, OUTPUT);

  digitalWrite(DIR_PIN_1, LOW);
  analogWrite(EN_PIN_1, 200);
  digitalWrite(DIR_PIN_2, LOW);
  analogWrite(EN_PIN_2, 200);
  digitalWrite(DIR_PIN_3, LOW);
  analogWrite(EN_PIN_3, 200);
}

void loop() {
  // put your main code here, to run repeatedly:

}
