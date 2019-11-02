
//Wiichuck Mouse
//Brian Krukoski
//April 2012
//red-+5, white-gnd, green-sda, yllw-clck

#include <Wire.h>
#include "nunchuck_funcs.h"


// parameters for reading the joystick:
int range = 40;                // output range of X or Y movement
int threshold = range/4;       // resting threshold.  Typically range/10
int center = range/2;          // resting position value


// Button Defines
const int buttonPin1 = 4;         // input pin for pushbutton
int previousButton1State = LOW;   // for checking the state of a pushButton
const int buttonPin2 = 7;         // input pin for pushbutton
int previousButton2State = LOW;   // for checking the state of a pushButton
const int buttonPin3 = 9;         // input pin for pushbutton
int previousButton3State = LOW;   // for checking the state of a pushButton

// LED Defines
const int ledPin1 = 8;
const int ledPin2 = 10;


void setup() {
  
  // initilization for the Wiichuck
  nunchuck_init();
  
  // Setup the pushButton pins as inputs
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  
  // Setup the LED pins as outputs
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  
  // initialize control over the keyboard:
  Keyboard.begin();
  
  Serial.begin(9600);
}


const int MODE_OFF     = 0;
const int MODE_LANDING = 1;
const int MODE_COMBAT  = 2;
const int MODE_INVALID = 3;
int systemMode   = MODE_OFF;

void loop() {
  
  // If the button 3 state has changed turn the system on/off
  int button3State = digitalRead(buttonPin3);
  
  if ((button3State != previousButton3State) && (button3State == HIGH)) {    
    systemMode++;  if (systemMode == MODE_INVALID) { systemMode = MODE_OFF; }
  } 
  previousButton3State = button3State;
  
  if (systemMode == MODE_LANDING) {
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, LOW);
    handleControls();
    
  } else if (systemMode == MODE_COMBAT) {
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, HIGH);
    handleControls();
    
  } else {
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
  }
  
  delay(1);
 
}

int loopCnt=0;
void handleControls() {
  
    if( loopCnt > 10 ) { // every 10 msecs get new data
      loopCnt = 0;
      handleChuck();
      handleButtons();
    }
    loopCnt++;    
}



void handleChuck() {

  char zButtonKey    = 's';
  char cButtonKey    = 'w';
  char leftStickKey  = 'e';
  char rightStickKey = 'q';
  char upStickKey    = 'f';
  char downStickKey  = 'r';
  
  switch (systemMode) { 
    case MODE_COMBAT : leftStickKey = 'y'; rightStickKey = 'h'; upStickKey = KEY_TAB; downStickKey = 'x'; break;
    case MODE_LANDING : zButtonKey = zButtonKey; break;
  }  
    nunchuck_get_data();

    // Nunchuck C and Z button press control
    int leftState = nunchuck_cbutton(); 
    if (leftState) Keyboard.press(zButtonKey); else Keyboard.release(zButtonKey);
    
    int rightState = nunchuck_zbutton();
    if (rightState) Keyboard.press(cButtonKey); else Keyboard.release(cButtonKey);

    // Nunchuck X Axis
    int xReading = nunchuck_joyx(); //calls on nunchuck_funcs library for x axis reading
    xReading = map(xReading, 38, 232, 0, range); // 38 and 232 arbitrarily determined through experimentation, maps to 0 and preset range
    int xDistance = xReading - center;
    if (abs(xDistance) < threshold) //if absolute value of xDistance is less than predefined threshold....
      {
        xDistance = 0;
        Keyboard.release(rightStickKey);
        Keyboard.release(leftStickKey);
      }
    else if (xDistance > 0) {
        Keyboard.press(leftStickKey);
      
    } else {
        Keyboard.press(rightStickKey);      
    }

    // Nunchuck Y Axis
    int yReading = nunchuck_joyy(); //calls on nunchuck_funcs library for y axis reading
    yReading = map(yReading, 38, 232, 0, range); // 38 and 232 arbitrarily determined through experimentation, maps to 0 and preset range
    int yDistance = yReading - center;
    if (abs(yDistance) < threshold) //if absolute value of yDistance is less than predefined threshold....
      {
        yDistance = 0;
        Keyboard.release(upStickKey);
        Keyboard.release(downStickKey);
      }
    else if (yDistance > 0) {
        Keyboard.press(upStickKey);
      
    } else {
        Keyboard.press(downStickKey);     
    }
  
    //efnunchuck_print_data(); //prints nunchuck data to serial monitor for debugging purposes
}

boolean targetCycle = false;

void handleButtons() {
  
  // read the pushbutton:
  int button1State = digitalRead(buttonPin1);
  int button2State = digitalRead(buttonPin2);
  
  // if the button 1 state has changed,
  if ((button1State != previousButton1State) && (button1State == HIGH)) { 
    if (systemMode == MODE_LANDING) {   
      Keyboard.press('1');delay(100);Keyboard.release('1');delay(100);
      Keyboard.press('e');delay(100);Keyboard.release('e');delay(100);
      Keyboard.press('e');delay(100);Keyboard.release('e');delay(100);
      Keyboard.press(' ');delay(100);Keyboard.release(' ');delay(100);
      Keyboard.press('s');delay(100);Keyboard.release('s');delay(100);
      Keyboard.press(' ');delay(100);Keyboard.release(' ');delay(100);
      Keyboard.press('q');delay(100);Keyboard.release('q');delay(100);
      Keyboard.press('q');delay(100);Keyboard.release('q');delay(100);
      Keyboard.press('1');delay(100);Keyboard.release('1');delay(100);
      Keyboard.press(KEY_INSERT);delay(100);Keyboard.release(KEY_INSERT);
      
    } else if ( (systemMode == MODE_COMBAT) && (targetCycle == true) ) {

      Keyboard.press('q');delay(100);Keyboard.release('q');delay(100);
      Keyboard.press('q');delay(100);Keyboard.release('q');delay(100);
      Keyboard.press('1');delay(100);Keyboard.release('1');delay(100);
     
     targetCycle = false;
      
    } else if (systemMode == MODE_COMBAT) {
      Keyboard.press('1');delay(100);Keyboard.release('1');delay(100);
      Keyboard.press('e');delay(100);Keyboard.release('e');delay(100);
      Keyboard.press('e');delay(100);Keyboard.release('e');delay(100);
      Keyboard.press('s');delay(100);Keyboard.release('s');delay(100);
      targetCycle = true;
  
    }
  }
  
  if (systemMode == MODE_COMBAT && targetCycle) { Keyboard.press('s');delay(100);Keyboard.release('s');delay(200); }
  
  // save the current button state for comparison next time:
  previousButton1State = button1State;
  
  // if the button 2 state has changed,
  if ((button2State != previousButton2State) && (button2State == HIGH)) {    
    Keyboard.press(KEY_RETURN);delay(100);Keyboard.release(KEY_RETURN);
  }
  // save the current button state for comparison next time:
  previousButton2State = button2State;
  
}
