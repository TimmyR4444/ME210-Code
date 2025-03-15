#include "Wire.h"
#include "sensorbar.h"

// Uncomment one of the four lines to match your SX1509's address
const uint8_t SX1509_ADDRESS = 0x3E;  // SX1509 I2C address (00)
SensorBar mySensorBar(SX1509_ADDRESS);
volatile int sensorReadings[8];

void setup()
{
  Serial.begin(9600);  // start serial for output
  Serial.println("Program started.");
  Serial.println();
  
  //For this demo, the IR will only be turned on during reads.
  //mySensorBar.setBarStrobe();
  //Other option: Command to run all the time
  mySensorBar.clearBarStrobe();

  //Default dark on light
  mySensorBar.clearInvertBits();
  //Other option: light line on dark
  //mySensorBar.setInvertBits();
  
  //Don't forget to call .begin() to get the bar ready.  This configures HW.
  uint8_t returnStatus = mySensorBar.begin();
  if(returnStatus)
  {
	  Serial.println("sx1509 IC communication OK");
  }
  else
  {
	  Serial.println("sx1509 IC communication FAILED!");
	  while(1);
  }
  Serial.println();
  
}

void loop()
{
  // Get the data from the sensor bar
  uint8_t rawValue = mySensorBar.getRaw();

  // Print individual sensor readings
  for (int i = 7; i >= 0; i--)
  {
    sensorReadings[i] = (rawValue >> i) & 0x01;
  }

  isMiddleOnTape();
}

void isMiddleOnTape() {
  if (sensorReadings[3] == 1) {
    Serial.println("Yes. We're over tape.");
  } else {
    Serial.println("No. We're not over tape.");
  }
}


