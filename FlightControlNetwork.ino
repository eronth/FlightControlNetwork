
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
                   JOYSTICK_TYPE_MULTI_AXIS, 7, 0,
                   false, false, false, false, false, false,
                   true, true, false, false, false);


const int numberOfThrottleButtons = 4;

//------------------------------------------------------------------------------------
//                                    Set pin vars
//------------------------------------------------------------------------------------
// Rudder
const int rudderEncoder1Pin = 0; // Rudder
const int rudderEncoder2Pin = 1; // Rudder
const int rudderButtonPin = 2;

// Throttle
const int throttleToggleFPin = 3;  // Throttle Toggle
const int throttleToggleBPin = 4;  // Throttle Toggle
const int throttleUpDownPin  = A0; // Throttle
const int throttleButtonPins[] = {8, 9, 10, 11}; // Throttle Button

// Placeholders
const int inPin5 = 5;
const int inPin6 = 6;
const int inPin7 = 7;
const int inPin12 = 12;
const int inPin13 = 13;
const int inPinA1 = A1;
const int inPinA2 = A2;
const int inPinA3 = A3;
const int inPinA4 = A4;
const int inPinA5 = A5;

//------------------------------------------------------------------------------------
//                                    Create various state and last-state vars
//------------------------------------------------------------------------------------
// Rudder States
int rudderEncoder1State = 0;
int rudderEncoder1LastState = 0;
int rudderEncoder2State = 0;
int rudderButtonState = 0;
int rudderButtonLastState = 0;

// Throttle States
int throttleUpDownState = 0;
int throttleButtonStates[] = {0, 0, 0, 0};
int throttleButtonLastStates[] = {0, 0, 0, 0};

int throttleToggleFState = 0;
int throttleToggleBState = 0;
int throttleToggleCombinedState = 0;
int throttleToggleLastFullState = 0;

// Placeholder
int pin5State;
int pin6State;
int pin7State;
int pin12State;
int pin13State;
int pinA1State;
int pinA2State;
int pinA3State;
int pinA4State;
int pinA5State;

//------------------------------------------------------------------------------------
//                                    Create position trackers
//------------------------------------------------------------------------------------
const char initialPos = 0;

// Rudder positioning
char rudderPosition = initialPos;
const int rudderRangeMin = -127;
const int rudderRangeMax = 127;

// Throttle Positioning
int throttlePosition = initialPos;
const int throttleRangeMin = -127;
const int throttleRangeMax = 127;


//------------------------------------------------------------------------------------
//                                    Map physical button to a JoyStick's Buttons
//------------------------------------------------------------------------------------
// Rudder Buttons
// TODO map rudder?

// Throttle Buttons
const int throttleButton_JSBs[] = {0, 1, 2, 3};
const int throttleToggleF_JSB = 4;
const int throttleToggleN_JSB = 5;
const int throttleToggleB_JSB = 6;

// These are used to create a bit of a delay before releasing the button so games can register it.
int throttleToggleDelayCount = -1;
const int toggleInputMilSecDelay = 300;





// Loop check for debugging. Only iterates loops on changes.
int loopN = 1;
bool pressed = false;

//////////////////////////////////////////////////////////////////////////////////////
//                                    SETUP
//////////////////////////////////////////////////////////////////////////////////////
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
  for (int i = 0; i < numberOfThrottleButtons; i++) {
    pinMode(throttleButtonPins[i], INPUT_PULLUP);
  }
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

//////////////////////////////////////////////////////////////////////////////////////
//                                    REGULAR LOOP
//////////////////////////////////////////////////////////////////////////////////////
// The loop routine runs over and over again forever:
void loop() {
  //------------------------------------------------------------------------------------
  //                                    Pull values from Pins
  //------------------------------------------------------------------------------------
  // Rudder Items
  rudderEncoder1State = digitalRead(rudderEncoder1Pin);   // Rudder
  rudderEncoder2State = digitalRead(rudderEncoder2Pin);   // Rudder
  rudderButtonState = digitalRead(rudderButtonPin);       // Rudder
  
  // Throttle Items
  throttleToggleFState = digitalRead(throttleToggleFPin);      // Throttle
  throttleToggleBState = digitalRead(throttleToggleBPin);      // Throttle
  throttleUpDownState = analogRead(throttleUpDownPin);    // Throttle
  for (int i = 0; i < numberOfThrottleButtons; i++) {
    throttleButtonStates[i] = digitalRead(throttleButtonPins[i]); // Throttle
  }

  // Placeholder Items
  pin5State = digitalRead(inPin5); // Throttle
  pin6State = digitalRead(inPin6); // Throttle
  pin7State = digitalRead(inPin7); // Throttle

  //readPot();

  //------------------------------------------------------------------------------------
  //                                    Throttle Inputs
  //------------------------------------------------------------------------------------
  // Throttle Button Presses
  for (int i = 0; i < numberOfThrottleButtons; i++) {
    if (throttleButtonStates[i] != throttleButtonLastStates[i]) {
      Joystick.setButton(throttleButton_JSBs[i], throttleButtonStates[i]);
      String prState = String("") + (throttleButtonStates[i] == 1 ? " Pressed" : " Released");
      Serial.println( String("|") + "Button " + i + prState);

      throttleButtonLastStates[i] = throttleButtonStates[i];
      pressed = !pressed;
    }
  }

  // Throttle Toggle Inputs
  if (throttleToggleFState == 1) {
    throttleToggleCombinedState = 1;
  } else if (throttleToggleBState == 1) {
    throttleToggleCombinedState = -1;
  } else {
    throttleToggleCombinedState = 0;
  }
  // If the state has changed, we need to toggle a quick button press.
  if (throttleToggleLastFullState != throttleToggleCombinedState) {
    if (throttleToggleCombinedState == -1) {
      Joystick.setButton(throttleToggleB_JSB, 1);
      Serial.println("Toggle Back");
    } else if (throttleToggleCombinedState == 0) {
      Joystick.setButton(throttleToggleN_JSB, 1);
      Serial.println("Toggle Middle");
    } else if (throttleToggleCombinedState == 1) {
      Joystick.setButton(throttleToggleF_JSB, 1);
      Serial.println("Toggle Front");
    }
    throttleToggleLastFullState = throttleToggleCombinedState;
    throttleToggleDelayCount = toggleInputMilSecDelay;
  } else {
    if (throttleToggleDelayCount == 0) {
      Joystick.setButton(throttleToggleF_JSB, 0);
      Joystick.setButton(throttleToggleN_JSB, 0);
      Joystick.setButton(throttleToggleB_JSB, 0);
      Serial.println("Toggle release");
      throttleToggleDelayCount = -1;
    } else if (throttleToggleDelayCount != -1) {
      throttleToggleDelayCount--;
    }

  }

  // Throttle up/down input.
  // The max and min seems to be... 919, 112, and I don't know why!
  int throttledUp = 112;
  int throttledDown = 919;
  if (throttleToggleFState == 1 || throttleToggleBState == 1) {
    // for some reason, 924 and 153 if shields up
    throttledUp = 153;
    throttledDown = 924;
  }
  throttlePosition = map(throttleUpDownState, throttledDown, throttledUp, throttleRangeMin, throttleRangeMax);
  Joystick.setThrottle(throttlePosition);

  //------------------------------------------------------------------------------------
  //                                    Rudder inputs
  //------------------------------------------------------------------------------------
  if (rudderEncoder1State != rudderEncoder1LastState) {
    if (rudderEncoder2State != rudderEncoder1State) {
      rudderPosition++;
      Joystick.setRudder(rudderPosition);
    } else {
      rudderPosition--;
      Joystick.setRudder(rudderPosition);
    }
    Serial.println("Rudder position at " + rudderPosition);
    rudderEncoder1LastState = rudderEncoder1State;
  }

  // Rudder button (purposeless)
  if (rudderButtonState != rudderButtonLastState) {
      //Joystick.setButton(throttleButton_JSBs[i], );
      String prState = String("") + (rudderButtonState == 1 ? " Pressed" : " Released");
      Serial.println( String("|") + "Rudder button" + prState);

      rudderButtonLastState = rudderButtonState;
      pressed = !pressed;
  }

  //------------------------------------------------------------------------------------
  //                                    Output/Debug section
  //------------------------------------------------------------------------------------
  //outputDebugValues();
  loopN++;

  // delay in between reads for stability
  delay(1);
}

void outputDebugValues()
{
  String output = String("|")
                  + " Position:" + displaySpacing((int)rudderPosition, 3, true)
                  + "  |  Set number:" + displaySpacing((int)loopN, 5, false)
                  + "  |  (" + rudderEncoder1State + ", " + rudderEncoder2State + ")"
                  + "  | Rudder Click: " + rudderButtonState
                  + "  |  Throttle Toggle: " + displaySpacing((int)throttleToggleCombinedState, 1, true) + " from " + throttleToggleFState + " " + throttleToggleBState
                  + "  |  Throttled to " + throttleUpDownState + " " + throttlePosition
                  + "  |  Buttons: " + pressed + throttleButtonStates[0] + " " + throttleButtonStates[1] + " " + throttleButtonStates[2] + " " + throttleButtonStates[3]
                  //+ "  |  p6: " + pin6State
                  //+ "  |  p7: " + pin7State
                  + "  |";
    // Print out the state of the button:
    Serial.println(output);
}
///////////////////////////////////// COPIED CODE ////////////////////////////
void readPot() { /* function readPot */
  ////Read Potentiometer value

  Serial.print("Raw val : "); Serial.println(throttleUpDownState);
  Serial.print("Phys val : "); Serial.println(potRawToPhys(throttleUpDownState));
  delay(200);
}

float potRawToPhys(int raw) { /* function potRawToPhys */
  ////Potentiometer conversion rule
  float Vout = float(raw) * (5 / float(1024)); //Conversion analog to voltage
  float phys = ( 1/*R*/ * (5 - Vout)) / 5; //Conversion voltage to resistance

  return phys;
}
///////////////////////////////////// COPIED CODE ////////////////////////////

// Function to cleanly handle the spacing for debug output.
String displaySpacing(int valueToSpaceFor, int maxDigits, bool canBeNegative)
{
  String retval;

  // Automatically hand out enough spaces based on possible digit sizes.
  for (int i = 1; i < maxDigits; i++)
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
  return retval + valueToSpaceFor;
}
