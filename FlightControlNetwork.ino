
/* Using MHeironimus' ArduinoJoystickLibrary
https://github.com/MHeironimus/ArduinoJoystickLibrary */
#include "Joystick.h"
#include <math.h>

///////////////////////////////////// COPIED CODE ////////////////////////////

//Constants
#define NB_POT 1



///////////////////////////////////// COPIED CODE ////////////////////////////


// Create Joystick
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, 
  JOYSTICK_TYPE_MULTI_AXIS, 0, 0,
  false, false, false, false, false, false,
  true, true, false, false, false);

// Digital input pin setup:

const int rudderEncoder1Pin = 0; // Rudder
const int rudderEncoder2Pin = 1; // Rudder
const int rudderButtonPin = 2;

const int throttleToggleFPin = 2; // Throttle
const int throttleToggleBPin = 3; // Throttle
const int throttleUpDownPin  = A0;


const int inPin4 = 4;

const int inPin5 = 5; 

const int inPin6 = 6; // Throttle Toggle
const int inPin7 = 7; // Throttle Toggle

const int inPin8 = 8; // Throttle Button
const int inPin9 = 9; // Throttle Button
const int inPin10 = 10; // Throttle Button
const int inPin11 = 11; // Throttle Button
const char initialPos = 0;

// Rudder related vars.
const int rudderRangeMin = -127;
const int rudderRangeMax = 127;
char RudderPos = initialPos;
int rudderEncoder1State;
int rudderEncoder1LastState;
int rudderEncoder2State;
int rudderButtonPress;


const int throttleRangeMin = -127;
const int throttleRangeMax = 127;
char throttlePos = initialPos;
int throttleToggleF;
int throttleToggleB;
int throttleUpDownState = 0;

int pin5State;
  int pin6State;
  int pin7State;
  int pin8State;
  int pin9State; 
  int pin10State;
  int pin11State;

// Loop check for debugging. Only iterates loops on changes.
int loopN = 1;


// The setup routine runs once when you first add power or press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
  // Set up pin inputs and outputs.
  pinMode(rudderEncoder1Pin, INPUT);
  pinMode(rudderEncoder2Pin, INPUT);
  pinMode(rudderButtonPin, INPUT);
  pinMode(throttleToggleFPin, INPUT);
  pinMode(throttleToggleBPin, INPUT);
  pinMode(inPin5, INPUT_PULLUP );
  pinMode(inPin6, INPUT_PULLUP );
  pinMode(inPin7, INPUT);
  pinMode(inPin8, INPUT);
  pinMode(inPin9, INPUT);
  pinMode(inPin10, INPUT);
  pinMode(inPin11, INPUT);
  // Copied code!!!!!
  Serial.println(F("Initialize System"));
  //Init potentiometer
  pinMode(throttleUpDownPin, INPUT);

  
  // Prepare Joystick and Joystick range values.
  //Joystick.setXAxisRange(-127, 127);
  //Joystick.setYAxisRange(-127, 127);
  //Joystick.setZAxisRange(-127, 127);
  //Joystick.setRxAxisRange(0, 360);
  //Joystick.setRyAxisRange(360, 0);
  //Joystick.setRzAxisRange(0, 720);
  //Joystick.setThrottleRange(0, 255);
  Joystick.setThrottleRange(throttleRangeMin, throttleRangeMax);
  //Joystick.setRudderRange(255, 0);
  Joystick.setRudderRange(rudderRangeMin, rudderRangeMax);
  Joystick.begin();

  // Initialize rudderEncoder1LastState with a starter value.
  rudderEncoder1LastState = digitalRead(rudderEncoder1Pin);

  // Information output.
  Serial.println("End of Initialize.");
}

// The loop routine runs over and over again forever:
void loop() {
  // Read the input pins:
  // Rudder Items
  rudderEncoder1State = digitalRead(rudderEncoder1Pin); // Rudder
  rudderEncoder2State = digitalRead(rudderEncoder2Pin); // Rudder
  rudderButtonPress = digitalRead(rudderButtonPin);     // Rudder
  
  throttleToggleF = digitalRead(throttleToggleFPin);    // Throttle
  throttleToggleB = digitalRead(throttleToggleBPin);    // Throttle
  throttleUpDownState = analogRead(throttleUpDownPin);  // Throttle
  pin5State = digitalRead(inPin5); // Throttle
  pin6State = digitalRead(inPin6); // Throttle
  pin7State = digitalRead(inPin7); // Throttle
  pin8State = digitalRead(inPin8); // Throttle
  pin9State = digitalRead(inPin9); // Throttle
  pin10State = digitalRead(inPin10); // Throttle
  pin11State = digitalRead(inPin11); // Throttle

  readPot();

  // Read throttle inputs
  // The max and min seems to be... 919, 112
  int throttledUp = 112;
  int throttledDown = 919;
  if (throttleToggleF == 1 || throttleToggleB == 1) {
    // for some reason, 924 and 153 if shields up
    throttledUp = 153;
    throttledDown = 924;
  } else {
    
  }
  int throttleMap = map(throttleUpDownState, throttledDown, throttledUp, throttleRangeMin, throttleRangeMax);
  Joystick.setThrottle(throttleMap);

  // Read rudder inputs
  if (rudderEncoder1State != rudderEncoder1LastState) {
    if (rudderEncoder2State != rudderEncoder1State) {
      RudderPos++;
      Joystick.setRudder(RudderPos);
      //Joystick.setThrottle(RudderPos);
    } else {
      RudderPos--;
      Joystick.setRudder(RudderPos);
      //Joystick.setThrottle(RudderPos);
    }
    rudderEncoder1LastState = rudderEncoder1State;
  }
  outputDebugValues();
  // String output;
    String output = String("|") 
                    + " Position:" + displaySpacing((int)RudderPos, 3, true)
                    + "  |  Set number:" + displaySpacing((int)loopN, 5, false)
                    + "  |  (" + rudderEncoder1State + ", " + rudderEncoder2State + ")"
                    + "  | Rudder Click: " + rudderButtonPress
                    + "  |  Throttle Toggle: " + throttleToggleF + " " + throttleToggleB
                    + "  |  Throttled to " + throttleUpDownState + " " + throttleMap
                    + "  |  p5: " + pin5State
                    + "  |  p6: " + pin6State
                    + "  |  p7: " + pin7State
                    + "  |  p8: " + pin8State
                    //+ "  |  p9: " + pin9State
                    //+ "  |  p10: " + pin10State
                    //+ "  |  p11: " + pin11State
                    + "  |";
    loopN++;
    // Print out the state of the button:
    Serial.println(output);

  delay(1);        // delay in between reads for stability
}

void outputDebugValues()
{
  
}
///////////////////////////////////// COPIED CODE ////////////////////////////
void readPot(){/* function readPot */
////Read Potentiometer value

Serial.print("Raw val : ");Serial.println(throttleUpDownState);
Serial.print("Phys val : ");Serial.println(potRawToPhys(throttleUpDownState));
delay(200);
                        }

float potRawToPhys(int raw){/* function potRawToPhys */
////Potentiometer conversion rule
float Vout = float(raw) * (5/float(1024));//Conversion analog to voltage
float phys = ( 1/*R*/ * (5 - Vout))/5;//Conversion voltage to resistance

 return phys;
}
///////////////////////////////////// COPIED CODE ////////////////////////////

String displaySpacing(int valueToSpaceFor, int maxDigits, bool canBeNegative)
{
  String retval;

  // Automatically hand out enough spaces based on possible digit sizes.
  for(int i = 1; i < maxDigits; i++)
  {
    if (abs(valueToSpaceFor) < pow(10, i))
    {
      retval += " ";
    }
  }

  // Handle an extra space when negatives are involved.
  if (canBeNegative && valueToSpaceFor >= 0)
  {
    retval += " ";
  }
  return retval+valueToSpaceFor;
}
