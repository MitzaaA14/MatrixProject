#include <LedControl.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// pin configuration
const int pinDin = 12;
const int pinClk = 11;
const int pinCs = 10;

const int pinJoy1X = A0;
const int pinJoy1Y = A1;
const int pinJoy1Btn = 2;

const int pinJoy2X = A2;
const int pinJoy2Y = A3;
const int pinJoy2Btn = 3;

const int pinBtnPause = 7;
const int pinBuzzer = 6;

// musical note frequencies
const int noteF = 349;
const int noteGs = 415;
const int noteA = 440;
const int noteAs = 455;
const int noteB = 466;
const int noteCh = 523;
const int noteCsh = 554;
const int noteDh = 587;
const int noteDsh = 622;
const int noteEh = 659;
const int noteFh = 698;
const int noteFsh = 740;
const int noteGh = 784;
const int noteGsh = 830;
const int noteAh = 880;

// timing constants
const unsigned long joyInitialDelay = 260;
const unsigned long joyRepeatDelay = 120;
const unsigned long longPressDuration = 750;
const int joyLowThreshold = 300;
const int joyHighThreshold = 700;
const unsigned long blinkInterval = 300;

// map dimensions
const int mapWidth = 8;
const int mapHeight = 16;
const int matrixHalf = 8;

// display settings
const int matrixCount = 4;
const int matrixIntensity = 8;
const int lcdI2cAddress = 0x27;
const int lcdWidth = 16;
const int lcdHeight = 2;

// fleet configuration
const int fleetSize = 5;
const int shipCarrierLength = 5;
const int shipBattleLength = 4;
const int shipCruiserLength = 3;
const int shipSubLength = 3;
const int shipDestLength = 2;

// buzzer tone settings
const int beepHighFreq = 2000;
const int beepHighDuration = 100;
const int errorFreq = 300;
const int errorDuration = 200;
const int successFreq1 = 1500;
const int successFreq2 = 2000;
const int successDuration = 100;

// menu configuration
const int menuItemsMain = 2;
const int menuItemsPause = 3;

// lcd positions
const int lcdColStart = 0;
const int lcdRowFirst = 0;
const int lcdRowSecond = 1;
const int lcdColText = 1;

// initial positions
const int initialPos = 0;

// loop delay
const int loopDelay = 20;

// player identifiers
const int playerOne = 1;
const int playerTwo = 2;

// matrix indexes
const int matrixP1Start = 0;
const int matrixP2Start = 2;

// direction values
const int dirNeutral = 0;
const int dirNegative = -1;
const int dirPositive = 1;

// button press states
const int btnNoPress = 0;
const int btnShortPress = 1;
const int btnLongPress = 2;

// constraint values for ship placement
const int maxXHorizontal = 7;
const int maxYVertical = 15;

// game state enumeration
enum GameState {
  MENU_MAIN,
  MENU_PAUSE,
  MENU_ABOUT,
  GAME_INIT,
  GAME_PLACE_SHIPS_P1,
  GAME_PLACE_SHIPS_P2,
  GAME_SHOW_MAPS,
  GAME_PLAY_P1,
  GAME_PLAY_P2
};

// cell state enumeration
enum CellState {
  WATER = 0,
  SHIP = 1,
  HIT = 2,
  MISS = 3
};

// ship class - represents a single ship
class Ship {
  private:
    int length;
    int x, y;
    bool horizontal;
    bool placed;
    const char* name;

  public:
    Ship(const char* n, int len) : name(n), length(len), x(initialPos), y(initialPos), horizontal(true), placed(false) {}

    int getLength() const { return length; }
    int getX() const { return x; }
    int getY() const { return y; }
    bool isHorizontal() const { return horizontal; }
    bool isPlaced() const { return placed; }
    const char* getName() const { return name; }

    void setPosition(int newX, int newY) { x = newX; y = newY; }
    void toggleOrientation() { horizontal = !horizontal; }
    void setPlaced(bool p) { placed = p; }

    // checks if ship fits within map boundaries
    bool fitsOnMap(int maxX, int maxY) const {
      if (horizontal) {
        return (x >= initialPos) && (y >= initialPos) && (x < maxX) && (y + length <= maxY);
      } else {
        return (x >= initialPos) && (y >= initialPos) && (x + length <= maxX) && (y < maxY);
      }
    }
};

// map class - represents game board
class Map {
  private:
    byte data[mapWidth][mapHeight];

  public:
    Map() { clear(); }

    void clear() { memset(data, WATER, sizeof(data)); }

    // gets cell state at coordinates
    byte get(int x, int y) const {
      if (x >= initialPos && x < mapWidth && y >= initialPos && y < mapHeight) {
        return data[x][y];
      }
      return WATER;
    }

    // sets cell state at coordinates
    void set(int x, int y, byte value) {
      if (x >= initialPos && x < mapWidth && y >= initialPos && y < mapHeight) {
        data[x][y] = value;
      }
    }

    // checks if ship can be placed without overlap
    bool canPlaceShip(const Ship& ship) const {
      int sx = ship.getX();
      int sy = ship.getY();
      int len = ship.getLength();

      if (!ship.fitsOnMap(mapWidth, mapHeight)) return false;

      if (ship.isHorizontal()) {
        for (int i = initialPos; i < len; i++) {
          if (data[sx][sy + i] != WATER) return false;
        }
      } else {
        for (int i = initialPos; i < len; i++) {
          if (data[sx + i][sy] != WATER) return false;
        }
      }
      return true;
    }

    // places ship on map
    void placeShip(const Ship& ship) {
      int sx = ship.getX();
      int sy = ship.getY();
      int len = ship.getLength();

      if (ship.isHorizontal()) {
        for (int i = initialPos; i < len; i++) {
          data[sx][sy + i] = SHIP;
        }
      } else {
        for (int i = initialPos; i < len; i++) {
          data[sx + i][sy] = SHIP;
        }
      }
    }
};

// fleet class - manages collection of ships
class Fleet {
  private:
    Ship* ships[fleetSize];
    int currentShipIndex;

  public:
    Fleet() : currentShipIndex(initialPos) {
      ships[0] = new Ship("Carrier", shipCarrierLength);
      ships[1] = new Ship("Battle", shipBattleLength);
      ships[2] = new Ship("Cruiser", shipCruiserLength);
      ships[3] = new Ship("Sub", shipSubLength);
      ships[4] = new Ship("Dest", shipDestLength);
    }

    ~Fleet() {
      for (int i = initialPos; i < fleetSize; i++) delete ships[i];
    }

    // gets current ship being placed
    Ship* getCurrentShip() {
      if (currentShipIndex < fleetSize) return ships[currentShipIndex];
      return nullptr;
    }

    void nextShip() {
      if (currentShipIndex < fleetSize) currentShipIndex++;
    }

    // resets fleet to initial state
    void reset() {
      currentShipIndex = initialPos;
      for (int i = initialPos; i < fleetSize; i++) {
        ships[i]->setPlaced(false);
        ships[i]->setPosition(initialPos, initialPos);
      }
    }

    int getRemainingShips() const { return fleetSize - currentShipIndex; }
};

// display class - manages lcd and led matrices
class Display {
  private:
    LedControl* lc;
    LiquidCrystal_I2C* lcd;

  public:
    Display(LedControl* l, LiquidCrystal_I2C* d) : lc(l), lcd(d) {}

    // initializes all display components
    void init() {
      for (int i = initialPos; i < matrixCount; i++) {
        lc->shutdown(i, false);
        lc->setIntensity(i, matrixIntensity);
        lc->clearDisplay(i);
      }
      lcd->init();
      lcd->backlight();
      lcd->clear();
    }

    void clearMatrices() {
      for (int i = initialPos; i < matrixCount; i++) lc->clearDisplay(i);
    }

    void clearLCD() { lcd->clear(); }

    void printLCD(int col, int row, const char* text) {
      lcd->setCursor(col, row);
      lcd->print(text);
    }

    // draws map on led matrices
    void drawMap(const Map& map, int startMatrix) {
      for (int x = initialPos; x < mapWidth; x++) {
        for (int y = initialPos; y < mapHeight; y++) {
          byte state = map.get(x, y);
          bool shouldLight = (state == SHIP || state == HIT);
          if (y < matrixHalf) lc->setLed(startMatrix, x, y, shouldLight);
          else lc->setLed(startMatrix + dirPositive, x, y - matrixHalf, shouldLight);
        }
      }
    }

    // draws map with blinking ship preview
    void drawMapWithShipPreview(const Map& map, const Ship& ship, int startMatrix) {
      drawMap(map, startMatrix);

      unsigned long t = millis();
      bool showShip = ((t / blinkInterval) % 2) == dirNeutral;

      if (!ship.isPlaced() && showShip) {
        int sx = ship.getX();
        int sy = ship.getY();
        int len = ship.getLength();

        if (ship.isHorizontal()) {
          for (int i = initialPos; i < len; i++) {
            int col = sy + i;
            if (sx >= initialPos && sx < mapWidth && col >= initialPos && col < mapHeight) {
              if (map.get(sx, col) != SHIP) {
                if (col < matrixHalf) lc->setLed(startMatrix, sx, col, true);
                else lc->setLed(startMatrix + dirPositive, sx, col - matrixHalf, true);
              }
            }
          }
        } else {
          for (int i = initialPos; i < len; i++) {
            int row = sx + i;
            if (row >= initialPos && row < mapWidth && sy >= initialPos && sy < mapHeight) {
              if (map.get(row, sy) != SHIP) {
                if (sy < matrixHalf) lc->setLed(startMatrix, row, sy, true);
                else lc->setLed(startMatrix + dirPositive, row, sy - matrixHalf, true);
              }
            }
          }
        }
      }
    }
};

// input class - handles joystick and button input
class Input {
  private:
    unsigned long lastJoyMoveTime;
    int joyLastX, joyLastY;
    unsigned long joyHoldStart;
    bool buttonWasPressed;
    unsigned long buttonPressStart;
    bool pauseWasPressed;

  public:
    Input() : lastJoyMoveTime(initialPos), joyLastX(initialPos), joyLastY(initialPos), 
              joyHoldStart(initialPos), buttonWasPressed(false), buttonPressStart(initialPos), 
              pauseWasPressed(false) {}

    // reads joystickfor menu navigation
    void readJoystickRepeatMenu(int pinX, int pinY, int &dx, int &dy) {
      dx = dirNeutral; 
      dy = dirNeutral;
      int rawX = analogRead(pinX);
      int rawY = analogRead(pinY);

      int sx = dirNeutral;
      int sy = dirNeutral;
      
      if (rawX < joyLowThreshold) sy = dirNegative;
      else if (rawX > joyHighThreshold) sy = dirPositive;
      
      if (rawY < joyLowThreshold) sx = dirNegative;
      else if (rawY > joyHighThreshold) sx = dirPositive;

      unsigned long now = millis();

      if (sx != joyLastX || sy != joyLastY) {
        joyLastX = sx; 
        joyLastY = sy;
        lastJoyMoveTime = now;
        joyHoldStart = now;
        dx = sx; 
        dy = sy;
        return;
      }

      if (sx == dirNeutral && sy == dirNeutral) return;

      unsigned long held = now - joyHoldStart;
      if (held >= joyInitialDelay) {
        if (now - lastJoyMoveTime >= joyRepeatDelay) {
          lastJoyMoveTime = now;
          dx = sx; 
          dy = sy;
        }
      }
    }

    // reads joystick with repeat for game navigation
    void readJoystickRepeatGame(int pinX, int pinY, int &dx, int &dy) {
      dx = dirNeutral; 
      dy = dirNeutral;
      int rawX = analogRead(pinX);
      int rawY = analogRead(pinY);

      int sx = dirNeutral;
      int sy = dirNeutral;
      
      if (rawX < joyLowThreshold) sx = dirNegative;
      else if (rawX > joyHighThreshold) sx = dirPositive;

      if (rawY < joyLowThreshold) sy = dirPositive;
      else if (rawY > joyHighThreshold) sy = dirNegative;

      unsigned long now = millis();

      if (sx != joyLastX || sy != joyLastY) {
        joyLastX = sx; 
        joyLastY = sy;
        lastJoyMoveTime = now;
        joyHoldStart = now;
        dx = sx; 
        dy = sy;
        return;
      }

      if (sx == dirNeutral && sy == dirNeutral) return;

      unsigned long held = now - joyHoldStart;
      if (held >= joyInitialDelay) {
        if (now - lastJoyMoveTime >= joyRepeatDelay) {
          lastJoyMoveTime = now;
          dx = sx; 
          dy = sy;
        }
      }
    }

    // checks button press and returns press type
    int checkButtonPress(int pin) {
      bool current = digitalRead(pin) == LOW;
      unsigned long now = millis();

      if (current && !buttonWasPressed) {
        buttonWasPressed = true;
        buttonPressStart = now;
        return btnNoPress;
      }

      if (!current && buttonWasPressed) {
        unsigned long dt = now - buttonPressStart;
        buttonWasPressed = false;
        if (dt >= longPressDuration) return btnLongPress;
        return btnShortPress;
      }

      return btnNoPress;
    }

    // checks pause button state
    int checkPauseButton() {
      bool current = digitalRead(pinBtnPause) == LOW;

      if (current && !pauseWasPressed) {
        pauseWasPressed = true;
        return btnNoPress;
      }
      if (!current && pauseWasPressed) {
        pauseWasPressed = false;
        return btnShortPress;
      }
      return btnNoPress;
    }

    void resetButtonState() {
      buttonWasPressed = false;
      buttonPressStart = initialPos;
    }
};

// audio class - handles buzzer and music
class Audio {
  private:
    int buzzerPin;

    struct Note {
      int freq;
      unsigned int dur;
    };

    static const Note imperialSeq[];
    static const int imperialLen;

    int idx;
    unsigned long noteStart;
    bool playing;
    int lastFreq;
    
    unsigned long toneStartTime;
    unsigned long toneDuration;
    bool toneActive;
    int toneFrequency;
    bool playingSecondTone;

  public:
    Audio(int pin) : buzzerPin(pin), idx(initialPos), noteStart(initialPos), playing(false), 
                     lastFreq(dirNegative), toneStartTime(0), toneDuration(0), 
                     toneActive(false), toneFrequency(0), playingSecondTone(false) {}

    void begin() { pinMode(buzzerPin, OUTPUT); }

    // starts playing imperial march
    void startImperialMarch() {
      if (playing) return;
      playing = true;
      idx = initialPos;
      noteStart = millis();
      lastFreq = dirNegative;
    }

    void stopImperialMarch() {
      if (!playing) return;
      playing = false;
      noTone(buzzerPin);
      lastFreq = dirNegative;
    }

    bool isPlaying() const { return playing; }

    // updates music playback and non-blocking tones
    void update() {
      unsigned long now = millis();
      
      if (toneActive) {
        if (now - toneStartTime >= toneDuration) {
          if (!playingSecondTone && toneFrequency != 0) {
            playingSecondTone = true;
            tone(buzzerPin, toneFrequency);
            toneStartTime = now;
            toneDuration = successDuration;
            return;
          }
          
          noTone(buzzerPin);
          toneActive = false;
          playingSecondTone = false;
          toneFrequency = 0;
          
          if (playing) {
            const Note &next = imperialSeq[idx];
            if (next.freq > initialPos) tone(buzzerPin, next.freq);
            else noTone(buzzerPin);
            lastFreq = next.freq;
          }
        }
        return;
      }
      
      // handle imperial march
      if (!playing) return;
      
      const Note &n = imperialSeq[idx];

      if (lastFreq == dirNegative) {
        if (n.freq > initialPos) tone(buzzerPin, n.freq);
        else noTone(buzzerPin);
        lastFreq = n.freq;
        noteStart = now;
      }

      if (now - noteStart >= n.dur) {
        idx++;
        if (idx >= imperialLen) idx = initialPos;
        const Note &next = imperialSeq[idx];
        if (next.freq > initialPos) tone(buzzerPin, next.freq);
        else noTone(buzzerPin);
        lastFreq = next.freq;
        noteStart = now;
      }
    }

    void beepHigh() { 
      if (playing) {
        noTone(buzzerPin);
      }
      tone(buzzerPin, beepHighFreq);
      toneStartTime = millis();
      toneDuration = beepHighDuration;
      toneActive = true;
      toneFrequency = 0; 
      playingSecondTone = false;
    }
    
    void playError() { 
      if (playing) {
        noTone(buzzerPin); 
      }
      tone(buzzerPin, errorFreq);
      toneStartTime = millis();
      toneDuration = errorDuration;
      toneActive = true;
      toneFrequency = 0;
      playingSecondTone = false;
    }
    
    void playSuccess() {
      if (playing) {
        noTone(buzzerPin);
      }
      tone(buzzerPin, successFreq1);
      toneStartTime = millis();
      toneDuration = successDuration;
      toneActive = true;
      toneFrequency = successFreq2; 
      playingSecondTone = false;
    }
};

// imperial march note sequence
const Audio::Note Audio::imperialSeq[] = {
  {noteA, 500},{noteA, 500},{noteA, 500},{noteF, 350},{noteCh, 150},
  {noteA, 500},{noteF, 350},{noteCh, 150},{noteA, 650},{0, 500},
  {noteEh, 500},{noteEh, 500},{noteEh, 500},{noteFh, 350},{noteCh, 150},
  {noteGs, 500},{noteF, 350},{noteCh, 150},{noteA, 650},{0, 500},
  {noteAh, 500},{noteA, 300},{noteA, 150},{noteAh, 500},{noteGsh, 325},{noteGh, 175},
  {noteFsh, 125},{noteFh, 125},{noteFsh, 250},{0, 325},
  {noteAs, 250},{noteDsh, 500},{noteDh, 325},{noteCsh, 175},
  {noteCh, 125},{noteB, 125},{noteCh, 250},{0, 350},
  {noteF, 250},{noteGs, 500},{noteF, 350},{noteA, 125},
  {noteCh, 500},{noteA, 375},{noteCh, 125},{noteEh, 650},{0, 500},
  {noteAh, 500},{noteA, 300},{noteA, 150},{noteAh, 500},{noteGsh, 325},{noteGh, 175},
  {noteFsh, 125},{noteFh, 125},{noteFsh, 250},{0, 325},
  {noteAs, 250},{noteDsh, 500},{noteDh, 325},{noteCsh, 175},
  {noteCh, 125},{noteB, 125},{noteCh, 250},{0, 350},
  {noteF, 250},{noteGs, 500},{noteF, 375},{noteCh, 125},
  {noteA, 500},{noteF, 375},{noteCh, 125},{noteA, 650},{0, 650}
};
const int Audio::imperialLen = sizeof(Audio::imperialSeq) / sizeof(Audio::Note);

// forward declaration
class Audio;

// menu class - handles menu display and navigation
class Menu {
  private:
    int selection;
    int itemCount;
    Display* display;
    bool isPauseMenu;

  public:
    Menu(Display* d, int items) : selection(initialPos), itemCount(items), display(d), isPauseMenu(false) {}

    // shows main menu
    void show() {
      display->clearLCD();
      display->printLCD(lcdColStart, lcdRowFirst, "== BATTLESHIP ==");
      isPauseMenu = false;
      selection = initialPos;
      updateSelection();
    }

    // shows pause menu
    void showPauseMenu() {
      display->clearLCD();
      display->printLCD(lcdColStart, lcdRowFirst, "=== PAUSED ===");
      isPauseMenu = true;
      selection = initialPos;
      updateSelection();
    }

    // updates menu selection display
    void updateSelection() {
      display->printLCD(lcdColStart, lcdRowSecond, "                ");
      display->printLCD(lcdColStart, lcdRowSecond, ">");

      if (isPauseMenu) {
        switch (selection) {
          case 0: display->printLCD(lcdColText, lcdRowSecond, "Resume Game "); break;
          case 1: display->printLCD(lcdColText, lcdRowSecond, "Restart Game"); break;
          case 2: display->printLCD(lcdColText, lcdRowSecond, "Exit to Main "); break;
        }
      } else {
        switch (selection) {
          case 0: display->printLCD(lcdColText, lcdRowSecond, "Start Game  "); break;
          case 1: display->printLCD(lcdColText, lcdRowSecond, "About       "); break;
        }
      }
    }

    // navigates menu in given direction
    void navigate(int direction, Audio* audio) {
      int maxItems = isPauseMenu ? menuItemsPause : itemCount;
      selection = (selection + direction + maxItems) % maxItems;
      updateSelection();
      audio->beepHigh();
    }

    int getSelection() const { return selection; }
};

// game controller class - main game logic
class GameController {
  private:
    GameState state;
    GameState stateBeforePause;
    Display* display;
    Input* input;
    Audio* audio;
    Menu* menu;

    Map p1Ships;
    Map p1Attacks;
    Map p2Ships;
    Map p2Attacks;

    Fleet p1Fleet;
    Fleet p2Fleet;

    int cursorX, cursorY;
    
    // non-blocking delay variables
    unsigned long transitionStartTime;
    bool waitingForTransition;
    GameState nextStateAfterWait;
    unsigned long transitionDuration;

    // checks if ship would overlap existing ships
    bool wouldOverlapShip(const Map& map, const Ship& ship) {
      int sx = ship.getX();
      int sy = ship.getY();
      int len = ship.getLength();

      if (ship.isHorizontal()) {
        for (int i = initialPos; i < len; i++) {
          if (map.get(sx, sy + i) == SHIP) return true;
        }
      } else {
        for (int i = initialPos; i < len; i++) {
          if (map.get(sx + i, sy) == SHIP) return true;
        }
      }
      return false;
    }

    // skips cursor over existing ships
    int skipOverShip(const Map& map, const Ship& ship, int dx, int dy) {
      int sx = ship.getX();
      int sy = ship.getY();
      int len = ship.getLength();

      if (dx != dirNeutral) {
        int startX = sx;
        int maxX = ship.isHorizontal() ? maxXHorizontal : mapWidth - len;
        
        while (sx >= initialPos && sx <= maxX) {
          sx += dx;
          if (sx < initialPos || sx > maxX) break;
          
          Ship tmp = ship;
          tmp.setPosition(sx, sy);
          if (!wouldOverlapShip(map, tmp)) {
            return sx;
          }
        }
        return startX;
      }

      if (dy != dirNeutral) {
        int startY = sy;
        int maxY = ship.isHorizontal() ? mapHeight - len : maxYVertical;
        
        while (sy >= initialPos && sy <= maxY) {
          sy += dy;
          if (sy < initialPos || sy > maxY) break;
          
          Ship tmp = ship;
          tmp.setPosition(sx, sy);
          if (!wouldOverlapShip(map, tmp)) {
            return sy;
          }
        }
        return startY;
      }

      return (dx != dirNeutral) ? sx : sy;
    }
    
    void startTransition(unsigned long duration, GameState nextState) {
      waitingForTransition = true;
      transitionStartTime = millis();
      transitionDuration = duration;
      nextStateAfterWait = nextState;
    }
    
    bool checkTransition() {
      if (!waitingForTransition) return false;
      
      if (millis() - transitionStartTime >= transitionDuration) {
        waitingForTransition = false;
        state = nextStateAfterWait;
        return true;
      }
      return false;
    }

    // handles main menu input and navigation
    void handleMenu() {
      int dx, dy;
      dx = dy = dirNeutral;
      input->readJoystickRepeatMenu(pinJoy1X, pinJoy1Y, dx, dy);
      if (dy != dirNeutral) menu->navigate(dy, audio);

      int btn = input->checkButtonPress(pinJoy1Btn);
      if (btn == btnShortPress) {
        audio->playSuccess(); 
        input->resetButtonState();
        switch (menu->getSelection()) {
          case 0: state = GAME_INIT; break;
          case 1: state = MENU_ABOUT; showAbout(); break;
        }
      }

      int pauseBtn = input->checkPauseButton();
      if (pauseBtn == btnShortPress) {
        audio->beepHigh(); 
        stateBeforePause = state;
        state = MENU_PAUSE;
        menu->showPauseMenu();
      }
    }

    // handles pause menu input
    void handlePauseMenu() {
      int dx, dy; 
      dx = dy = dirNeutral;
      input->readJoystickRepeatMenu(pinJoy1X, pinJoy1Y, dx, dy);
      if (dy != dirNeutral) menu->navigate(dy, audio);

      int btn = input->checkButtonPress(pinJoy1Btn);
      if (btn == btnShortPress) {
        audio->playSuccess(); 
        input->resetButtonState();
        switch (menu->getSelection()) {
          case 0:
            state = stateBeforePause;
            display->clearLCD();
            display->drawMap(p1Ships, matrixP1Start);
            display->drawMap(p2Ships, matrixP2Start);
            break;
          case 1:
            resetGame();
            state = GAME_INIT;
            break;
          case 2:
            state = MENU_MAIN;
            menu->show();
            break;
        }
      }
    }

    // shows about screen
    void showAbout() {
      display->clearLCD();
      display->printLCD(lcdColStart, lcdRowFirst, "BATTLESHIP v1.0");
      display->printLCD(lcdColStart, lcdRowSecond, "Matrix Project");
    }

    // handles about screen input
    void handleAbout() {
      int btnState = input->checkButtonPress(pinJoy1Btn);
      if (btnState == btnShortPress) {
        audio->beepHigh(); 
        input->resetButtonState();
        state = MENU_MAIN;
        menu->show();
      }
      int pauseBtn = input->checkPauseButton();
      if (pauseBtn == btnShortPress) {
        audio->beepHigh(); 
        stateBeforePause = state;
        state = MENU_PAUSE;
        menu->showPauseMenu();
      }
    }

    // resets game to initial state
    void resetGame() {
      p1Ships.clear(); 
      p2Ships.clear();
      p1Attacks.clear(); 
      p2Attacks.clear();
      p1Fleet.reset(); 
      p2Fleet.reset();
      display->clearMatrices();

      display->clearLCD();
      display->printLCD(lcdColText, lcdRowFirst, "Game Reset!");
      startTransition(1000, MENU_MAIN);
    }

    // initializes new game
    void initGame() {
      if (waitingForTransition) {
        if (checkTransition()) {
          updatePlacementDisplay(playerOne);
        }
        return;
      }
      
      p1Ships.clear(); 
      p2Ships.clear();
      p1Attacks.clear(); 
      p2Attacks.clear();
      p1Fleet.reset(); 
      p2Fleet.reset();

      cursorX = initialPos; 
      cursorY = initialPos;

      display->clearLCD();
      display->clearMatrices();
      display->printLCD(lcdColStart, lcdRowFirst, "   PLAYER 1");
      display->printLCD(lcdColStart, lcdRowSecond, "  Place Ships!");
      
      startTransition(1000, GAME_PLACE_SHIPS_P1);
    }

    // handles ship placement for specified player
    void handleShipPlacement(int player) {
      if (waitingForTransition) {
        if (checkTransition()) {
          updatePlacementDisplay(player == playerOne ? playerTwo : playerOne);
        }
        return;
      }
      
      Fleet* fleet = (player == playerOne) ? &p1Fleet : &p2Fleet;
      Map* map = (player == playerOne) ? &p1Ships : &p2Ships;
      
      int joyX = (player == playerOne) ? pinJoy1X : pinJoy2X;
      int joyY = (player == playerOne) ? pinJoy1Y : pinJoy2Y;
      int joyBtn = (player == playerOne) ? pinJoy1Btn : pinJoy2Btn;

      Ship* currentShip = fleet->getCurrentShip();

      if (!currentShip) {
        if (player == playerOne) {
          display->clearLCD();
          display->clearMatrices();
          display->printLCD(lcdColStart, lcdRowFirst, "   PLAYER 2");
          display->printLCD(lcdColStart, lcdRowSecond, "  Place Ships!");
          cursorX = initialPos; 
          cursorY = initialPos;
          startTransition(1000, GAME_PLACE_SHIPS_P2);
        } else {
          display->clearMatrices();
          display->drawMap(p1Ships, matrixP1Start);
          display->drawMap(p2Ships, matrixP2Start);
          display->clearLCD();
          display->printLCD(lcdColStart, lcdRowFirst, " These are the");
          display->printLCD(lcdColStart, lcdRowSecond, "     Maps!");
          state = GAME_SHOW_MAPS;
        }
        return;
      }

      currentShip->setPosition(cursorX, cursorY);

      int matrixStart = (player == playerOne) ? matrixP1Start : matrixP2Start;
      display->drawMapWithShipPreview(*map, *currentShip, matrixStart);

      int dx = dirNeutral, dy = dirNeutral;
      input->readJoystickRepeatGame(joyX, joyY, dx, dy);

      if (dx != dirNeutral) {
        int newX = cursorX + dx;
        if (currentShip->isHorizontal()) {
          newX = constrain(newX, initialPos, maxXHorizontal);
        } else {
          newX = constrain(newX, initialPos, mapWidth - currentShip->getLength());
        }
        
        currentShip->setPosition(newX, cursorY);
        if (wouldOverlapShip(*map, *currentShip)) {
          newX = skipOverShip(*map, *currentShip, dx, dirNeutral);
          if (currentShip->isHorizontal()) {
            newX = constrain(newX, initialPos, maxXHorizontal);
          } else {
            newX = constrain(newX, initialPos, mapWidth - currentShip->getLength());
          }
        }
        cursorX = newX;
      }

      if (dy != dirNeutral) {
        int newY = cursorY + dy;
        if (currentShip->isHorizontal()) {
          newY = constrain(newY, initialPos, mapHeight - currentShip->getLength());
        } else {
          newY = constrain(newY, initialPos, maxYVertical);
        }
        
        currentShip->setPosition(cursorX, newY);
        if (wouldOverlapShip(*map, *currentShip)) {
          newY = skipOverShip(*map, *currentShip, dirNeutral, dy);
          if (currentShip->isHorizontal()) {
            newY = constrain(newY, initialPos, mapHeight - currentShip->getLength());
          } else {
            newY = constrain(newY, initialPos, maxYVertical);
          }
        }
        cursorY = newY;
      }

      int btnState = input->checkButtonPress(joyBtn);
      if (btnState == btnShortPress) {
        currentShip->toggleOrientation();
        audio->beepHigh();
        if (currentShip->isHorizontal()) {
          cursorX = constrain(cursorX, initialPos, maxXHorizontal);
          cursorY = constrain(cursorY, initialPos, mapHeight - currentShip->getLength());
        } else {
          cursorX = constrain(cursorX, initialPos, mapWidth - currentShip->getLength());
          cursorY = constrain(cursorY, initialPos, maxYVertical);
        }
        input->resetButtonState();
      } else if (btnState == btnLongPress) {
        currentShip->setPosition(cursorX, cursorY);
        if (map->canPlaceShip(*currentShip)) {
          map->placeShip(*currentShip);
          currentShip->setPlaced(true);
          fleet->nextShip();
          audio->playSuccess();
          cursorX = initialPos; 
          cursorY = initialPos;
          updatePlacementDisplay(player);
        } else {
          audio->playError();
        }
        input->resetButtonState();
      }

      int pauseBtn = input->checkPauseButton();
      if (pauseBtn == btnShortPress) {
        stateBeforePause = (player == playerOne) ? GAME_PLACE_SHIPS_P1 : GAME_PLACE_SHIPS_P2;
        state = MENU_PAUSE;
        menu->showPauseMenu();
      }
    }

    // updates lcd with ship placement info
    void updatePlacementDisplay(int player) {
      Fleet* fleet = (player == playerOne) ? &p1Fleet : &p2Fleet;
      Ship* ship = fleet->getCurrentShip();

      display->clearLCD();
      char buf[lcdWidth + dirPositive];

      sprintf(buf, "P%d Ships:[%d]", player, fleet->getRemainingShips());
      display->printLCD(lcdColStart, lcdRowFirst, buf);

      if (ship) {
        sprintf(buf, "%s (%d)", ship->getName(), ship->getLength());
        display->printLCD(lcdColStart, lcdRowSecond, buf);
      } else {
        display->printLCD(lcdColStart, lcdRowSecond, "All Done!");
      }
    }

    // handles map display state
    void handleShowMaps() {
      int pauseBtn = input->checkPauseButton();
      if (pauseBtn == btnShortPress) {
        stateBeforePause = GAME_SHOW_MAPS;
        state = MENU_PAUSE;
        menu->showPauseMenu();
        return;
      }
      display->drawMap(p1Ships, matrixP1Start);
      display->drawMap(p2Ships, matrixP2Start);
    }

    // handles gameplay state
    void handleGamePlay() {
      int pauseBtn = input->checkPauseButton();
      if (pauseBtn == btnShortPress) {
        stateBeforePause = state;
        state = MENU_PAUSE;
        menu->showPauseMenu();
      }
    }

  public:
    GameController(Display* d, Input* i, Audio* a, Menu* m) :
      state(MENU_MAIN), stateBeforePause(MENU_MAIN), display(d), input(i), audio(a), menu(m),
      cursorX(initialPos), cursorY(initialPos), transitionStartTime(0), 
      waitingForTransition(false), nextStateAfterWait(MENU_MAIN), transitionDuration(0) {}

    void init() {
      display->clearMatrices();
      menu->show();
    }

    // main update loop
    void update() {
      audio->update();

      if (state == MENU_MAIN && waitingForTransition) {
        if (checkTransition()) {
          menu->show();
        }
        return;
      }

      bool inMenu = (state == MENU_MAIN || state == MENU_PAUSE || state == MENU_ABOUT);
      if (inMenu && !audio->isPlaying()) {
        audio->startImperialMarch();
      } else if (!inMenu && audio->isPlaying()) {
        audio->stopImperialMarch();
      }

      switch (state) {
        case MENU_MAIN: handleMenu(); break;
        case MENU_PAUSE: handlePauseMenu(); break;
        case MENU_ABOUT: handleAbout(); break;
        case GAME_INIT: initGame(); break;
        case GAME_PLACE_SHIPS_P1: handleShipPlacement(playerOne); break;
        case GAME_PLACE_SHIPS_P2: handleShipPlacement(playerTwo); break;
        case GAME_SHOW_MAPS: handleShowMaps(); break;
        case GAME_PLAY_P1:
        case GAME_PLAY_P2: handleGamePlay(); break;
      }
    }
};

// global objects
LedControl lc(pinDin, pinClk, pinCs, matrixCount);
LiquidCrystal_I2C lcd(lcdI2cAddress, lcdWidth, lcdHeight);

Display display(&lc, &lcd);
Input input;
Audio audio(pinBuzzer);
Menu menu(&display, menuItemsMain);
GameController game(&display, &input, &audio, &menu);

void setup() {
  Serial.begin(9600);

  pinMode(pinJoy1Btn, INPUT_PULLUP);
  pinMode(pinJoy2Btn, INPUT_PULLUP);
  pinMode(pinBtnPause, INPUT_PULLUP);
  pinMode(pinBuzzer, OUTPUT);

  display.init();
  audio.begin();
  game.init();
}

void loop() {
  game.update();
}