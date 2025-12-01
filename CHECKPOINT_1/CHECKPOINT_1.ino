/*
  LED MATRIX PROJECT
  CHECKPOINT #1
  Drawing game using Joystick and LCD menu display
*/


#include <LiquidCrystal.h>
#include "LedControl.h"

// Matrix pins
const byte pinDin = 12;
const byte pinClock = 11;
const byte pinLoad = 10;

// LCD pins
const byte pinLcdRs = 9;
const byte pinLcdEnable = 8;
const byte pinLcdD4 = 7;
const byte pinLcdD5 = 6;
const byte pinLcdD6 = 5;
const byte pinLcdD7 = 4;
const byte pinLcdContrast = 3;

// Joystick pins
const byte pinJoystickVrx = A0;
const byte pinJoystickVry = A1;
const byte pinJoystickSelect = A2;

// Buzzer
const byte pinBuzzer = 2;

// LCD constants
const byte lcdContrastNormal = 100;
const byte lcdContrastDim = 10;
const byte lcdCols = 16;
const byte lcdRows = 2;

// Matrix constants
const byte matrixSize = 8;
const byte matrixIntensity = 8;

// Timing constants
const unsigned int blinkIntervalSlow = 500;
const unsigned int blinkIntervalFast = 200;
const unsigned int cursorBlinkInterval = 500;
const unsigned int menuDebounce = 200;
const unsigned int buttonDebounce = 300;
const unsigned int moveDebounce = 150;
const unsigned int longPressDuration = 3000;
const unsigned int splashDuration = 1000;
const unsigned int messageDuration = 1500;
const unsigned int aboutDuration = 2000;
const unsigned int exitDelay = 500;
const unsigned int loopDelay = 50;
const unsigned int startupDelay = 200;

// Joystick thresholds
const int joystickThresholdLow = 300;
const int joystickThresholdHigh = 700;
const int joystickCenter = 512;

// Buzzer tones
const unsigned int toneMenuNavigate = 1000;
const unsigned int toneMenuSelect = 1500;
const unsigned int toneMove = 800;
const unsigned int toneClear = 500;
const unsigned int toneExit = 800;
const unsigned int toneDurationShort = 50;
const unsigned int toneDurationMedium = 100;
const unsigned int toneDurationLong = 200;
const unsigned int toneDurationClear = 300;

// Menu constants
const int menuSize = 3;
const int menuStartGame = 0;
const int menuClearDraw = 1;
const int menuAbout = 2;

// LED states
const byte ledOff = 0;
const byte ledOnFixed = 1;
const byte ledOnBlinkSlow = 2;
const byte ledOnBlinkFast = 3;
const byte ledStateCount = 4;

// Initial cursor position
const byte initialCursorX = 3;
const byte initialCursorY = 3;

// Initialize components
LedControl lc = LedControl(pinDin, pinClock, pinLoad, 1);
LiquidCrystal lcd(pinLcdRs, pinLcdEnable, pinLcdD4, pinLcdD5, pinLcdD6, pinLcdD7);

// Game state
bool inGame = false;

// Menu
int selectedMenu = 0;

// Drawing matrix - stores LED state (off, fixed, blink slow, blink fast)
byte ledState[matrixSize][matrixSize];
int cursorX = initialCursorX;
int cursorY = initialCursorY;

// Timing
unsigned long lastBlinkSlow = 0;
unsigned long lastBlinkFast = 0;
unsigned long lastCursorBlink = 0;
bool blinkStateSlow = false;
bool blinkStateFast = false;
bool cursorBlinkState = false;

// Joystick previous state
int lastVry = joystickCenter;

// Non-blocking delays
unsigned long splashStartTime = 0;
unsigned long messageStartTime = 0;
bool showingSplash = false;
bool showingMessage = false;
String messageText = "";

// Initializes hardware and displays splash screen
void setup() {
  pinMode(pinLcdContrast, OUTPUT);
  analogWrite(pinLcdContrast, lcdContrastNormal);
  
  pinMode(pinJoystickSelect, INPUT_PULLUP);
  pinMode(pinBuzzer, OUTPUT);
  
  lcd.begin(lcdCols, lcdRows);
  lcd.clear();
  lcd.print("Matrix Draw");
  lcd.setCursor(0, 1);
  lcd.print("Loading...");
  
  splashStartTime = millis();
  showingSplash = true;
  
  lc.shutdown(0, false);
  lc.setIntensity(0, matrixIntensity);
  lc.clearDisplay(0);
  
  // Initialize LED states to off
  for(int i = 0; i < matrixSize; i++) {
    for(int j = 0; j < matrixSize; j++) {
      ledState[i][j] = ledOff;
    }
  }
  
  lastVry = analogRead(pinJoystickVry);
}

// Main loop managing splash screen and game states
void loop() {
  unsigned long currentTime = millis();
  
  // Handle splash screen
  if(showingSplash) {
    if(currentTime - splashStartTime >= splashDuration) {
      showingSplash = false;
      showMenu();
    }
    return;
  }
  
  // Handle message display
  if(showingMessage) {
    if(currentTime - messageStartTime >= messageDuration) {
      showingMessage = false;
      showMenu();
    }
    return;
  }
  
  // Update slow blink state for LEDs
  if(currentTime - lastBlinkSlow >= blinkIntervalSlow) {
    blinkStateSlow = !blinkStateSlow;
    lastBlinkSlow = currentTime;
  }
  
  // Update fast blink state for LEDs
  if(currentTime - lastBlinkFast >= blinkIntervalFast) {
    blinkStateFast = !blinkStateFast;
    lastBlinkFast = currentTime;
  }
  
  // Update cursor blink for menu selection
  if(currentTime - lastCursorBlink >= cursorBlinkInterval) {
    cursorBlinkState = !cursorBlinkState;
    lastCursorBlink = currentTime;
    
    if(!inGame) {
      showMenu();
    }
  }
  
  // Route to appropriate state handler
  if(inGame) {
    gameLoop();
  } else {
    menuLoop();
  }
}

// Displays menu with current selection
void showMenu() {
  analogWrite(pinLcdContrast, lcdContrastNormal);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("=== MENU ===");
  
  lcd.setCursor(0, 1);
  
  // Display menu item with blinking cursor
  if(selectedMenu == menuStartGame) {
    if(cursorBlinkState) lcd.print(">"); else lcd.print(" ");
    lcd.print("Start Game");
  } else if(selectedMenu == menuClearDraw) {
    if(cursorBlinkState) lcd.print(">"); else lcd.print(" ");
    lcd.print("Clear Draw");
  } else if(selectedMenu == menuAbout) {
    if(cursorBlinkState) lcd.print(">"); else lcd.print(" ");
    lcd.print("About");
  }
  
  displayDrawing();
}

// Handles menu navigation and selection
void menuLoop() {
  analogWrite(pinLcdContrast, lcdContrastNormal);
  
  static unsigned long lastAction = 0;
  unsigned long currentTime = millis();
  
  // Debounce menu actions
  if(currentTime - lastAction < menuDebounce) {
    return;
  }
  
  int vry = analogRead(pinJoystickVry);
  bool btn = (digitalRead(pinJoystickSelect) == LOW);
  
  // Navigate up in menu
  if(vry < joystickThresholdLow && lastVry >= joystickThresholdLow) {
    selectedMenu--;
    if(selectedMenu < 0) selectedMenu = menuSize - 1;
    
    showMenu();
    tone(pinBuzzer, toneMenuNavigate, toneDurationMedium);
    lastAction = currentTime;
  }
  // Navigate down in menu
  else if(vry > joystickThresholdHigh && lastVry <= joystickThresholdHigh) {
    selectedMenu++;
    if(selectedMenu >= menuSize) selectedMenu = 0;
    
    showMenu();
    tone(pinBuzzer, toneMenuNavigate, toneDurationMedium);
    lastAction = currentTime;
  }
  
  lastVry = vry;
  
  // Execute menu selection
  if(btn && currentTime - lastAction > buttonDebounce) {
    tone(pinBuzzer, toneMenuSelect, toneDurationMedium);
    
    if(selectedMenu == menuStartGame) {
      startGame();
    } else if(selectedMenu == menuClearDraw) {
      clearAll();
    } else if(selectedMenu == menuAbout) {
      showAbout();
    }
    
    lastAction = currentTime;
  }
}

// Enters draw mode and initializes cursor
void startGame() {
  analogWrite(pinLcdContrast, lcdContrastNormal);
  
  inGame = true;
  cursorX = initialCursorX;
  cursorY = initialCursorY;
  
  lcd.clear();
  lcd.print("DRAW MODE");
  updateDisplay();
}

// Handles cursor movement and LED state toggling
void gameLoop() {
  analogWrite(pinLcdContrast, lcdContrastNormal);
  
  static unsigned long lastMove = 0;
  static unsigned long btnPressStartTime = 0;
  static bool btnWasPressed = false;
  
  unsigned long currentTime = millis();
  
  int vrx = analogRead(pinJoystickVrx);
  int vry = analogRead(pinJoystickVry);
  bool btn = (digitalRead(pinJoystickSelect) == LOW);
  
  // Handle cursor movement with debouncing
  if(currentTime - lastMove > moveDebounce) {
    bool moved = false;
    
    // Move cursor horizontally
    if(vrx < joystickThresholdLow && cursorX > 0) {
      cursorX--;
      moved = true;
    }
    else if(vrx > joystickThresholdHigh && cursorX < matrixSize - 1) {
      cursorX++;
      moved = true;
    }
    
    // Move cursor vertically
    if(vry < joystickThresholdLow && cursorY > 0) {
      cursorY--;
      moved = true;
    }
    else if(vry > joystickThresholdHigh && cursorY < matrixSize - 1) {
      cursorY++;
      moved = true;
    }
    
    if(moved) {
      tone(pinBuzzer, toneMove, toneDurationShort);
      updateDisplay();
      lastMove = currentTime;
    }
  }
  
  // Track button press start time
  if(btn && !btnWasPressed) {
    btnPressStartTime = currentTime;
    btnWasPressed = true;
  }
  
  // Check for long press to exit game
  if(btn && btnWasPressed) {
    if(currentTime - btnPressStartTime >= longPressDuration) {
      inGame = false;
      btnWasPressed = false;
      
      tone(pinBuzzer, toneExit, toneDurationLong);
      
      lastVry = analogRead(pinJoystickVry);
      
      static unsigned long exitStartTime = 0;
      static bool waitingToExit = true;
      exitStartTime = currentTime;
      
      // Wait before returning to menu
      while(millis() - exitStartTime < exitDelay) {
        // Non-blocking wait
      }
      
      showMenu();
      return;
    }
  }
  
  // Handle short press to cycle LED state
  if(!btn && btnWasPressed) {
    unsigned long pressDuration = currentTime - btnPressStartTime;
    
    if(pressDuration < longPressDuration) {
      ledState[cursorY][cursorX] = (ledState[cursorY][cursorX] + 1) % ledStateCount;
      tone(pinBuzzer, toneMenuSelect, toneDurationMedium);
      updateDisplay();
    }
    
    btnWasPressed = false;
  }
  
  updateMatrix();
}

// Updates matrix display with current LED states
void updateMatrix() {
  lc.clearDisplay(0);
  
  // Render each LED based on its state
  for(int y = 0; y < matrixSize; y++) {
    for(int x = 0; x < matrixSize; x++) {
      bool shouldLight = false;
      
      switch(ledState[y][x]) {
        case ledOnFixed:
          shouldLight = true;
          break;
        case ledOnBlinkSlow:
          shouldLight = blinkStateSlow;
          break;
        case ledOnBlinkFast:
          shouldLight = blinkStateFast;
          break;
        case ledOff:
        default:
          shouldLight = false;
          break;
      }
      
      if(shouldLight) {
        lc.setLed(0, y, x, true);
      }
    }
  }
  
  // Show cursor in draw mode
  if(inGame && cursorBlinkState) {
    lc.setLed(0, cursorY, cursorX, true);
  }
}

// Displays drawing on matrix in menu mode
void displayDrawing() {
  lc.clearDisplay(0);
  
  for(int y = 0; y < matrixSize; y++) {
    for(int x = 0; x < matrixSize; x++) {
      bool shouldLight = false;
      
      switch(ledState[y][x]) {
        case ledOnFixed:
          shouldLight = true;
          break;
        case ledOnBlinkSlow:
          shouldLight = blinkStateSlow;
          break;
        case ledOnBlinkFast:
          shouldLight = blinkStateFast;
          break;
        case ledOff:
        default:
          shouldLight = false;
          break;
      }
      
      if(shouldLight) {
        lc.setLed(0, y, x, true);
      }
    }
  }
}

// Updates LCD with cursor position and LED state
void updateDisplay() {
  analogWrite(pinLcdContrast, lcdContrastNormal);
  
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("X:");
  lcd.print(cursorX);
  lcd.print(" Y:");
  lcd.print(cursorY);
  lcd.print(" ");
  
  // Display current LED state at cursor
  switch(ledState[cursorY][cursorX]) {
    case ledOff:
      lcd.print("OFF");
      break;
    case ledOnFixed:
      lcd.print("ON");
      break;
    case ledOnBlinkSlow:
      lcd.print("SLOW");
      break;
    case ledOnBlinkFast:
      lcd.print("FAST");
      break;
  }
}

// Clears all LEDs from drawing
void clearAll() {
  analogWrite(pinLcdContrast, lcdContrastDim);
  
  for(int i = 0; i < matrixSize; i++) {
    for(int j = 0; j < matrixSize; j++) {
      ledState[i][j] = ledOff;
    }
  }
  lc.clearDisplay(0);
  
  lcd.clear();
  lcd.print("Drawing cleared!");
  tone(pinBuzzer, toneClear, toneDurationClear);
  
  messageStartTime = millis();
  showingMessage = true;
}

// Displays about information
void showAbout() {
  analogWrite(pinLcdContrast, lcdContrastNormal);
  
  lcd.clear();
  lcd.print("Matrix Draw v2");
  lcd.setCursor(0, 1);
  lcd.print("Lab Project 2024");
  
  messageStartTime = millis();
  showingMessage = true;
  messageText = "about";
}