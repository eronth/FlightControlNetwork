
/* Using MHeironimus' ArduinoJoystickLibrary
https://github.com/MHeironimus/ArduinoJoystickLibrary */
#include "Joystick.h"
#include <math.h>

// Create Joystick
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, 
  JOYSTICK_TYPE_MULTI_AXIS, 0, 0,
  false, false, false, false, false, false,
  true, true, false, false, false);

// Digital input pin setup:
const int inPin2 = 2;
const int inPin3 = 3;
const int inPin4 = 4;
const int inPin6 = 6;
const char initialPos = 0;

// Rudder related vars.
char RudderPos = initialPos;
int p2State;
int p2LastState;

// Loop check for debugging. Only iterates loops on changes.
int loopN = 1;


// The setup routine runs once when you first add power or press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
  // Set up pin inputs and outputs.
  pinMode(inPin2, INPUT);
  pinMode(inPin3, INPUT);
  pinMode(inPin4, INPUT);

  // Prepare Joystick and Joystick range values.
  //Joystick.setXAxisRange(-127, 127);
  //Joystick.setYAxisRange(-127, 127);
  //Joystick.setZAxisRange(-127, 127);
  //Joystick.setRxAxisRange(0, 360);
  //Joystick.setRyAxisRange(360, 0);
  //Joystick.setRzAxisRange(0, 720);
  //Joystick.setThrottleRange(0, 255);
  Joystick.setThrottleRange(-127, 127);
  //Joystick.setRudderRange(255, 0);
  Joystick.setRudderRange(-127, 127);
  Joystick.begin();

  // Initialize p2LastState with a starter value.
  p2LastState = digitalRead(inPin2);

  // Information output.
  Serial.println("End of Initialize.");
}

// The loop routine runs over and over again forever:
void loop() {
  // Read the input pins:
  int pin2State = digitalRead(inPin2);
  int pin3State = digitalRead(inPin3);
  int pin4State = digitalRead(inPin4);

  p2State = digitalRead(inPin2);
  if (pin4State == 0)
  {
    RudderPos = initialPos;
    loopN = 0;
    Joystick.setRudder(initialPos);
  }
  if (p2State != p2LastState) {
    if (digitalRead(inPin3) != p2State) {
      RudderPos++;
      Joystick.setRudder(RudderPos);
      //Joystick.setThrottle(RudderPos);
    } else {
      RudderPos--;
      Joystick.setRudder(RudderPos);
      //Joystick.setThrottle(RudderPos);
    }
    
    // String output;
    String output = String("|") 
                    + " Position:" + displaySpacing((int)RudderPos, 3, true)
                    + "  |  Set number:" + displaySpacing((int)loopN, 5, false)
                    + "  |  (" + pin2State + ", " + pin3State + ")"
                    + "  |  p4: " + pin4State
                    + "  |";
    loopN++;
    // Print out the state of the button:
    Serial.println(output);

    p2LastState = p2State;
  }

  delay(1);        // delay in between reads for stability
}

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
