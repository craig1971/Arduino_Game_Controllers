

#include <Keyboard.h>

// define the pins for the buttons.
byte buttonPins[] = {8, 9, 10}; 
char keys[]       = {'KEY_LEFT_CTRL', 'w', ' '}; 

// define which button will activate/deactivate the system
int SYSTEM_BUTTON = 2;

//  determine the size of the array above
#define NUMBUTTONS sizeof(buttonPins)


boolean buttonState[NUMBUTTONS];
boolean buttonStatePrevious[NUMBUTTONS];


boolean systemState = false;
boolean previousSystemState = -1;


void setup() {
  
  Serial.begin(9600);
  
  // Setup the pushButton pins as inputs
  for (int i=0; i<NUMBUTTONS; i++)  {
    pinMode(buttonPins[i], INPUT_PULLUP);
    buttonStatePrevious[i] = -1;
  }
  
  
}

void loop() {

  checkButtons();

  if ( isSystemActive(buttonState[SYSTEM_BUTTON]) ) {
    handleButtons();  
  }

  delay(500);
  
}


void checkButtons() {
  
  for (int i=0; i<NUMBUTTONS; i++)  {
    buttonState[i] = digitalRead(buttonPins[i]) == LOW;
  }
  
}


void handleButtons(){

  boolean changeOccured = false;
  for (int i=0; i<NUMBUTTONS; i++)  {
            
    if (i != SYSTEM_BUTTON) {    
      changeOccured = (buttonState[i] != buttonStatePrevious[i] ) || changeOccured;
    }
    
  }
  
  if (changeOccured) {
    
    for (int i=0; i<NUMBUTTONS; i++)  {  
            
      if (i != SYSTEM_BUTTON) {    
      
        if (i > 0) { Serial.print(" - "); }
        Serial.print(buttonState[i]);
  
        char keyState = ' ';
        if (buttonState[i])  { Keyboard.press(keys[i]); delay(100);   keyState = 'D';}
        else                 { Keyboard.release(keys[i]);  keyState = 'U';}
        buttonStatePrevious[i] = buttonState[i];     
  
        Serial.print(" - ");
        Serial.print(keyState);
      }  
    }  
    
    Serial.println();
  }    
  
}




boolean isSystemActive(boolean systemButtonState) {
  
  systemState = systemButtonState;
  
  if (systemState != previousSystemState) {
    
      if (systemState) {
          Serial.println("Starting Keyboard");
          
          // give control over the keyboard:
          Keyboard.begin();
          
      } else {
          Serial.println("Stopping Keyboard");
          
          Keyboard.releaseAll();
          Keyboard.end();
      }
  }
  previousSystemState = systemState;
  
  return systemState;

}

