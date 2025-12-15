#include <LedControl.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <avr/pgmspace.h>
#include <EEPROM.h>

// global constants and settings

// hardware pins
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

const int pinP1Trig = 4;
const int pinP1Echo = 5;
const int pinP2Trig = 8;
const int pinP2Echo = 9;

// display settings
const int lcdI2cAddr = 0x27;
const int lcdCols = 16;
const int lcdRows = 2;
const int numMatrices = 4;
const int matrixSize = 8;
const int mapWidth = 8;
const int mapHeight = 16;
const int matrixStartLeft = 0;
const int matrixStartRight = 2;
const int lcdColStart = 0;
const int lcdRowTop = 0;
const int lcdRowBottom = 1;
const int lcdCursorOffset = 2;
const int lcdNameInputCol = 6;
const int lcdNameWinCol = 4;

// bit manipulation for font
const int bitShift4 = 4;
const int bitShift2 = 2;
const int bitShift1 = 1;
const byte bitMaskF0 = 0xF0;
const byte bitMask0F = 0x0F;
const byte bitMaskCC = 0xCC;
const byte bitMask33 = 0x33;
const byte bitMaskAA = 0xAA;
const byte bitMask55 = 0x55;
const byte allLedsOn = 0xFF;
const int scrollCharSize = 8;
const int scrollScreenSize = 16;

// timing and speeds (used for non-blocking logic)
const unsigned long joyInitDelayMs = 200;
const unsigned long joyRepeatDelayMs = 100;
const unsigned long btnLongPressMs = 400;
const unsigned long sonarTriggerMs = 800;
const unsigned long scrollSpeedUs = 2000;
const unsigned long blinkCursorMs = 100; 
const unsigned long blinkMissMs = 500;
const unsigned long blinkPlaceMs = 200;
const unsigned long transitionDelayMs = 2000;
const unsigned long audioShortDuration = 100;
const unsigned long audioMedPause = 200;
const unsigned long audioToneBaseDuration = 100;
const unsigned long charInputDelayMs = 150;
const unsigned long blinkInputMs = 300;
const unsigned long airstrikeAnimMs = 1000;

// joystick calibration
const int joyThresholdLow = 300;
const int joyThresholdHigh = 700;
const int joyDeltaPos = 1;
const int btnPressNone = 0;
const int btnPressShort = 1;
const int btnPressLong = 2;

// game rules and limits
const int fleetSize = 7;
const int maxSonarAmmo = 2;
const int maxPlayers = 5;
const int sonarMaxDist = 15;
const int nameLen = 4;
const int nameLenChars = 3;
const int winBeepCount = 3;
const int playerTurn1 = 1;
const int playerTurn2 = 2;
const int maxMatrixBri = 15;
const int maxLcdBri = 10;
const int sonarOutRange = 999;
const int sonarPulseTimeout = 15000;
const int sonarMicroDelay1 = 2;
const int sonarMicroDelay2 = 10;
const float sonarSpeedFactor = 0.034;
const int sonarSpeedDivisor = 2;
const int areaDamageRadius = 1;

// eeprom memory map
const int addrSettings = 0;
const int addrPlayerCount = 10;
const int addrPlayerData = 11;
const int defaultMatrixBri = 8;
const int defaultLcdBri = 10;

// ship id mapping
const byte shipIdCarrier = 0;
const byte shipIdBattle = 1;
const byte shipIdCruiser = 2;
const byte shipIdSub = 3;
const byte shipIdDest = 4;
const int shipLenCarrier = 5;
const int shipLenBattle = 4;
const int shipLenCruiser = 3;
const int shipLenSub = 3;
const int shipLenDest = 2;

// map cell values
const byte mapCellEmpty = 0;
const byte mapCellShip = 1;
const byte mapCellHit = 2;
const byte mapCellMiss = 3;

// audio frequencies
const int noteC4 = 262;
const int noteD4 = 294;
const int noteE4 = 330;
const int noteF4 = 349;
const int noteG4 = 392;
const int noteGs4 = 415;
const int noteA4 = 440;
const int noteAs4 = 466;
const int noteB4 = 494;
const int noteC5 = 523;
const int noteE5 = 659;
const int noteF5 = 698;

const int toneNav = 2000;
const int toneOk = 1500;
const int toneErr = 300;
const int toneHitBase = 1200;
const int toneHitMod = 400;
const int toneBoomBase = 50;
const int toneBoomModFactor = 2;
const int toneWin = 1000;
const int toneWinEnd = 2000;

// string literals (progmem)
const char strEmpty[] PROGMEM = "";
const char strSpace[] PROGMEM = " ";
const char strUnderscore[] PROGMEM = "_";
const char strCursor[] PROGMEM = ">";
const char strCursorSpace[] PROGMEM = "> ";
const char strDefaultName[] PROGMEM = "AAA";

const char strShipCarrier[] PROGMEM = "Carrier";
const char strShipBattle[] PROGMEM = "Battleships";
const char strShipCruiser[] PROGMEM = "Cruiser";
const char strShipSub[] PROGMEM = "Submarine";
const char strShipDest[] PROGMEM = "Destroyer";
const char strShipGeneric[] PROGMEM = "Ship";

const char strMenuStart[] PROGMEM = "Start Game";
const char strMenuSett[] PROGMEM = "Settings";
const char strMenuHowto[] PROGMEM = "How to Play";
const char strMenuLeader[] PROGMEM = "Leaderboard";
const char strMenuAbout[] PROGMEM = "About";
const int mainMenuCount = 5;
const int menuStartIdx = 0;
const int menuSettingsIdx = 1;
const int menuHowtoIdx = 2;
const int menuLeaderIdx = 3;
const int menuAboutIdx = 4;

const char strPauseResume[] PROGMEM = "Resume";
const char strPauseRestart[] PROGMEM = "Restart";
const char strPauseExit[] PROGMEM = "Exit";
const int pauseMenuCount = 3;
const int pauseMenuResumeIdx = 0;
const int pauseMenuRestartIdx = 1;
const int pauseMenuExitIdx = 2;

const char strTitleMain[] PROGMEM = " = Battleships = ";
const char strTitlePause[] PROGMEM = " Paused ";
const char strTitleLeader[] PROGMEM = "Leaderboard";
const char strNoRecords[] PROGMEM = "No records";
const char strSelP1[] PROGMEM = "Select player 1";
const char strSelP2[] PROGMEM = "Select player 2";
const char strNewName[] PROGMEM = "> New name";
const char strP1NameLbl[] PROGMEM = "P1 name:";
const char strP2NameLbl[] PROGMEM = "P2 name:";
const char strBothReady[] PROGMEM = " Both ready! ";
const char strBattleStart[] PROGMEM = " Battle start ";
const char strAirstrike[] PROGMEM = "Airstrike!!!";
const char strGameTitleScroll[] = "BATTLESHIPS";
const char strWinsSuffix[] PROGMEM = " wins!";

const char strSettMatBri[] PROGMEM = "Matrix Bright";
const char strSettLcdBri[] PROGMEM = "Lcd Brightness";
const char strSettSound[] PROGMEM = "Sound";

const char strSettReset[] PROGMEM = "Reset Leaderboard";
const char strSettBack[] PROGMEM = "Back";
const char strSettOn[] PROGMEM = "< on >";
const char strSettOff[] PROGMEM = "< off >";
const char strPressBtn[] PROGMEM = "Press btn";

const int settingsMenuCount = 5;
const int settingsMatrixBriIdx = 0;
const int settingsLcdBriIdx = 1;
const int settingsSoundIdx = 2;
const int settingsResetIdx = 3;
const int settingsBackIdx = 4;

const char strHowto1A[] PROGMEM = "How To Play:";
const char strHowto1B[] PROGMEM = "1.Select name";
const char strHowto2A[] PROGMEM = "2.Place ships";
const char strHowto2B[] PROGMEM = "3.Find enemy";
const char strHowto3A[] PROGMEM = "Fire = button";
const char strHowto3B[] PROGMEM = "Sonar = hand";
const char strHowto4A[] PROGMEM = "Hit = again";
const char strHowto4B[] PROGMEM = "Miss = pass";
const int howToPagesCount = 4;
const int howToPage1 = 0;
const int howToPage2 = 1;
const int howToPage3 = 2;
const int howToPage4 = 3;

const char strAboutTitle[] PROGMEM = "BATTLESHIPS PRO";
const char strAboutSubtitle[] PROGMEM = "Matrix Project";
const char strAboutAuthName[] PROGMEM = "Authors:";
const char strAboutAuthVal1[] PROGMEM = "Preoteasa M";
const char strAboutAuthVal2[] PROGMEM = "Dumitriu F";
const char strAboutGitName[] PROGMEM = "GitHub:";
const char strAboutGitVal1[] PROGMEM = "Filip407";
const char strAboutGitVal2[] PROGMEM = "MitzaaA14";

const int aboutPageCount = 5;
const int aboutPageTitle = 0;
const int aboutPageAuthors1 = 1;
const int aboutPageAuthors2 = 2;
const int aboutPageGit1 = 3;
const int aboutPageGit2 = 4;


// format strings (printf)
const char fmtLeaderboard[] PROGMEM = "%d.%s wins:%d";
const char fmtSettVal[] PROGMEM = "< %d >";
const char fmtP1Ship[] PROGMEM = "P1: Ship(%d)";
const char fmtP1Ready[] PROGMEM = "P1: Ready";
const char fmtP2Ship[] PROGMEM = "P2: Ship(%d)";
const char fmtP2Ready[] PROGMEM = "P2: Ready";
const char fmtTurnStatus[] PROGMEM = "%s turn S:%d";
const char fmtStats[] PROGMEM = "Myships:%d Tgt:%d";
const char fmtWinMsg[] PROGMEM = "%s wins";

// initial settings (loaded from eeprom)
int settingMatrixBri = defaultMatrixBri;
int settingLcdBri = defaultLcdBri;
bool settingSound = true;

// font data (8x8)

// helper to map char to font index
int getCharIndex(char c) {
  if (c == ' ') return 0;
  // map a-z/a-z to index 1-26
  if (c >= 'A' && c <= 'Z') return c - 'A' + 1;
  if (c >= 'a' && c <= 'z') return c - 'a' + 1;
  // map 0-9 to index 27-36
  if (c >= '0' && c <= '9') return c - '0' + 27;
  return 0;
}

const byte font8x8[][8] PROGMEM = {
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },  // 0: space
  { 0x7E, 0x11, 0x11, 0x11, 0x7E, 0x00, 0x00, 0x00 },  // 1: a
  { 0x7F, 0x49, 0x49, 0x49, 0x36, 0x00, 0x00, 0x00 },  // 2: b
  { 0x3E, 0x41, 0x41, 0x41, 0x22, 0x00, 0x00, 0x00 },  // 3: c
  { 0x7F, 0x41, 0x41, 0x22, 0x1C, 0x00, 0x00, 0x00 },  // 4: d
  { 0x7F, 0x49, 0x49, 0x49, 0x41, 0x00, 0x00, 0x00 },  // 5: e
  { 0x7F, 0x09, 0x09, 0x09, 0x01, 0x00, 0x00, 0x00 },  // 6: f
  { 0x3E, 0x41, 0x49, 0x49, 0x7A, 0x00, 0x00, 0x00 },  // 7: g
  { 0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00, 0x00, 0x00 },  // 8: h
  { 0x00, 0x41, 0x7F, 0x41, 0x00, 0x00, 0x00, 0x00 },  // 9: i
  { 0x20, 0x40, 0x41, 0x3F, 0x01, 0x00, 0x00, 0x00 },  // 10: j
  { 0x7F, 0x08, 0x14, 0x22, 0x41, 0x00, 0x00, 0x00 },  // 11: k
  { 0x7F, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00 },  // 12: l
  { 0x7F, 0x02, 0x0C, 0x02, 0x7F, 0x00, 0x00, 0x00 },  // 13: m
  { 0x7F, 0x04, 0x08, 0x10, 0x7F, 0x00, 0x00, 0x00 },  // 14: n
  { 0x3E, 0x41, 0x41, 0x41, 0x3E, 0x00, 0x00, 0x00 },  // 15: o
  { 0x7F, 0x09, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00 },  // 16: p
  { 0x3E, 0x41, 0x51, 0x21, 0x5E, 0x00, 0x00, 0x00 },  // 17: q
  { 0x7F, 0x09, 0x19, 0x29, 0x46, 0x00, 0x00, 0x00 },  // 18: r
  { 0x46, 0x49, 0x49, 0x49, 0x31, 0x00, 0x00, 0x00 },  // 19: s
  { 0x01, 0x01, 0x7F, 0x01, 0x01, 0x00, 0x00, 0x00 },  // 20: t
  { 0x3F, 0x40, 0x40, 0x40, 0x3F, 0x00, 0x00, 0x00 },  // 21: u
  { 0x1F, 0x20, 0x40, 0x20, 0x1F, 0x00, 0x00, 0x00 },  // 22: v
  { 0x3F, 0x40, 0x38, 0x40, 0x3F, 0x00, 0x00, 0x00 },  // 23: w
  { 0x63, 0x14, 0x08, 0x14, 0x63, 0x00, 0x00, 0x00 },  // 24: x
  { 0x07, 0x08, 0x70, 0x08, 0x07, 0x00, 0x00, 0x00 },  // 25: y
  { 0x61, 0x51, 0x49, 0x45, 0x43, 0x00, 0x00, 0x00 },  // 26: z
  { 0x3E, 0x51, 0x49, 0x45, 0x3E, 0x00, 0x00, 0x00 },  // 27: 0
  { 0x00, 0x42, 0x7F, 0x40, 0x00, 0x00, 0x00, 0x00 },  // 28: 1
  { 0x42, 0x61, 0x51, 0x49, 0x46, 0x00, 0x00, 0x00 },  // 29: 2
  { 0x21, 0x41, 0x45, 0x4B, 0x31, 0x00, 0x00, 0x00 },  // 30: 3
  { 0x18, 0x14, 0x12, 0x7F, 0x10, 0x00, 0x00, 0x00 },  // 31: 4
  { 0x27, 0x45, 0x45, 0x45, 0x39, 0x00, 0x00, 0x00 },  // 32: 5
  { 0x3C, 0x4A, 0x49, 0x49, 0x30, 0x00, 0x00, 0x00 },  // 33: 6
  { 0x01, 0x71, 0x09, 0x05, 0x03, 0x00, 0x00, 0x00 },  // 34: 7
  { 0x36, 0x49, 0x49, 0x49, 0x36, 0x00, 0x00, 0x00 },  // 35: 8
  { 0x06, 0x49, 0x49, 0x29, 0x1E, 0x00, 0x00, 0x00 },  // 36: 9
};

// custom matrix patterns

// pattern for Start Game
const byte patternStartGame[] PROGMEM = {
  0x00, 0x18, 0x3C, 0x7E, 0x3C, 0x18, 0x00, 0x00
};

// pattern for Settings
const byte patternSettings[] PROGMEM = {
  0x3C, 0x42, 0xA5, 0x99, 0xA5, 0x42, 0x3C, 0x00
};

// pattern for How to Play
const byte patternHowto[] PROGMEM = {
  0x00, 0x3C, 0x42, 0x60, 0x18, 0x00, 0x18, 0x00
};

// pattern for Leaderboard
const byte patternLeaderboard[] PROGMEM = {
  0x00, 0x10, 0x38, 0x7C, 0x10, 0x10, 0x00, 0x00
};

// pattern for About
const byte patternAbout[] PROGMEM = {
  0x00, 0x10, 0x00, 0x18, 0x10, 0x10, 0x38, 0x00
};

// data structures and enums

struct PlayerRecord {
  char name[nameLen];
  int wins;
};

struct GameSettings {
  byte matrixBrightness;
  byte lcdBrightness;
  bool soundEnabled;
};

struct Note {
  int freq;
  int duration;
};

enum GameState {
  menuMain,
  menuSettings,
  menuHowto,
  menuAbout,
  menuLeaderboard,
  menuSelectP1,
  menuSelectP2,
  menuNameInput,
  menuPause,
  gameInit,
  gamePlaceShips,
  gamePlay,
  gameWin
};

// classes

class Storage {
public:
  int playerCount;
  PlayerRecord players[maxPlayers];

  void init() {
    playerCount = EEPROM.read(addrPlayerCount);
    // basic validation
    if (playerCount > maxPlayers || playerCount < 0) {
      playerCount = 0;
      EEPROM.write(addrPlayerCount, 0);
    }
    int addr = addrPlayerData;
    for (int i = 0; i < playerCount; i++) {
      EEPROM.get(addr, players[i]);
      addr += sizeof(PlayerRecord);
    }
  }

  void saveSettings(int matBri, int lcdBri, bool snd) {
    GameSettings s;
    s.matrixBrightness = (byte)matBri;
    s.lcdBrightness = (byte)lcdBri;
    s.soundEnabled = snd;
    EEPROM.put(addrSettings, s);
  }

  void loadSettings(int& matBri, int& lcdBri, bool& snd) {
    GameSettings s;
    EEPROM.get(addrSettings, s);
    // basic validation
    if (s.matrixBrightness > maxMatrixBri || s.lcdBrightness > maxLcdBri) {
      matBri = defaultMatrixBri;
      lcdBri = defaultLcdBri;
      snd = true;
    } else {
      matBri = s.matrixBrightness;
      lcdBri = s.lcdBrightness;
      snd = s.soundEnabled;
    }
  }

  // adds new player if name doesn't exist
  void savePlayer(const char* name) {
    for (int i = 0; i < playerCount; i++) {
      if (strcmp(players[i].name, name) == 0) return;
    }
    if (playerCount < maxPlayers) {
      strcpy(players[playerCount].name, name);
      players[playerCount].wins = 0;
      playerCount++;
      saveToEEPROM();
    }
  }

  // increments win count for a player
  void addWin(const char* name) {
    for (int i = 0; i < playerCount; i++) {
      if (strcmp(players[i].name, name) == 0) {
        players[i].wins++;
        saveToEEPROM();
        return;
      }
    }
  }
  
  // reseys leaderboard
  void resetLeaderboard() {
    playerCount = 0;
    EEPROM.update(addrPlayerCount, 0);
  }

  // commits data to permanent memory
  void saveToEEPROM() {
    EEPROM.update(addrPlayerCount, playerCount);
    int addr = addrPlayerData;
    for (int i = 0; i < playerCount; i++) {
      EEPROM.put(addr, players[i]);
      addr += sizeof(PlayerRecord);
    }
  }

  // bubble sort players by wins
  void sortLeaderboard() {
    for (int i = 0; i < playerCount - 1; i++) {
      for (int j = 0; j < playerCount - i - 1; j++) {
        if (players[j].wins < players[j + 1].wins) {
          PlayerRecord temp = players[j];
          players[j] = players[j + 1];
          players[j + 1] = temp;
        }
      }
    }
  }
};

class Sonar {
private:
  int trigPin, echoPin;
public:
  Sonar(int trig, int echo) : trigPin(trig), echoPin(echo) {}

  void init() {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
  }

  // returns distance in cm or sonaroutrange if out of range
  int getDistance() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(sonarMicroDelay1);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(sonarMicroDelay2);
    digitalWrite(trigPin, LOW);
    long duration = pulseIn(echoPin, HIGH, sonarPulseTimeout);
    if (duration == 0) return sonarOutRange;
    return duration * sonarSpeedFactor / sonarSpeedDivisor;
  }
};

class Map {
private:
  byte data[mapWidth][mapHeight];
public:
  Map() { clear(); }

  void clear() {
    memset(data, 0, sizeof(data));
  }

  byte get(int x, int y) const {
    if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight) return data[x][y];
    return mapCellEmpty;
  }

  void set(int x, int y, byte value) {
    if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight) data[x][y] = value;
  }
};

class Ship {
private:
  int length, coordX, coordY;
  bool horizontal, placed;
  byte typeId;

public:
  Ship(byte id, int len) : typeId(id), length(len), coordX(0), coordY(0), horizontal(true), placed(false) {}

  int getLength() const { return length; }
  int getX() const { return coordX; }
  int getY() const { return coordY; }
  bool isHorizontal() const { return horizontal; }
  bool isPlaced() const { return placed; }

  const __FlashStringHelper* getName() const {
    switch (typeId) {
      case shipIdCarrier: return (__FlashStringHelper*)strShipCarrier;
      case shipIdBattle: return (__FlashStringHelper*)strShipBattle;
      case shipIdCruiser: return (__FlashStringHelper*)strShipCruiser;
      case shipIdSub: return (__FlashStringHelper*)strShipSub;
      case shipIdDest: return (__FlashStringHelper*)strShipDest;
      default: return (__FlashStringHelper*)strShipGeneric;
    }
  }

  void setPosition(int newX, int newY) { coordX = newX; coordY = newY; }
  void toggleOrientation() { horizontal = !horizontal; }
  void setPlaced(bool p) { placed = p; }

  // checks if all parts of ship are hit (mapcellhit value)
  bool isSunk(const Map& map) {
    if (!placed) return false;
    if (horizontal) {
      for (int i = 0; i < length; i++)
        if (map.get(coordX + i, coordY) != mapCellHit) return false;
    } else {
      for (int i = 0; i < length; i++)
        if (map.get(coordX, coordY + i) != mapCellHit) return false;
    }
    return true;
  }
};

class MapHelper {
public:
  // validates if ship fits within bounds
  static bool canPlace(const Map& map, const Ship& ship) {
    int sx = ship.getX();
    int sy = ship.getY();
    int sl = ship.getLength();
    if (ship.isHorizontal()) {
      if (sx < 0 || sx + sl > mapWidth || sy < 0 || sy >= mapHeight) return false;
      for (int i = 0; i < sl; i++)
        if (map.get(sx + i, sy) != mapCellEmpty) return false;
    } else {
      if (sx < 0 || sx >= mapWidth || sy < 0 || sy + sl > mapHeight) return false;
      for (int i = 0; i < sl; i++)
        if (map.get(sx, sy + i) != mapCellEmpty) return false;
    }
    return true;
  }

  // writes ship id to map
  static void place(Map& map, const Ship& ship) {
    int sx = ship.getX();
    int sy = ship.getY();
    if (ship.isHorizontal()) {
      for (int i = 0; i < ship.getLength(); i++) map.set(sx + i, sy, mapCellShip);
    } else {
      for (int i = 0; i < ship.getLength(); i++) map.set(sx, sy + i, mapCellShip);
    }
  }

  // checks collision with existing ships
  static bool overlaps(const Map& map, const Ship& ship) {
    int sx = ship.getX();
    int sy = ship.getY();
    if (ship.isHorizontal()) {
      for (int i = 0; i < ship.getLength(); i++)
        if (map.get(sx + i, sy) == mapCellShip) return true;
    } else {
      for (int i = 0; i < ship.getLength(); i++)
        if (map.get(sx, sy + i) == mapCellShip) return true;
    }
    return false;
  }
};

class Fleet {
private:
  Ship* ships[fleetSize];
  int index;
public:
  Fleet() : index(0) {
    // fleet composition
    ships[0] = new Ship(shipIdCarrier, shipLenCarrier);
    ships[1] = new Ship(shipIdBattle, shipLenBattle);
    ships[2] = new Ship(shipIdCruiser, shipLenCruiser);
    ships[3] = new Ship(shipIdSub, shipLenSub);
    ships[4] = new Ship(shipIdSub, shipLenSub);
    ships[5] = new Ship(shipIdDest, shipLenDest);
    ships[6] = new Ship(shipIdDest, shipLenDest);
  }
  ~Fleet() {
    for (int i = 0; i < fleetSize; i++) delete ships[i];
  }

  void reset() {
    index = 0;
    for (int i = 0; i < fleetSize; i++) {
      ships[i]->setPlaced(false);
      ships[i]->setPosition(0, 0);
    }
  }

  Ship* currentShip() { return (index < fleetSize) ? ships[index] : nullptr; }
  void next() { if (index < fleetSize) index++; }
  bool done() const { return index >= fleetSize; }

  // returns number of ships not fully sunk
  int aliveCount(const Map& map) {
    int count = 0;
    for (int i = 0; i < fleetSize; i++) {
      if (ships[i]->isPlaced() && !ships[i]->isSunk(map)) count++;
    }
    return count;
  }
};

class Audio {
private:
  int pin;
  struct Note { int freq; int duration; };
  static const Note imperialSeq[];
  static const int imperialLen;
  int noteIndex;
  unsigned long noteStart, toneStart;
  bool playing, toneActive, secondTone;
  int lastFreq, toneFreq;

public:
  Audio(int p) : pin(p), noteIndex(0), playing(false), lastFreq(-1), toneActive(false), secondTone(false), toneFreq(0) {}

  void begin() { pinMode(pin, OUTPUT); }

  // non-blocking tone wrapper
  void safeTone(int freq, int duration = 0) {
    if (!settingSound) {
      noTone(pin);
      return;
    }
    if (duration > 0) tone(pin, freq, duration);
    else tone(pin, freq);
  }

  // processes music or sound effects
  void update() {
    if (!settingSound) {
      if (playing || toneActive) {
        noTone(pin);
        playing = false;
        toneActive = false;
      }
      return;
    }
    unsigned long now = millis();

    // handle short tone (sound effect)
    if (toneActive) {
      if (now - toneStart >= audioToneBaseDuration) {
        if (!secondTone && toneFreq != 0) {
          secondTone = true;
          safeTone(toneFreq);
          toneStart = now;
          return;
        }
        noTone(pin);
        toneActive = false;
        secondTone = false;
        toneFreq = 0;
        // resume music if playing
        if (playing) {
          const Note& next = imperialSeq[noteIndex];
          if (next.freq > 0) safeTone(next.freq);
          else noTone(pin);
          lastFreq = next.freq;
          noteStart = now;
        }
      }
      return;
    }

    // handle music
    if (!playing) return;
    const Note& currentNote = imperialSeq[noteIndex];

    if (lastFreq == -1) { // initial note setup
      if (currentNote.freq > 0) safeTone(currentNote.freq);
      else noTone(pin);
      lastFreq = currentNote.freq;
      noteStart = now;
    }

    if (now - noteStart >= currentNote.duration) {
      noteIndex = (noteIndex + 1) % imperialLen;
      const Note& nextNote = imperialSeq[noteIndex];
      if (nextNote.freq > 0) safeTone(nextNote.freq);
      else noTone(pin);
      lastFreq = nextNote.freq;
      noteStart = now;
    }
  }

  void startMusic() {
    if (playing || !settingSound) return;
    playing = true;
    noteIndex = 0;
    lastFreq = -1;
    noteStart = millis();
  }

  void stopMusic() {
    if (!playing) return;
    playing = false;
    noTone(pin);
    lastFreq = -1;
  }

  bool isMusicPlaying() { return playing; }

  // sound effect presets
  void beepNav() {
    if (!settingSound) return;
    if (toneActive) return;
    noTone(pin); safeTone(toneNav); toneStart = millis(); toneActive = true; toneFreq = 0;
  }
  void beepOk() {
    if (!settingSound) return;
    if (toneActive) return;
    noTone(pin); safeTone(toneOk); toneStart = millis(); toneActive = true; toneFreq = toneNav;
  }
  void beepErr() {
    if (!settingSound) return;
    if (toneActive) return;
    noTone(pin); safeTone(toneErr); toneStart = millis(); toneActive = true; toneFreq = 0;
  }
  void beepHit() {
    if (!settingSound) return;
    if (toneActive) return;
    noTone(pin); safeTone(toneHitBase - toneHitMod); toneStart = millis(); toneActive = true; toneFreq = toneHitBase;
  }
  void beepBoom() {
    if (!settingSound) return;
    if (toneActive) return;
    noTone(pin); safeTone(toneBoomBase * toneBoomModFactor); toneStart = millis(); toneActive = true; toneFreq = toneBoomBase;
  }
  void beepMiss() { beepErr(); }

  // final tone for win screen
  void beepWinStart() {
    if (!settingSound) return;
    stopMusic();
    safeTone(toneWinEnd, blinkMissMs);
  }

  // repetitive tone for win animation
  void beepWinTone() {
    if (!settingSound) return;
    safeTone(toneWin, audioShortDuration);
  }
};

const Audio::Note Audio::imperialSeq[] = {
  { noteA4, 500 }, { noteA4, 500 }, { noteA4, 500 }, { noteF4, 350 }, { noteC5, 150 },
  { noteA4, 500 }, { noteF4, 350 }, { noteC5, 150 }, { noteA4, 650 }, { 0, 500 },
  { noteE5, 500 }, { noteE5, 500 }, { noteE5, 500 }, { noteF5, 350 }, { noteC5, 150 },
  { noteGs4,500 }, { noteF4, 350 }, { noteC5, 150 }, { noteA4, 650 }, { 0, 500 }
};
const int Audio::imperialLen = 20;

class Display {
private:
  LedControl* lc;
  LiquidCrystal_I2C* lcd;

  // reverses bits for correct orientation (vertical flip)
  byte reverseBits(byte b) {
    b = (b & bitMaskF0) >> bitShift4 | (b & bitMask0F) << bitShift4;
    b = (b & bitMaskCC) >> bitShift2 | (b & bitMask33) << bitShift2;
    b = (b & bitMaskAA) >> bitShift1 | (b & bitMask55) << bitShift1;
    return b;
  }

public:
  Display(LedControl* l, LiquidCrystal_I2C* d) : lc(l), lcd(d) {}

  void init() {
    applyMatrixBrightness();
    applyLcdBacklight();
    for (int i = 0; i < numMatrices; i++) {
      lc->shutdown(i, false);
      lc->clearDisplay(i);
    }
    lcd->init();
    lcd->backlight();
    lcd->clear();
  }

  void applyMatrixBrightness() {
    for (int i = 0; i < numMatrices; i++) lc->setIntensity(i, settingMatrixBri);
  }
  void applyLcdBacklight() {
    if (settingLcdBri > 0) lcd->backlight();
    else lcd->noBacklight();
  }
  void clearMatrices() {
    for (int i = 0; i < numMatrices; i++) lc->clearDisplay(i);
  }
  void clearLCD() { lcd->clear(); }
  void printLCD(int col, int row, const __FlashStringHelper* text) {
    lcd->setCursor(col, row);
    lcd->print(text);
  }
  void printLCD(int col, int row, const char* text) {
    lcd->setCursor(col, row);
    lcd->print(text);
  }
  void lightAllMatrix() {
    for (int i = 0; i < numMatrices; i++)
      for (int r = 0; r < matrixSize; r++) lc->setRow(i, r, allLedsOn);
  }

  // scrolls text right to left, corrected for orientation
  void scrollText(const char* text) {
    clearMatrices();
    int len = strlen(text);
    int totalCols = len * matrixSize;
    int totalSpan = mapHeight;

    // main time loop
    for (int pos = 0; pos < totalCols + totalSpan; pos++) {

      // calculate offset for optimization
      int basePos = pos - (totalSpan - 1);

      // loop visible screen columns (0 to 15)
      for (int screenCol = 0; screenCol < totalSpan; screenCol++) {

        int textColIndex = basePos + screenCol;
        byte colData = mapCellEmpty;

        // if inside valid text range
        if (textColIndex >= 0 && textColIndex < totalCols) {
          int charIdx = textColIndex / scrollCharSize;
          int byteIdx = textColIndex % scrollCharSize;

          if (charIdx < len) {
            byte raw = pgm_read_byte(&font8x8[getCharIndex(text[charIdx])][byteIdx]);
            colData = reverseBits(raw);
          }
        }

        // map to physical matrices
        if (screenCol < matrixSize) {
          // left matrices (indices 0 and 2)
          lc->setColumn(matrixStartLeft, screenCol, colData);
          lc->setColumn(matrixStartRight, screenCol, colData);
        } else {
          // right matrices (indices 1 and 3)
          int rightCol = screenCol - matrixSize;
          lc->setColumn(matrixStartLeft + 1, rightCol, colData);
          lc->setColumn(matrixStartRight + 1, rightCol, colData);
        }
      }
      // precision speed control
      delayMicroseconds(scrollSpeedUs);
    }
    clearMatrices();
  }

  // renders game state to led matrix
  void drawGame(const Map& map, int matStart, bool hide, int cursorX, int cursorY, bool active) {
    unsigned long now = millis();
    bool blinkC = (now / blinkCursorMs) % 2 == 0;
    bool blinkM = (now / blinkMissMs) % 2 == 0;

    for (int x = 0; x < mapWidth; x++) {
      for (int y = 0; y < mapHeight; y++) {
        byte state = map.get(x, y);
        bool on = false;

        if (state == mapCellHit) on = true; // hit
        else if (state == mapCellMiss) on = blinkM; // miss
        else if (state == mapCellShip && !hide) on = true; // ship visible

        // cursor blink
        if (active && x == cursorX && y == cursorY) on = blinkC;

        if (y < matrixSize) lc->setLed(matStart, x, y, on);
        else lc->setLed(matStart + 1, x, y - matrixSize, on);
      }
    }
  }

  // renders ship placement phase
  void drawPlacement(const Map& map, const Ship& ship, int matStart, int cursorX, int cursorY) {
    // draw placed ships
    for (int x = 0; x < mapWidth; x++) {
      for (int y = 0; y < mapHeight; y++) {
        bool on = (map.get(x, y) == mapCellShip);
        if (y < matrixSize) lc->setLed(matStart, x, y, on);
        else lc->setLed(matStart + 1, x, y - matrixSize, on);
      }
    }
    // blink current ship
    if ((millis() / blinkPlaceMs) % 2 == 0) {
      int len = ship.getLength();
      if (ship.isHorizontal()) {
        for (int i = 0; i < len; i++) {
          int px = cursorX + i;
          if (px < mapWidth && cursorY < mapHeight) {
            if (cursorY < matrixSize) lc->setLed(matStart, px, cursorY, true);
            else lc->setLed(matStart + 1, px, cursorY - matrixSize, true);
          }
        }
      } else {
        for (int i = 0; i < len; i++) {
          int py = cursorY + i;
          if (cursorX < mapWidth && py < mapHeight) {
            if (py < matrixSize) lc->setLed(matStart, cursorX, py, true);
            else lc->setLed(matStart + 1, cursorX, py - matrixSize, true);
          }
        }
      }
    }
  }

  void drawPattern(const byte pattern[]) {
    for (int r = 0; r < matrixSize; r++) {
      byte rowData = pgm_read_byte(&pattern[r]);
      rowData = reverseBits(rowData);
      lc->setRow(matrixStartLeft, r, rowData);
      lc->setRow(matrixStartRight, r, rowData);
      lc->setRow(matrixStartLeft + 1, r, rowData);
      lc->setRow(matrixStartRight + 1, r, rowData);
    }
  }
};

class Input {
private:
  // structure for joystick debouncing
  struct JoyState {
    unsigned long lastTime, holdTime;
    int lastX, lastY;
  } joystick[2];
  unsigned long btnStartTime[2];
  bool btnCurrentState[2], pauseState;
public:
  Input() {
    memset(joystick, 0, sizeof(joystick));
    memset(btnCurrentState, 0, sizeof(btnCurrentState));
    pauseState = false;
  }

  // reads joystick axis with debouncing and auto-repeat
  void readJoy(int id, int pinX, int pinY, int& deltaX, int& deltaY, bool invertY) {
    deltaX = 0; deltaY = 0;

    // read raw axes
    int rawX = analogRead(pinX);
    int rawY = analogRead(pinY);

    int sx = 0, sy = 0;

    // x axis
    if (rawX < joyThresholdLow) sx = -1;
    else if (rawX > joyThresholdHigh) sx = joyDeltaPos;

    // y axis (invert logic for menus/map)
    if (rawY < joyThresholdLow) sy = invertY ? joyDeltaPos : -1;
    else if (rawY > joyThresholdHigh) sy = invertY ? -1 : joyDeltaPos;

    unsigned long now = millis();
    // new movement or different direction (immediate movement)
    if (sx != joystick[id].lastX || sy != joystick[id].lastY) {
      joystick[id].lastX = sx; joystick[id].lastY = sy;
      joystick[id].lastTime = now; joystick[id].holdTime = now;
      deltaX = sx; deltaY = sy;
      return;
    }
    // no movement
    if (sx == 0 && sy == 0) return;

    // auto-repeat after initial delay and repeat delay
    if (now - joystick[id].holdTime >= joyInitDelayMs) {
      if (now - joystick[id].lastTime >= joyRepeatDelayMs) {
        joystick[id].lastTime = now;
        deltaX = sx; deltaY = sy;
      }
    }
  }

  // checks button press (btnpressnone, btnpressshort, btnpresslong)
  int checkBtn(int id, int pin) {
    bool current = digitalRead(pin) == LOW;
    unsigned long now = millis();
    // initial press
    if (current && !btnCurrentState[id]) {
      btnCurrentState[id] = true;
      btnStartTime[id] = now;
      return btnPressNone;
    }
    // release
    if (!current && btnCurrentState[id]) {
      btnCurrentState[id] = false;
      return (now - btnStartTime[id] >= btnLongPressMs) ? btnPressLong : btnPressShort;
    }
    return btnPressNone;
  }

  // checks and toggles pause state
  int checkPause() {
    bool current = digitalRead(pinBtnPause) == LOW;
    // press
    if (current && !pauseState) {
      pauseState = true;
      return btnPressNone;
    }
    // release
    if (!current && pauseState) {
      pauseState = false;
      return btnPressShort;
    }
    return btnPressNone;
  }

  void reset(int id) {
    joystick[id].lastX = 0; joystick[id].lastY = 0;
    btnCurrentState[id] = false;
  }
};

class GameController {
private:
  GameState state, prevState;
  Display* display; Input* input; Audio* audio; Storage* storage;
  Map p1Ships, p1Attacks, p2Ships, p2Attacks;
  Fleet p1Fleet, p2Fleet;
  Sonar* sonar1; Sonar* sonar2;
  int cursor1X, cursor1Y, cursor2X, cursor2Y, turn;
  bool p1ViewOwn, p2ViewOwn, waitingForTransition;
  unsigned long transStartTime;
  GameState nextState;
  int menuSelect, settingSelect, howToPage;
  int p1Ammo, p2Ammo;
  unsigned long p1SonarTimer, p2SonarTimer;
  bool lcdDirty;
  char p1Name[nameLen], p2Name[nameLen];
  char inputName[nameLen];
  int inputCharIdx;
  bool isInputingP1;
  unsigned long winBeepTimer;
  int winBeepCountRemaining;
  int aboutPage;


  void startTransition(int ms, GameState next) {
    waitingForTransition = true;
    transStartTime = millis();
    nextState = next;
    lcdDirty = true;
  }

  bool checkTransition() {
    // keep transition duration
    if (waitingForTransition && millis() - transStartTime > transitionDelayMs) {
      waitingForTransition = false;
      state = nextState;
      lcdDirty = true;
      return true;
    }
    return false;
  }

  // logic for skipping invalid cells during placement
  int skipLogic(const Map& map, const Ship& ship, int value, int delta, bool horizMove) {
    int nextValue = value + delta;
    Ship tempShip = ship;
    int sx = horizMove ? nextValue : ship.getX();
    int sy = horizMove ? ship.getY() : nextValue;
    tempShip.setPosition(sx, sy);
    if (MapHelper::overlaps(map, tempShip)) {
      int limit = horizMove ? (ship.isHorizontal() ? mapWidth - ship.getLength() : mapWidth - 1)
                            : (ship.isHorizontal() ? mapHeight - 1 : mapHeight - ship.getLength());
      while (nextValue >= 0 && nextValue <= limit) {
        nextValue += delta;
        if (nextValue < 0 || nextValue > limit) break;
        tempShip.setPosition(horizMove ? nextValue : ship.getX(), horizMove ? ship.getY() : nextValue);
        if (!MapHelper::overlaps(map, tempShip)) return nextValue;
      }
      return value;
    }
    return nextValue;
  }

  // activates sonar
  void activateAirstrike(Map& enemyShips, Map& myAttacks, int cursorX, int cursorY) {
    display->clearLCD();
    display->printLCD(lcdColStart, lcdRowTop, (__FlashStringHelper*)strAirstrike);
    audio->beepBoom();

    for (int x = cursorX - areaDamageRadius; x <= cursorX + areaDamageRadius; x++) {
      for (int y = cursorY - areaDamageRadius; y <= cursorY + areaDamageRadius; y++) {
        if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight) {
          byte target = enemyShips.get(x, y);
          if (target == mapCellShip) {
            enemyShips.set(x, y, mapCellHit); // hit
            myAttacks.set(x, y, mapCellHit);
          } else if (target == mapCellEmpty) {
            myAttacks.set(x, y, mapCellMiss); // miss
          }
        }
      }
    }
    startTransition(airstrikeAnimMs, gamePlay);
    lcdDirty = true;
  }

  // logic for win beep
  void handleWinBeep() {
    if (winBeepCountRemaining > -1) {
      unsigned long now = millis();
      if (winBeepTimer == 0) {
        audio->beepWinTone();
        winBeepTimer = now;
      } else if (winBeepCountRemaining > 0 && now - winBeepTimer >= audioShortDuration + audioMedPause) {
        audio->beepWinTone();
        winBeepTimer = now;
        winBeepCountRemaining--;
      } else if (winBeepCountRemaining == 0) {
        // final tone
        audio->beepWinStart();
        winBeepCountRemaining = -1;
        winBeepTimer = 0;
      }
    }
  }

  void checkWin() {
    char msg[nameLen + 10];
    bool p1Win = p2Fleet.aliveCount(p2Ships) == 0;
    bool p2Win = p1Fleet.aliveCount(p1Ships) == 0;

    if (p1Win || p2Win) {
      const char* winnerName = p1Win ? p1Name : p2Name;

      storage->addWin(winnerName);
      sprintf_P(msg, fmtWinMsg, winnerName);
      state = gameWin;

      display->clearLCD();
      display->printLCD(lcdColStart, lcdRowTop, winnerName);
      display->printLCD(lcdNameWinCol, lcdRowTop, (__FlashStringHelper*)strWinsSuffix);

      winBeepCountRemaining = winBeepCount;
      winBeepTimer = 0;

      display->scrollText(msg);
    }
  }

public:
  GameController(Display* d, Input* i, Audio* a, Sonar* son1, Sonar* son2, Storage* s)
    : display(d), input(i), audio(a), sonar1(son1), sonar2(son2), storage(s), state(menuMain), menuSelect(0), lcdDirty(true),
      waitingForTransition(false), winBeepTimer(0), winBeepCountRemaining(-1),
      aboutPage(0) {}

  void init() {
    storage->init();
    storage->loadSettings(settingMatrixBri, settingLcdBri, settingSound);
    display->init();
    sonar1->init();
    sonar2->init();

    // start animation
    
    display->scrollText(strGameTitleScroll);
    showMainMenu();
  }

  void resetGame() {
    p1Ships.clear(); p2Ships.clear();
    p1Attacks.clear(); p2Attacks.clear();
    p1Fleet.reset(); p2Fleet.reset();
    cursor1X = 0; cursor1Y = 0; cursor2X = 0; cursor2Y = 0;
    p1ViewOwn = true; p2ViewOwn = true;
    turn = playerTurn1;
    p1Ammo = maxSonarAmmo; p2Ammo = maxSonarAmmo;
    display->clearMatrices();
    lcdDirty = true;
  }

  void showMainMenu() {
    display->clearLCD();
    display->clearMatrices();

    const byte* currentPattern = patternStartGame;

    if (menuSelect == menuStartIdx) {
      currentPattern = patternStartGame;
    } else if (menuSelect == menuSettingsIdx) {
      currentPattern = patternSettings;
    } else if (menuSelect == menuHowtoIdx) {
      currentPattern = patternHowto;
    } else if (menuSelect == menuLeaderIdx) {
      currentPattern = patternLeaderboard;
    } else if (menuSelect == menuAboutIdx) {
      currentPattern = patternAbout;
    }

    display->drawPattern(currentPattern);

    display->printLCD(lcdColStart, lcdRowTop, (__FlashStringHelper*)strTitleMain);
    const char* const items[] = { strMenuStart, strMenuSett, strMenuHowto, strMenuLeader, strMenuAbout };
    display->printLCD(lcdColStart, lcdRowBottom, (__FlashStringHelper*)strCursor);
    display->printLCD(lcdCursorOffset, lcdRowBottom, (__FlashStringHelper*)items[menuSelect]);
}

  void handleMainMenu() {
    {
    int dx, dy;
    input->readJoy(playerTurn1 - 1, pinJoy1X, pinJoy1Y, dx, dy, false);
    if (dy != 0) {
      menuSelect = (menuSelect + dy + mainMenuCount) % mainMenuCount;
      showMainMenu();
      audio->beepNav();
    }
    if (input->checkBtn(playerTurn1 - 1, pinJoy1Btn) == btnPressShort) {
      audio->beepOk();
      if (menuSelect == menuStartIdx) {
        state = menuSelectP1;
        menuSelect = 0;
        isInputingP1 = true;
        lcdDirty = true;
      } else if (menuSelect == menuSettingsIdx) {
        state = menuSettings;
        settingSelect = 0;
        showSettings();
        display->lightAllMatrix();
      } else if (menuSelect == menuHowtoIdx) {
        state = menuHowto;
        howToPage = howToPage1;
        showHowTo();
      } else if (menuSelect == menuLeaderIdx) {
        state = menuLeaderboard;
        storage->sortLeaderboard();
        menuSelect = 0;
        lcdDirty = true;
      } else if (menuSelect == menuAboutIdx) {
        state = menuAbout;
        aboutPage = aboutPageTitle; // set initial page to Title
        showAbout();
      }
    }
  }
}
  void handlePlayerSelect() {
    int activeId = isInputingP1 ? playerTurn1 - 1 : playerTurn2 - 1;
    int activeX = isInputingP1 ? pinJoy1X : pinJoy2X;
    int activeY = isInputingP1 ? pinJoy1Y : pinJoy2Y;
    int activeBtn = isInputingP1 ? pinJoy1Btn : pinJoy2Btn;

    if (lcdDirty) {
      display->clearLCD();
      display->printLCD(lcdColStart, lcdRowTop, isInputingP1 ? (__FlashStringHelper*)strSelP1 : (__FlashStringHelper*)strSelP2);
      if (menuSelect == 0) display->printLCD(lcdColStart, lcdRowBottom, (__FlashStringHelper*)strNewName);
      else {
        display->printLCD(lcdColStart, lcdRowBottom, (__FlashStringHelper*)strCursorSpace);
        display->printLCD(lcdCursorOffset, lcdRowBottom, storage->players[menuSelect - 1].name);
      }
      lcdDirty = false;
    }
    int dx, dy;
    input->readJoy(activeId, activeX, activeY, dx, dy, false);
    if (dy != 0) {
      int maxItems = 1 + storage->playerCount;
      menuSelect = (menuSelect + dy + maxItems) % maxItems;
      lcdDirty = true;
      audio->beepNav();
    }
    if (input->checkBtn(activeId, activeBtn) == btnPressShort) {
      audio->beepOk();
      if (menuSelect == 0) {
        state = menuNameInput;
        strcpy_P(inputName, strDefaultName);
        inputCharIdx = 0;
        input->reset(activeId);
        lcdDirty = true;
      } else {
        const char* selectedName = storage->players[menuSelect - 1].name;

        if (isInputingP1) {
          strcpy(p1Name, selectedName);
          isInputingP1 = false;
          state = menuSelectP2;
          menuSelect = 0;
          lcdDirty = true;
          input->reset(playerTurn1 - 1);
        } else {
          if (strcmp(p1Name, selectedName) == 0) {
            audio->beepErr();
            display->clearLCD();
            display->printLCD(lcdColStart, lcdRowTop, F("Error: same name!"));
            display->printLCD(lcdColStart, lcdRowBottom, selectedName);
            startTransition(transitionDelayMs, menuSelectP2);
            lcdDirty = true;
            return;
          }
          strcpy(p2Name, selectedName);
          resetGame();
          state = gamePlaceShips;
          display->clearLCD();
          input->reset(playerTurn2 - 1);
        }
      }
    }
  }

  void handleNameInput() {
    int activeId = isInputingP1 ? playerTurn1 - 1 : playerTurn2 - 1;
    int activeX = isInputingP1 ? pinJoy1X : pinJoy2X;
    int activeY = isInputingP1 ? pinJoy1Y : pinJoy2Y;
    int activeBtn = isInputingP1 ? pinJoy1Btn : pinJoy2Btn;

    static unsigned long lastInputTime = 0;
    static unsigned long lastBlinkRefresh = 0;

    unsigned long now = millis();
    if (now - lastBlinkRefresh > blinkInputMs) {
      lcdDirty = true;
      lastBlinkRefresh = now;
    }
    bool showUnderscore = (now / blinkInputMs) % 2 == 0;
    if (now - lastInputTime < blinkInputMs * 2) showUnderscore = false;

    if (lcdDirty) {
      display->clearLCD();
      display->printLCD(lcdColStart, lcdRowTop, isInputingP1 ? (__FlashStringHelper*)strP1NameLbl : (__FlashStringHelper*)strP2NameLbl);
      display->printLCD(lcdNameInputCol, lcdRowBottom, inputName);
      if (showUnderscore) display->printLCD(lcdNameInputCol + inputCharIdx, lcdRowBottom, (__FlashStringHelper*)strUnderscore);
      lcdDirty = false;
    }

    int dx, dy;
    input->readJoy(activeId, activeX, activeY, dx, dy, false);

    // nav left-right
    if (dx != 0) {
      inputCharIdx = constrain(inputCharIdx + dx, 0, nameLenChars - 1);
      lcdDirty = true;
      audio->beepNav();
      lastInputTime = now;
    }
    // change letter
    if (dy != 0) {
      char c = inputName[inputCharIdx];
      c -= dy;
      if (c > 'Z') c = 'A';
      if (c < 'A') c = 'Z';
      inputName[inputCharIdx] = c;
      lcdDirty = true;
      // emulate character input delay
      lastInputTime = now;
      lastBlinkRefresh = now;
    }
    // confirm name
    if (input->checkBtn(activeId, activeBtn) == btnPressShort) {
      audio->beepOk();

      if (!isInputingP1) {
        if (strcmp(p1Name, inputName) == 0) {
          audio->beepErr();
          display->clearLCD();
          display->printLCD(lcdColStart, lcdRowBottom, inputName);
          startTransition(transitionDelayMs, menuNameInput);
          inputCharIdx = 0;
          lcdDirty = true;
          return;
        }
      }

      storage->savePlayer(inputName);
      if (isInputingP1) {
        strcpy(p1Name, inputName);
        isInputingP1 = false;
        state = menuSelectP2;
        menuSelect = 0;
        lcdDirty = true;
        input->reset(playerTurn1 - 1);
      } else {
        strcpy(p2Name, inputName);
        resetGame();
        state = gamePlaceShips;
        display->clearLCD();
        input->reset(playerTurn2 - 1);
      }
    }
  }

  void handleLeaderboard() {
    if (lcdDirty) {
      display->clearLCD();
      if (storage->playerCount == 0) display->printLCD(lcdColStart, lcdRowTop, (__FlashStringHelper*)strNoRecords);
      else {
        PlayerRecord p = storage->players[menuSelect];
        char buf[lcdCols + 1];
        sprintf_P(buf, fmtLeaderboard, menuSelect + 1, p.name, p.wins);
        display->printLCD(lcdColStart, lcdRowTop, (__FlashStringHelper*)strTitleLeader);
        display->printLCD(lcdColStart, lcdRowBottom, buf);
      }
      lcdDirty = false;
    }
    int dx, dy;
    input->readJoy(playerTurn1 - 1, pinJoy1X, pinJoy1Y, dx, dy, false);
    if (dy != 0 && storage->playerCount > 0) {
      menuSelect = (menuSelect + dy + storage->playerCount) % storage->playerCount;
      lcdDirty = true;
      audio->beepNav();
    }
    if (input->checkBtn(playerTurn1 - 1, pinJoy1Btn) == btnPressShort) {
      state = menuMain;
      menuSelect = 0;
      showMainMenu();
      audio->beepOk();
    }
  }

  void showSettings() {
    display->clearLCD();
    if (settingSelect == settingsMatrixBriIdx) {
      display->printLCD(lcdColStart, lcdRowTop, (__FlashStringHelper*)strSettMatBri);
      char buf[lcdCols];
      sprintf_P(buf, fmtSettVal, settingMatrixBri);
      display->printLCD(lcdColStart, lcdRowBottom, buf);
    } else if (settingSelect == settingsLcdBriIdx) {
      display->printLCD(lcdColStart, lcdRowTop, (__FlashStringHelper*)strSettLcdBri);
      char buf[lcdCols];
      sprintf_P(buf, fmtSettVal, settingLcdBri);
      display->printLCD(lcdColStart, lcdRowBottom, buf);
    } else if (settingSelect == settingsSoundIdx) {
      display->printLCD(lcdColStart, lcdRowTop, (__FlashStringHelper*)strSettSound);
      display->printLCD(lcdColStart, lcdRowBottom, settingSound ? (__FlashStringHelper*)strSettOn : (__FlashStringHelper*)strSettOff);
    } else if (settingSelect == settingsResetIdx) {
      display->printLCD(lcdColStart, lcdRowTop, (__FlashStringHelper*)strSettReset);
      display->printLCD(lcdColStart, lcdRowBottom, (__FlashStringHelper*)strPressBtn);
    } else if (settingSelect == settingsBackIdx) {
      display->printLCD(lcdColStart, lcdRowTop, (__FlashStringHelper*)strSettBack);
      display->printLCD(lcdColStart, lcdRowBottom, (__FlashStringHelper*)strPressBtn);
    }
  }

  void handleSettings() {
    if (waitingForTransition) {
      checkTransition();
      return;
    }
    
    int dx, dy;
    input->readJoy(playerTurn1 - 1, pinJoy1X, pinJoy1Y, dx, dy, false);
    if (dy != 0) {
      settingSelect = (settingSelect + dy + settingsMenuCount) % settingsMenuCount;
      showSettings();
      audio->beepNav();
    }
    if (dx != 0) {
      if (settingSelect == settingsMatrixBriIdx) {
        settingMatrixBri = constrain(settingMatrixBri + dx, 0, maxMatrixBri);
        display->applyMatrixBrightness();
        display->lightAllMatrix();
      } else if (settingSelect == settingsLcdBriIdx) {
        settingLcdBri = constrain(settingLcdBri + dx, 0, maxLcdBri);
        display->applyLcdBacklight();
      } else if (settingSelect == settingsSoundIdx) {
        settingSound = !settingSound;
        if (settingSound) audio->startMusic();
        else audio->stopMusic();
      }
      showSettings();
      if (settingSelect != settingsSoundIdx) audio->beepNav();
    }
    
    if (input->checkBtn(playerTurn1 - 1, pinJoy1Btn) == btnPressShort) {
      if (settingSelect == settingsBackIdx) {
        storage->saveSettings(settingMatrixBri, settingLcdBri, settingSound);
        display->clearMatrices();
        state = menuMain;
        showMainMenu();
        audio->beepOk();
      } else if (settingSelect == settingsResetIdx) {
        storage->resetLeaderboard();
        display->clearLCD();
        display->printLCD(lcdColStart, lcdRowTop, F("Leaderboard"));
        display->printLCD(lcdColStart, lcdRowBottom, F("Reset OK!"));
        startTransition(transitionDelayMs, menuSettings); 
        settingSelect = settingsBackIdx;
        audio->beepBoom();
      }
    }
  }

  void showHowTo() {
    display->clearLCD();
    if (howToPage == howToPage1) {
      display->printLCD(lcdColStart, lcdRowTop, (__FlashStringHelper*)strHowto1A);
      display->printLCD(lcdColStart, lcdRowBottom, (__FlashStringHelper*)strHowto1B);
    } else if (howToPage == howToPage2) {
      display->printLCD(lcdColStart, lcdRowTop, (__FlashStringHelper*)strHowto2A);
      display->printLCD(lcdColStart, lcdRowBottom, (__FlashStringHelper*)strHowto2B);
    } else if (howToPage == howToPage3) {
      display->printLCD(lcdColStart, lcdRowTop, (__FlashStringHelper*)strHowto3A);
      display->printLCD(lcdColStart, lcdRowBottom, (__FlashStringHelper*)strHowto3B);
    } else if (howToPage == howToPage4) {
      display->printLCD(lcdColStart, lcdRowTop, (__FlashStringHelper*)strHowto4A);
      display->printLCD(lcdColStart, lcdRowBottom, (__FlashStringHelper*)strHowto4B);
    }
  }

  void handleHowTo() {
    if (input->checkBtn(playerTurn1 - 1, pinJoy1Btn) == btnPressShort) {
      howToPage++;
      if (howToPage >= howToPagesCount) {
        state = menuMain;
        showMainMenu();
      } else showHowTo();
      audio->beepOk();
    }
  }

  void showAbout() {
    display->clearLCD();
    
    if (aboutPage == aboutPageTitle) {
      display->printLCD(lcdColStart, lcdRowTop, (__FlashStringHelper*)strAboutTitle);
      display->printLCD(lcdColStart, lcdRowBottom, (__FlashStringHelper*)strAboutSubtitle);

    } else if (aboutPage == aboutPageAuthors1) {
      display->printLCD(lcdColStart, lcdRowTop, (__FlashStringHelper*)strAboutAuthName);
      display->printLCD(lcdColStart, lcdRowBottom, (__FlashStringHelper*)strAboutAuthVal1);

    } else if (aboutPage == aboutPageAuthors2) {
      display->printLCD(lcdColStart, lcdRowTop, (__FlashStringHelper*)strAboutAuthName);
      display->printLCD(lcdColStart, lcdRowBottom, (__FlashStringHelper*)strAboutAuthVal2);

    } else if (aboutPage == aboutPageGit1) {
      display->printLCD(lcdColStart, lcdRowTop, (__FlashStringHelper*)strAboutGitName);
      display->printLCD(lcdColStart, lcdRowBottom, (__FlashStringHelper*)strAboutGitVal1);

    } else if (aboutPage == aboutPageGit2) {
      display->printLCD(lcdColStart, lcdRowTop, (__FlashStringHelper*)strAboutGitName);
      display->printLCD(lcdColStart, lcdRowBottom, (__FlashStringHelper*)strAboutGitVal2);
    }
  }
  
  void handleAbout() {
    if (input->checkBtn(playerTurn1 - 1, pinJoy1Btn) == btnPressShort) {
      audio->beepOk();
      
      aboutPage++;
      
      if (aboutPage >= aboutPageCount) {
        state = menuMain;
        menuSelect = 0;
        showMainMenu();
      } else {
        showAbout();  
      }
    }
  }

  void showPauseMenu() {
    display->clearLCD();
    display->printLCD(lcdColStart, lcdRowTop, (__FlashStringHelper*)strTitlePause);
    const char* const items[] = { strPauseResume, strPauseRestart, strPauseExit };
    display->printLCD(lcdColStart, lcdRowBottom, (__FlashStringHelper*)strCursor);
    display->printLCD(lcdCursorOffset, lcdRowBottom, (__FlashStringHelper*)items[menuSelect]);
  }

  void handlePause() {
    int dx, dy;
    input->readJoy(playerTurn1 - 1, pinJoy1X, pinJoy1Y, dx, dy, false);
    if (dy != 0) {
      menuSelect = (menuSelect + dy + pauseMenuCount) % pauseMenuCount;
      showPauseMenu();
      audio->beepNav();
    }
    if (input->checkBtn(playerTurn1 - 1, pinJoy1Btn) == btnPressShort) {
      audio->beepOk();
      if (menuSelect == pauseMenuResumeIdx) state = prevState;
      else if (menuSelect == pauseMenuRestartIdx) {
        resetGame();
        state = gamePlaceShips;
      } else if (menuSelect == pauseMenuExitIdx) {
        state = menuMain;
        menuSelect = 0;
        showMainMenu();
      }
    }
  }

  void handlePlacement() {
    if (waitingForTransition) {
      checkTransition();
      return;
    }
    bool p1Done = p1Fleet.done();
    bool p2Done = p2Fleet.done();
    if (p1Done && p2Done) {
      display->clearLCD();
      display->printLCD(lcdColStart, lcdRowTop, (__FlashStringHelper*)strBothReady);
      display->printLCD(lcdColStart, lcdRowBottom, (__FlashStringHelper*)strBattleStart);
      startTransition(transitionDelayMs, gamePlay);
      return;
    }

    // player 1 placement logic
    if (!p1Done) {
      Ship* ship = p1Fleet.currentShip();
      int dx, dy;
      // read with pins inverted, then manual dy inversion
      input->readJoy(playerTurn1 - 1, pinJoy1Y, pinJoy1X, dx, dy, true);
      dy = -dy; // manual axis inversion

      if (dx != 0 || dy != 0) {
        if (dx != 0) cursor1X = skipLogic(p1Ships, *ship, cursor1X, dx, true);
        if (dy != 0) cursor1Y = skipLogic(p1Ships, *ship, cursor1Y, dy, false);
        cursor1X = constrain(cursor1X, 0, ship->isHorizontal() ? mapWidth - ship->getLength() : mapWidth - 1);
        cursor1Y = constrain(cursor1Y, 0, ship->isHorizontal() ? mapHeight - 1 : mapHeight - ship->getLength());
        ship->setPosition(cursor1X, cursor1Y);
        lcdDirty = true;
      }
      int btn = input->checkBtn(playerTurn1 - 1, pinJoy1Btn);
      if (btn == btnPressShort) {
        ship->toggleOrientation();
        cursor1X = constrain(cursor1X, 0, ship->isHorizontal() ? mapWidth - ship->getLength() : mapWidth - 1);
        cursor1Y = constrain(cursor1Y, 0, ship->isHorizontal() ? mapHeight - 1 : mapHeight - ship->getLength());
        ship->setPosition(cursor1X, cursor1Y);
        audio->beepNav();
        lcdDirty = true;
      } else if (btn == btnPressLong) {
        if (MapHelper::canPlace(p1Ships, *ship)) {
          MapHelper::place(p1Ships, *ship);
          ship->setPlaced(true);
          p1Fleet.next();
          audio->beepOk();
          cursor1X = 0; cursor1Y = 0;
          lcdDirty = true;
        } else audio->beepErr();
      }
      display->drawPlacement(p1Ships, *ship, matrixStartLeft, cursor1X, cursor1Y);
    } else display->drawGame(p1Ships, matrixStartLeft, false, -1, -1, false);

    // player 2 placement logic
    if (!p2Done) {
      Ship* ship = p2Fleet.currentShip();
      int dx, dy;
      input->readJoy(playerTurn2 - 1, pinJoy2Y, pinJoy2X, dx, dy, true);
      dy = -dy; // manual axis inversion

      if (dx != 0 || dy != 0) {
        if (dx != 0) cursor2X = skipLogic(p2Ships, *ship, cursor2X, dx, true);
        if (dy != 0) cursor2Y = skipLogic(p2Ships, *ship, cursor2Y, dy, false);
        cursor2X = constrain(cursor2X, 0, ship->isHorizontal() ? mapWidth - ship->getLength() : mapWidth - 1);
        cursor2Y = constrain(cursor2Y, 0, ship->isHorizontal() ? mapHeight - 1 : mapHeight - ship->getLength());
        ship->setPosition(cursor2X, cursor2Y);
        lcdDirty = true;
      }
      int btn = input->checkBtn(playerTurn2 - 1, pinJoy2Btn);
      if (btn == btnPressShort) {
        ship->toggleOrientation();
        cursor2X = constrain(cursor2X, 0, ship->isHorizontal() ? mapWidth - ship->getLength() : mapWidth - 1);
        cursor2Y = constrain(cursor2Y, 0, ship->isHorizontal() ? mapHeight - 1 : mapHeight - ship->getLength());
        ship->setPosition(cursor2X, cursor2Y);
        audio->beepNav();
        lcdDirty = true;
      } else if (btn == btnPressLong) {
        if (MapHelper::canPlace(p2Ships, *ship)) {
          MapHelper::place(p2Ships, *ship);
          ship->setPlaced(true);
          p2Fleet.next();
          audio->beepOk();
          cursor2X = 0; cursor2Y = 0;
          lcdDirty = true;
        } else audio->beepErr();
      }
      display->drawPlacement(p2Ships, *ship, matrixStartRight, cursor2X, cursor2Y);
    } else display->drawGame(p2Ships, matrixStartRight, false, -1, -1, false);

    if (lcdDirty) {
      lcdDirty = false;
      char buffer1[lcdCols + 1], buffer2[lcdCols + 1];
      if (!p1Done) sprintf_P(buffer1, fmtP1Ship, p1Fleet.currentShip()->getLength()); else strcpy_P(buffer1, fmtP1Ready);
      if (!p2Done) sprintf_P(buffer2, fmtP2Ship, p2Fleet.currentShip()->getLength()); else strcpy_P(buffer2, fmtP2Ready);
      display->printLCD(lcdColStart, lcdRowTop, buffer1);
      display->printLCD(lcdColStart, lcdRowBottom, buffer2);
    }
  }

  void handleGameplay() {
    if (waitingForTransition) {
      checkTransition();
      return;
    }

    // player 1 movement
    int dx1, dy1;
    input->readJoy(playerTurn1 - 1, pinJoy1Y, pinJoy1X, dx1, dy1, true);
    dy1 = -dy1; // manual axis inversion

    if (dx1 != 0 || dy1 != 0) {
      cursor1X = constrain(cursor1X + dx1, 0, mapWidth - 1);
      cursor1Y += dy1;
      if (p1ViewOwn) {
        if (cursor1Y >= mapHeight) {
          p1ViewOwn = false; cursor1Y = 0; audio->beepNav(); display->clearMatrices(); lcdDirty = true;
        } else if (cursor1Y < 0) cursor1Y = 0;
      } else {
        if (cursor1Y < 0) {
          p1ViewOwn = true; cursor1Y = mapHeight - 1; audio->beepNav(); display->clearMatrices(); lcdDirty = true;
        } else if (cursor1Y >= mapHeight) cursor1Y = mapHeight - 1;
      }
    }

    // player 2 movement
    int dx2, dy2;
    input->readJoy(playerTurn2 - 1, pinJoy2Y, pinJoy2X, dx2, dy2, true);
    dy2 = -dy2; // manual axis inversion

    if (dx2 != 0 || dy2 != 0) {
      cursor2X = constrain(cursor2X + dx2, 0, mapWidth - 1);
      cursor2Y += dy2;
      if (p2ViewOwn) {
        if (cursor2Y >= mapHeight) {
          p2ViewOwn = false; cursor2Y = 0; audio->beepNav(); display->clearMatrices(); lcdDirty = true;
        } else if (cursor2Y < 0) cursor2Y = 0;
      } else {
        if (cursor2Y < 0) {
          p2ViewOwn = true; cursor2Y = mapHeight - 1; audio->beepNav(); display->clearMatrices(); lcdDirty = true;
        } else if (cursor2Y >= mapHeight) cursor2Y = mapHeight - 1;
      }
    }

    // draw both screens
    if (p1ViewOwn) display->drawGame(p1Ships, matrixStartLeft, false, cursor1X, cursor1Y, true);
    else display->drawGame(p1Attacks, matrixStartLeft, true, cursor1X, cursor1Y, true);

    if (p2ViewOwn) display->drawGame(p2Ships, matrixStartRight, false, cursor2X, cursor2Y, true);
    else display->drawGame(p2Attacks, matrixStartRight, true, cursor2X, cursor2Y, true);

    // airstrike p1 (sonar triggered)
    if (turn == playerTurn1 && !p1ViewOwn && p1Ammo > 0) {
      int dist1 = sonar1->getDistance();
      if (dist1 < sonarMaxDist && dist1 > 0) {
        if (p1SonarTimer == 0) p1SonarTimer = millis();
        if (millis() - p1SonarTimer > sonarTriggerMs) {
          p1Ammo--;
          activateAirstrike(p2Ships, p1Attacks, cursor1X, cursor1Y);
          turn = playerTurn2; checkWin(); p1SonarTimer = 0; lcdDirty = true;
        }
      } else p1SonarTimer = 0;
    }

    // airstrike p2 (sonar triggered)
    if (turn == playerTurn2 && !p2ViewOwn && p2Ammo > 0) {
      int dist2 = sonar2->getDistance();
      if (dist2 < sonarMaxDist && dist2 > 0) {
        if (p2SonarTimer == 0) p2SonarTimer = millis();
        if (millis() - p2SonarTimer > sonarTriggerMs) {
          p2Ammo--;
          activateAirstrike(p1Ships, p2Attacks, cursor2X, cursor2Y);
          turn = playerTurn1; checkWin(); p2SonarTimer = 0; lcdDirty = true;
        }
      } else p2SonarTimer = 0;
    }

    // attack p1 (button)
    if (input->checkBtn(playerTurn1 - 1, pinJoy1Btn) == btnPressShort && turn == playerTurn1 && !p1ViewOwn) {
      if (p1Attacks.get(cursor1X, cursor1Y) == mapCellEmpty) {
        if (p2Ships.get(cursor1X, cursor1Y) == mapCellShip) {
          p1Attacks.set(cursor1X, cursor1Y, mapCellHit); p2Ships.set(cursor1X, cursor1Y, mapCellHit); audio->beepHit();
        } else {
          p1Attacks.set(cursor1X, cursor1Y, mapCellMiss); audio->beepMiss(); turn = playerTurn2; lcdDirty = true;
        }
        lcdDirty = true; checkWin();
      } else audio->beepErr();
    }

    // attack p2 (button)
    if (input->checkBtn(playerTurn2 - 1, pinJoy2Btn) == btnPressShort && turn == playerTurn2 && !p2ViewOwn) {
      if (p2Attacks.get(cursor2X, cursor2Y) == mapCellEmpty) {
        if (p1Ships.get(cursor2X, cursor2Y) == mapCellShip) {
          p2Attacks.set(cursor2X, cursor2Y, mapCellHit); p1Ships.set(cursor2X, cursor2Y, mapCellHit); audio->beepHit();
        } else {
          p2Attacks.set(cursor2X, cursor2Y, mapCellMiss); audio->beepMiss(); turn = playerTurn1; lcdDirty = true;
        }
        lcdDirty = true; checkWin();
      } else audio->beepErr();
    }

    if (lcdDirty) {
      lcdDirty = false;
      char buffer1[lcdCols + 1], buffer2[lcdCols + 1];
      const char* currentPlayerName = (turn == playerTurn1 ? p1Name : p2Name);
      int currentAmmo = (turn == playerTurn1 ? p1Ammo : p2Ammo);
      int meAlive = (turn == playerTurn1) ? p1Fleet.aliveCount(p1Ships) : p2Fleet.aliveCount(p2Ships);
      int enemyTargets = (turn == playerTurn1) ? p2Fleet.aliveCount(p2Ships) : p1Fleet.aliveCount(p1Ships);

      sprintf_P(buffer1, fmtTurnStatus, currentPlayerName, currentAmmo);
      sprintf_P(buffer2, fmtStats, meAlive, enemyTargets);
      display->printLCD(lcdColStart, lcdRowTop, buffer1);
      display->printLCD(lcdColStart, lcdRowBottom, buffer2);
    }
  }

  void update() {
    audio->update();
    handleWinBeep();

    if (input->checkPause() == btnPressShort && state != gameWin) {
      prevState = state;
      state = menuPause;
      menuSelect = 0;
      showPauseMenu();
      audio->beepNav();
    }
    bool inMenu = (state != gamePlaceShips && state != gamePlay && state != gameWin);
    if (inMenu && !audio->isMusicPlaying() && settingSound) audio->startMusic();
    else if (!inMenu && audio->isMusicPlaying()) audio->stopMusic();

    switch (state) {
      case menuMain: handleMainMenu(); break;
      case menuSettings: handleSettings(); break;
      case menuHowto: handleHowTo(); break;
      case menuAbout: handleAbout(); break;
      case menuLeaderboard: handleLeaderboard(); break;
      case menuSelectP1:
      case menuSelectP2: handlePlayerSelect(); break;
      case menuNameInput: handleNameInput(); break;
      case menuPause: handlePause(); break;
      case gamePlaceShips: handlePlacement(); break;
      case gamePlay: handleGameplay(); break;
      case gameWin:
        if (input->checkBtn(playerTurn1 - 1, pinJoy1Btn) == btnPressShort) {
          resetGame();
          state = menuMain;
          showMainMenu();
        }
        break;
      default:
        // unknown state, revert to main menu
        break;
    }
  }
};


// initialization and loop

LedControl lc(pinDin, pinClk, pinCs, numMatrices);
LiquidCrystal_I2C lcd(lcdI2cAddr, lcdCols, lcdRows);
Display displayController(&lc, &lcd);
Input inputController;
Audio audioController(pinBuzzer);
Sonar sonar1(pinP1Trig, pinP1Echo);
Sonar sonar2(pinP2Trig, pinP2Echo);
Storage storageController;
GameController gameController(&displayController, &inputController, &audioController, &sonar1, &sonar2, &storageController);

void setup() {
  Wire.begin();
  Wire.setClock(400000); // i2c speed setting
  pinMode(pinJoy1Btn, INPUT_PULLUP);
  pinMode(pinJoy2Btn, INPUT_PULLUP);
  pinMode(pinBtnPause, INPUT_PULLUP);
  audioController.begin(); // buzzer pin initialization
  gameController.init();
}

void loop() {
  gameController.update();
}