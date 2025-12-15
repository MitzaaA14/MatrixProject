## Matrix Draw Project Introduction to Robotics (2025-2026)

 The laboratory matrix project created for the Introduction to Robotics course at the University of Bucharest's Faculty of Mathematics and Informatics in the Computer Science domain during the 2025–2026 academic year is included in this repository.  Code, implementation, explanations, and extra resources (pictures, test results, videos) are all included in each checkpoint.


<details> 
<summary><h2> Checkpoint #1: LED Matrix Drawing System</h2></summary>

Matrix Draw is an LED drawing application that allows users to create pixel art on an 8x8 LED matrix using:
- The joystick controller
- A menu system that is shown on an LCD screen
- several LED states (fixed, slow blink, fast blink)
- A buzzer that provides audio feedback

# Task Requirements:
Design and implement an interactive matrix project with the following features:

- Working 8x8 LED matrix controlled via joystick
- LCD screen with menu navigation - Controlled by joystick when the player is not curently in a game
- One interactive mode: "Start Game" - Allows player to draw on led matrix
- External input that directly influences matrix output - Joystick
- Multiple LED states with visual feedback - First button press LED FIXED , second button press LED SLOW BLINKING, third button press LED FAST BLINKING
- Audio feedback for user action

# Components:

- Arduino Uno
- 8x8 LED Matrix
- MAX7219
- LCD Display : Menu display and information
- Joystick Module : User input (menu navigation and drawing on matrix)
- Buzzer : Audio feedback
- Resistors : 220Ω for LCD and 100kΩ for matrix MAX7219	
- LCD connections and contrast
- Wires : Circuit connections

# Pin Configuration:

LED Matrix:

-  DIN->Pin 12
-  CLOCK->Pin 11
-  LOAD->Pin 10

LCD Display:

-  RS->Pin 9
-  Enable->Pin 8
-  D4->Pin 7
-  D5->Pin 6
-  D6->Pin 4
-  D7->Pin 2
-  Contrast (PWM)->Pin 5

Joystick:
-  VRx (X- axis)->A0
-  VRy (Y- axis)->A1
-  Button->A2

-  Buzzer->Pin 3

# System Features:

Main Menu:

Three options for the menu:

1. Start Game - Drawing mode where you can create pixel art
2. Clear Draw - Clears the entire drawing
3. About: Details about the project

Navigation:

1. Joystick Up/Down - Navigate between menu items
2. Press the button to choose the current menu item.
3. Visual cursor indicator blinks to show selected option

Draw Mode:

- The system goes into interactive drawing mode if the Start Game option is chosen:

1. Joystick->Cursor movement on an 8x8 matrix
2. Press the short button to cycle through the LED states at the cursor position.
3. Long Press (3 seconds)-> Exit to menu

LED Conditions:

1. OFF: The LED is not on.
2. FIXED: The LED is always on 
3. SLOW: The LED blinks slowly
4. FAST: The LED blinks quickly.

Display Information (in Drawing mode):
- "DRAW MODE"
- Current cursor position (X, Y) and LED state

Audio Feedback:
The buzzer provide distinct tones for different actions:

-  Menu navigation
-  Menu selection
-  Cursor movement
-  Drawing cleared
-  Exit draw mode

Visual Feedback:
-  Cursor Blinking - Indicates current position in menu and draw mode
-  LCD Contrast - Adjusts for optimal visibility
-  Matrix Blinking - LED states for more crative draw

System States:

1. Splash Screen
-  Displays message on startup for 1 seconds
-  Automatically transitions to main menu

2. Main Menu
-  Shows menu options with blinking cursor indicator
-  Current drawings are shown on the matrix; 
-  Joystick navigation

3. Draw Mode
-  Interactive drawing interface
-  Cursor position and state display
-  LED animations

4. Message Display
-  Temporary "Drawing cleared!" and "About" screens that return to the menu after a predetermined amount of time

Code Architecture:
Key Functions:
-  setup() : Initializes every component and shows the splash screen
-  loop() : Main loop that manages timing and states
-  showMenu() : Display and update menu on LCD
-  menuLoop() : Handle menu navigation and selection
-  startGame() : Set up the game's draw mode
-  gameLoop() : Manage cursor movement and drawing interactions
-  updateMatrix() : Update the LED matrix according to the display's current
-  displayDrawing() : Show drawing while in menu
-  updateDisplay() : Update the LCD with cursor information
-  clearAll() : Turn off all of the LEDs
-  showAbout() : Display project information

Non-Blocking Timing:
To prevent blocking delays, millis() is used in all timing operations:

-  Blink timers for LED states
-  Cursor blink timer for menu and draw mode
-  Timers for joystick and button inputs
-  Timers for splash and message screens


# Physical Implementation Checkpoint #1

<img width="579" height="760" alt="Screenshot 2025-12-01 at 17 08 52" src="https://github.com/user-attachments/assets/8477afb4-51f0-4208-9ad7-c85690e8ece6" />
<img width="973" height="736" alt="Screenshot 2025-12-01 at 17 09 03" src="https://github.com/user-attachments/assets/327b9e30-196e-42ad-b16f-e699a9f0a0e0" />
<img width="973" height="736" alt="Screenshot 2025-12-01 at 17 09 09" src="https://github.com/user-attachments/assets/43beabde-8739-4edf-8c3e-44b6f6cf6b82" />

# Video Demonstration

 https://youtu.be/-X2TxVpON-k?si=WHG6nfVh5Rvq6QeS

</details>

<details>

<summary><h2> Checkpoint #2: Battleships</h2></summary>

### Task Requirements:

- Create a matrix project with sensor and joystick interaction.
- Build a working LCD Screen navigated via joystick.
- Checkpoint 2 Goal (as declared): Implement map logic and ship placement system for the Battleship game.

### Project Overview

This is a two-player implementation of the classic Battleship game using Arduino hardware. Each player uses a joystick to place their 5 ships on a virtual 8x16 LED matrix grid and will later attack the opponent's fleet.

Team Members: https://github.com/filip407 & https://github.com/MitzaaA14

### Components Used

- Arduino Uno
- 4x 8x8 LED Matrix with MAX7219 driver (cascaded horizontally)
- 16x2 LCD Display with I2C interface
- Joystick module
- Buzzer
- Button
- 220Ω resistor
- 100kΩ resistors for MAX7219 drivers
- 4x 104pF Capacitor
- 4x 10μF Capacitor
- Wires and breadboard

Pin Configuration:

| Component / Function | Signal / Pin | Arduino Pin |
|-------------------------------|----------------------|--------------|
| LED Matrices (MAX7219) | DIN | D12 |
| | CLK | D11 |
| | CS (LOAD) | D10 |
| LCD Display (I2C - 0x27) | SDA | A4 |
| | SCL | A5 |
| Other Components | Buzzer | D6 |
| | Pause Button | D7 |
| Player 1 Joystick | X-axis (VRX) | A0 |
| | Y-axis (VRY) | A1 |
| | Button (SW) | D2 |
| Player 2 Joystick | X-axis (VRX) | A2 |
| | Y-axis (VRY) | A3 |
| | Button (SW) | D3 |

### Game Concept & Display Layout:

The Battleship Grid System

Each player will have two maps:

1. Ships Map (8x16): Shows your own ships are and where the enemy has attacked you
2. Attack Map (8x16): Shows where they have attacked the enemy (hits and misses).

Physical Display Configuration

Each player has 2 LED 8x8 matrices placed horizontally side by side, creating a 16x8 visible display. However, each map will be 8 columns x 16 rows logically. Since the physical display only shows 8 rows at once, players will be able to scroll vertically using the joystick to see all 16 rows.

### System Features:

Checkpoint 2 Implementation Status

Checkpoint 1 focused on the basic hardware and project direction. Checkpoint 2 aimed to create the map logic and ship placement. Here's what has been completed:

1. Ship Placement System

5 Ships:

- Carrier (5 cells)
- Battleship (4 cells)
- Cruiser (3 cells)
- Submarine (3 cells)
- Destroyer (2 cells)
  
Joystick Navigation:

- Move the cursor around the 8x16 grid.
  
Ship Orientation Control:

- Short press joystick button: Toggle horizontal <-> vertical
- Visual feedback with orientation change beep .
  
Ship Placement:

- Long press of the joystick button confirms placement
- The ship blinks quickly before being placed before placement
- A success sound plays if the placement is valid 
- An error sound plays if the placement is invalid.
  
Collision Detection:

- Ships cannot overlap
- Ships cannot be placed outside the grid.
- The cursor skips over existing ships.
  
Placement Order:

- Players place one ship at a time.
- The LCD shows how many ships are left to place.
- The LCD shows the name and length of the current ship.
- The game automatically moves to the next ship after a successful placement.

2. Map System

Dual Map Architecture:

- Each player has two 8x16 maps stored separately
- Ships Map: Stores ships positions
- Attack Map: Records attacks (to be used in gameplay phase)
  
Cell States:

- WATER = 0 // Empty cell
- SHIP = 1 // Ship present
- HIT = 2 // Ship hit by attack
- MISS = 3 // Water hit by attack
  
Map Display:

- Real-time rendering on LED matrices
- Ships show as lit LEDs
- Matrix 0: Columns 0-7
- Matrix 1: Columns 8-15
- Only 8 rows visible at once. Scrolling will be added to see the rest.

3. Menu System

Main Menu:

- Start Game
- About screen
  
Pause Menu:

- Resume Game
- Restart Game (clears maps and fleets)
- Exit to Main Menu
  
Game Flow:

- Intro message at the beginning
- Player 1 placement phase
- Player 2 placement phase
- Map screen shows both fleets
- Attack phase (to be implemented)

4. Audio Feedback

Imperial March Theme:

- Complete 69-note sequence
- Plays during menu screens
- Automatically stops during gameplay
  
Sound Effects:

- Cursor movement
- Orientation toggle (2000 Hz)
- Successful placement (1500 -> 2000 Hz dual tone)
- Invalid placement error (300 Hz)

5. Object-Oriented Architecture

The code is structured using object-oriented programming:

- Ship class: Manages individual ships (position, orientation, validation)
- Map class: Stores and manages the 8x16 grid
- Fleet class: Manages the 5 ships
- Display class: Handles the LED matrix and LCD
- Input class: Handles joystick and button inputs with debouncing
- Audio class: Plays music and sound effects
- Menu class: Manages the menu
- GameController class: Controls the overall game logic.

### What Needs to Be Implemented For The Final Project:

Core Gameplay Mechanics:

- Attack phase
- Detect hits and misses with feedback
- Track ship damage.
- Detect when ships sink.
- Detect victory (all enemy ships sunk).

Complete Menu System:

- Full Settings menu:
- Control LCD brightness
- Control Matrix brightness
- Turn sound on/off
- How to Play instructions screen

Sensor Integration:

- Integrate sensor in a game mechanic

### Resources & References:

- https://projecthub.arduino.cc/HiHiHiHiiHiiIiH/star-wars-on-a-buzzer-e70139

### Media:

Setup Photos:

<img width="1458" height="664" alt="Screenshot 2025-12-08 at 01 18 48" src="https://github.com/user-attachments/assets/7133e7cb-c41f-4a8a-be4a-d87c3ee06e69" />
<img width="462" height="372" alt="Screenshot 2025-12-08 at 01 18 30" src="https://github.com/user-attachments/assets/8694de8f-ab44-4fed-a520-1b52b1cbd6b0" />
<img width="781" height="534" alt="Screenshot 2025-12-08 at 01 19 05" src="https://github.com/user-attachments/assets/88f195d9-bb4e-4b68-999d-e60595cf37ea" />
<img width="1114" height="819" alt="Screenshot 2025-12-08 at 01 19 46" src="https://github.com/user-attachments/assets/1b544d1a-290f-472a-89d5-62d4bd6b17bb" />
<img width="1114" height="819" alt="Screenshot 2025-12-08 at 01 19 39" src="https://github.com/user-attachments/assets/b4283da8-43c0-4aed-809a-36907ebe5281" />
<img width="1114" height="819" alt="Screenshot 2025-12-08 at 01 19 33" src="https://github.com/user-attachments/assets/1a765a73-ef44-4d89-82c0-9697a00a1870" />
<img width="1114" height="819" alt="Screenshot 2025-12-08 at 01 19 24" src="https://github.com/user-attachments/assets/2edeaeab-9b3c-4a5b-a566-c479c563fa95" />




Video Demonstration: https://youtu.be/d0ZnUgxy8m8?si=SY6sl1CiGXIer6cr

</details>



<details>
<summary><h2>Checkpoint 3 Final Implementation</h2></summary>

This is a complete two-player implementation of the classic naval combat game Battleships built on Arduino Uno. The project combines LED matrix displays, LCD interface, joystick controls, and ultrasonic sensors to create an engaging strategic gaming experience.

**Team Members:** 
- https://github.com/filip407
- https://github.com/MitzaaA14

### Key Achievements
- **Full Gameplay**: Player selection -> Fleet deployment -> Turn-based combat -> Victory
- **Dual Display System**: Each player has dedicated 8x16 LED grid (two 8x8 matrices) and a 16x16 game logic with the rooms (one for own map and one for enemy map)
- **Input System**: Debounced joysticks with short/long press detection
- **Special Mechanics**: Ultrasonic sensor for a 3x3 airstrike power-up (2 for each player in every game)
- **Data Persistence**: EEPROM storage for player profiles, wins, and settings
- **Audio**: Imperial March theme music and dynamic sound effects
- **Menu System**: Main menu, settings, leaderboard, how to play, pause menu


Hardware Components

| Component | Quantity | Purpose |
|-----------|----------|---------|
| Arduino Uno | 1 | Main controller |
| 8x8 LED Matrix (MAX7219) | 4 | Game grid display (cascaded) |
| 16x2 LCD Display (I2C 0x27) | 1 | Menu and status display |
| Joystick Module | 2 | Player 1 & 2 controls |
| HC-SR04 Ultrasonic Sensor | 2 | Sonar airstrike mechanism |
| Passive Buzzer | 1 | Audio feedback and music |
| Push Button | 1 | Pause/menu navigation |
| 100kΩ Resistor | 4 | MAX7219 ISET pins |
| 104pF Capacitor | 4 | MAX7219 noise filtering |
| 10μF Electrolytic Capacitor | 4 | Power supply stabilization |
| 3xBreadboards & Jumper Wires | - | Connections |


Pin Configuration

| Component / Function | Signal / Pin | Arduino Pin |
|----------------------|--------------|-------------|
| **LED Matrices (MAX7219)** | DIN | D12 |
| | CLK | D11 |
| | CS (LOAD) | D10 |
| **LCD Display (I2C - 0x27)** | SDA | A4 |
| | SCL | A5 |
| **Buzzer** | Signal | D6 |
| **Pause Button** | Signal | D7 |
| **Player 1 Joystick** | X-axis (VRX) | A0 |
| | Y-axis (VRY) | A1 |
| | Button (SW) | D2 |
| **Player 2 Joystick** | X-axis (VRX) | A2 |
| | Y-axis (VRY) | A3 |
| | Button (SW) | D3 |
| **Player 1 Sonar (HC-SR04)** | TRIG | D4 |
| | ECHO | D5 |
| **Player 2 Sonar (HC-SR04)** | TRIG | D8 |
| | ECHO | D9 |

Software Architecture

### Object-Oriented Design
```cpp
class Ship {
  // Manages position, orientation, length
  // Checks if sunk (all parts hit)
}

class Map {
  // 8x16 grid storage
  // Cell state management (0=water, 1=ship, 2=hit, 3=miss)
}

class Fleet {
  // Manages 7 ships
  // Tracks placement progress
  // Counts alive ships
}

class Display {
  // LED matrix rendering
  // LCD text output
  // Scrolling text animation
}

class Input {
  // Joystick debouncing
  // Button short/long press detection
}

class Audio {
  // Imperial March playback (20-note sequence)
  // Sound effects (hit, miss, error, navigation)
}

class Sonar {
  // Ultrasonic distance measurement
  // Airstrike trigger detection
}

class Storage {
  // EEPROM player management
  // Settings persistence
  // Leaderboard sorting
}

class GameController {
  // State machine (11 states)
  // Game flow orchestration
}
```

### Game State Machine

**11 States:**
- 5 menu states (main, settings, how to play, leaderboard, about)
- 3 player setup states (select P1, select P2, name input)
- 1 pause state
- 2 game states (placement, combat)
- 1 victory state


Memory Management

### EEPROM Layout (1KB total)

Address 0-9:   GameSettings struct
Address 10:    Player count 
Address 11+:   PlayerRecord array:
               - name (3 chars + null )
               - win count (int)


### PROGMEM Usage

- 8x8 font bitmaps 
- Menu strings, ship names, messages
- Imperial March note sequence

Game Features

### Complete Gameplay
- Two-player simultaneous ship placement
- Turn-based combat with hit/miss detection
- Ship sinking detection (all cells hit)
- Victory condition and win tracking
- Sonar airstrike special ability (2 per player)

### Input Handling
- Joystick debouncing
- Short press (rotate) and long press (confirm)
- Pause button with state toggling
- Intelligent cursor skip over placed ships

### Audio System
- Imperial March background music (menu screens only)
- Sound effects: navigation, confirm, error, hit, miss, boom, victory
- Configurable enable/disable

### Visual Feedback
- LED matrix updates
- Cursor blink
- Miss cell blink 
- Ship placement preview blink
- Scrolling text for victory messages
- LCD status displays

### Data Persistence
- Player profiles saved to EEPROM
- Win counts tracked across sessions
- Settings persist (brightness, sound)
- Leaderboard sorted by wins

### Menu System
- **Main Menu**: Start Game, Settings, How to Play, Leaderboard, About
- **Settings Menu**: Matrix brightness (0-15), LCD brightness (0-10), Sound (ON/OFF), Reset Leaderboard
- **Pause Menu**: Resume, Restart, Exit
- **How to Play**: Interactive tutorial
- **Leaderboard**: Top 5 players by wins
- **About**: Shows game name, authors names and github usernames
  For each menu option a picture appears on each matrix.

### Display Optimization
- Bit reversal for correct LED matrix orientation
- Efficient column rendering for scrolling text
- Smart refresh only when needed (lcdDirty flag)

### Input Debouncing
- Separate timers for each joystick axis
- Initial hold delay before repeat
- Prevents spurious inputs from analog noise

### Collision Detection
- Overlap detection with existing ships
- Placement confirmation

### Audio Engine
- Dual-tone effects (frequency modulation)
- Music sequencer with note duration timing
- Automatic music stop during gameplay

### Sonar Mechanics
- Distance threshold: 15cm activation range
- 800ms for activation to prevent accidental triggers
- 3x3 area damage
- Visual and audio feedback


## Media

### Circuit Schema (without LCD)

<img width="720" height="436" alt="Screenshot 2025-12-15 at 13 39 10" src="https://github.com/user-attachments/assets/32dc3494-adf7-4392-9189-778a6aaa5f0e" />


### Physical Implementation 

<img width="567" height="762" alt="Screenshot 2025-12-15 at 13 40 00" src="https://github.com/user-attachments/assets/508f476a-f87a-4ef9-88e3-1fa7b9ae59d3" />
<img width="567" height="762" alt="Screenshot 2025-12-15 at 13 40 05" src="https://github.com/user-attachments/assets/b831f49e-4ce8-4e0a-987f-45e03c37ca86" />
<img width="567" height="762" alt="Screenshot 2025-12-15 at 13 39 29" src="https://github.com/user-attachments/assets/e68e7361-6cf7-422c-a7e8-27a0bd81a228" />
<img width="567" height="762" alt="Screenshot 2025-12-15 at 13 39 38" src="https://github.com/user-attachments/assets/4bbffb2f-e4ef-4a58-93df-37ffba781dee" />
<img width="567" height="762" alt="Screenshot 2025-12-15 at 13 39 43" src="https://github.com/user-attachments/assets/90b3b16e-a350-4ad8-8ff8-590ce15c72a9" />
<img width="567" height="762" alt="Screenshot 2025-12-15 at 13 39 48" src="https://github.com/user-attachments/assets/a98c21d7-ed3c-4257-8532-bb5b1ba3aa53" />
<img width="567" height="762" alt="Screenshot 2025-12-15 at 13 39 55" src="https://github.com/user-attachments/assets/38b2864f-0b1b-4e5b-b7a7-68dd8faa3285" />



### Video Demonstration

https://youtu.be/mqJJkp_8x_U?si=rNrnJzHELUI57RCK

</details>

<details>
<summary><h2>How to Play</h2></summary>

Game Objective

**Sink all 7 enemy ships before they sink yours!**

### Fleet Composition
Each player deploys:
- 1x Carrier (5 cells)
- 1x Battleship (4 cells)
- 3x Cruiser (3 cells each)
- 2x Destroyer (2 cells each)

**Total: 7 ships**

Phase 1: Player Setup

1. **Main Menu** -> **Start Game**
2. Select or create 3 letter name for Player 1
3. Select or create 3 letter name for Player 2

Names are saved and track your wins!

Phase 2: Fleet Deployment

Both players place ships simultaneously on their 8x16 grids.

### Controls

| Action | Input |
|--------|-------|
| Move cursor | Joystick X/Y |
| Rotate ship | Short button press |
| Confirm placement | Long button press |

### Rules
- Ships placed in order (largest to smallest)
- Ships cannot overlap
- Ships cannot go out of bounds
- Cursor skips over already placed ships

### Visual Feedback
- Blinking ship = preview before placement
- Fixed LEDs = confirmed ships
- Beep = success and Buzz = error

Phase 3: Combat

### View Switching
Each player has **two screens**:
- **Ships View**: Your fleet
- **Attack View**: Your attacks on enemy (hits/misses)

**Toggle between views:** Push joystick up/down at screen edge

### Attack Method 1: Standard Fire

**Controls:**
- Move cursor with joystick
- Press button to fire at cursor position

**Outcomes:**

| Result | Visual | Sound | Next Turn |
|--------|--------|-------|-----------|
| **HIT** | Solid LED | Rising tone | **You attack again** |
| **MISS** | Blinking LED | Low buzz | Opponent's turn |
| **Invalid** | No change | Error buzz | Try again (no turn pass) |

**Key Rule:** Hitting a ship gives you another turn!


### Attack Method 2: Sonar Airstrike

**Activation:**
1. Switch to **Attack View**
2. Position cursor at target
3. Hold hand **within 15cm** of your sensor for **800ms**
4. Airstrike launches automatically

**Effects:**
- Hits **3x3 area** around cursor (9 cells)
- All ships in area marked as HIT
- All water in area marked as MISS
- LCD shows AIRSTRIKE!!!
- Explosion sound plays

**Limitations:**
- Only **2 airstrikes** per game per player
- Turn **always passes** after airstrike (even on hits)
- Cannot recover used airstrikes
- Counter shown on LCD

Phase 4: Victory

### Win Condition
Destroy all 7 enemy ships

### When Game Ends
- Winner's name displayed on LCD
- Scrolling victory message on LED matrices
- Victory fanfare plays
- Win count saved to leaderboard
- Press any button to return to menu



Game Screen Info

### LCD Display During Combat

JOE TURN S:2 -> Current player + airstrikes remaining
MyShips:5 Tgt:7 -> Your ships alive vs Enemy targets left



### LED Matrix Display

Water - empty
Ship - fixed LED - in your own map 
Hit - fixed LED - in enemy map
Miss - LED blinks - in enemy map
Cursor - LED fast blink


### Pause Menu
Press **pause button** anytime during gameplay:
- **Resume**: Continue game
- **Restart**: Reset to deployment phase
- **Exit**: Return to main menu

### Settings Menu
Customize your experience:
- **Matrix Brightness** (0-15)
- **LCD Brightness** (0-10)
- **Sound** (ON/OFF)
- Settings saved permanently
- Reset Leaderboard

### Leaderboard
- Tracks top 5 players by wins
- Sorted automatically by win count
- Data saved in EEPROM

</details>


