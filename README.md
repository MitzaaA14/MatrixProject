# Matrix Draw Project Introduction to Robotics (2025-2026)

This repository contains the *Matrix Project* created for the *Introduction to Robotics course* during the 2025-2026 academic year at the Faculty of Mathematics and Informatics, University of Bucharest, Computer Science domain.  Code, implementation, explanations, and extra resources (pictures, test results, videos) are all included in each checkpoint.

 ## Game Overview
 Battleships is a complete two-player implementation of the classic naval combat strategy game built on Arduino Uno. Players deploy a fleet of 7 ships on an 8x16 grid and take turns attacking enemy positions to sink all opposing vessels. The game features dual LED matrix displays, LCD interface, joystick controls, and an innovative sonar airstrike mechanic using ultrasonic sensors. Players can create persistent profiles that track wins across sessions, with a leaderboard showcasing the top 5 commanders. Strategic gameplay is enhanced by the chain-attack rule: hitting a ship grants an immediate follow-up turn, allowing skilled players to dominate the battlefield.

 ## Team Members: 
- https://github.com/filip407
- https://github.com/MitzaaA14

<details>
<summary><h1>Battleships Project</h1></summary>

This is a complete two-player implementation of the classic naval combat game Battleships built on Arduino Uno. The project combines LED matrix displays, LCD interface, joystick controls, and ultrasonic sensors to create an engaging strategic gaming experience.

<details>
<summary><h3>Project Evolution</h3></summary>

The development of this project followed a structured path, evolving from basic hardware control to a complex game engine:

1. Hardware Prototyping & Drawing Tool
Initially, we focused on mastering the hardware components. We built a Pixel Art Drawing Tool to implement and test:

- Matrix Control: Managing individual pixels on the 8x8 LED matrix (ON, OFF, Blink states)
- Joystick Navigation: Mapping analog inputs to matrix coordinates
- LCD Interface: Creating a menu system and displaying real-time coordinates
- Sound: Implementing non-blocking audio feedback for user actions

Key Features Implemented:

- Working 8x8 LED matrix controlled via joystick
- LCD screen with menu navigation
- Interactive drawing mode
- Multiple LED states (FIXED, SLOW BLINK, FAST BLINK)
- Audio feedback system
- Non-blocking timing using millis() to prevent delays


Technical Challenges Solved:

- Cursor blinking without blocking the main loop
- Debouncing joystick inputs to prevent spurious readings
- Managing LED state transitions (OFF -> FIXED -> SLOW -> FAST -> OFF cycle)
- Splash screen with automatic menu transition
- Real-time cursor position (X, Y) and LED state display on LCD


https://youtu.be/-X2TxVpON-k?si=WHG6nfVh5Rvq6QeS


2. Game Logic Implementation
Once the hardware layer was robust, we moved to the core game architecture:

- Map System: Implemented dual 8x16 maps (Ships Map & Attack Map) for each player
- Ship Mechanics: Defined classes for ships (Carrier, Battleship, Cruiser, Submarine, Destroyer) including rotation logic and collision detection (preventing overlaps and out of bounds placement)
- Scrolling View: Since the physical display is smaller than the logical map, we implemented a vertical scrolling mechanism to view the full battlefield
- Fleet Management: System for placing 7 ships with proper validation

Major Changes:

- Expanded display: Cascaded 4x LED matrices horizontally (two per player)
- Dual joystick system: Added Player 2 joystick for simultaneous input
- Object-oriented redesign: Refactored into classes (Ship, Map, Fleet, Display, Input, Audio, GameController)
- I2C LCD: Switched from parallel to I2C interface to free up pins

Placement Mechanics:

Short button press: Toggle ship orientation (horizontal <-> vertical)
Long button press: Confirm placement
Ship blinks rapidly before placement to show preview
Success/error sound effects for feedback
Cursor automatically skips over placed ships

Menu System:

Main Menu: Start Game, About
Pause Menu: Resume, Restart, Exit
Imperial March theme music (69-note sequence) during menu screens
Music automatically stops during gameplay

https://youtu.be/d0ZnUgxy8m8?si=SY6sl1CiGXIer6cr

3. Complete Gameplay & Advanced Features
The final implementation transformed the project into a fully playable game with advanced mechanics and persistent data:
Combat System:

Turn-based attack mechanics with hit/miss detection
Smart turn rule: Hitting a ship grants another turn (chain attacks possible)
View switching: Players toggle between Ships View (own fleet) and Attack View (enemy attacks)
Visual feedback: Solid LED for hits, blinking LED for misses
Audio feedback: Rising tone for hits, low buzz for misses



Player profile system: 3-character names saved permanently
Win tracking across sessions
Leaderboard: Top 5 players sorted by wins
Settings persistence: Brightness levels and sound preferences
Profile selection/creation at game start


Features:

- Memory optimization using PROGMEM for fonts, strings, and music
- Efficient input debouncing with separate timers per axis
- Short/long press detection (800ms threshold)
- Smart LCD refresh (only updates when needed via lcdDirty flag)
- Bit reversal for correct LED matrix orientation
- Non-blocking animations (cursor blink, miss cell blink)
- Robust ship sinking detection (all parts hit)
- Imperial March background music (20-note sequence stored in PROGMEM)
- 7 different sound effects: navigation, confirm, error, hit, miss, boom, victory
- Configurable ON/OFF in settings
- Music plays only in menus, stops during gameplay
- Detects when all enemy ships are sunk
- Winner's name displayed on LCD
- Scrolling victory message across LED matrices
- Victory fanfare plays
- Win count automatically saved to EEPROM
- Leaderboard automatically updated

Complete Menu System:

- Main Menu: Start Game, Settings, How to Play, Leaderboard, About
- Settings: Matrix brightness (0-15), LCD brightness (0-10), Sound (ON/OFF), Reset Leaderboard
- How to Play: Interactive tutorial with game rules and controls
- Each menu option displays unique pixel art on matrices

Sensor Integration - Sonar Airstrike:
We integrated HC-SR04 ultrasonic sensors for a unique special ability:

- Each player has their own sensor
- Hold hand within 15cm for 800ms to trigger
- Attacks 3x3 area around cursor (9 cells)
- Limited to 2 airstrikes per player per game
- "AIRSTRIKE!!!" displayed on LCD with explosion sound
- Turn always passes after airstrike

https://youtu.be/d0ZnUgxy8m8?si=SY6sl1CiGXIer6cr

</details>


<details>
<summary><h3> Hardware Components and Pin Configuration </h3></summary>

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


| Component / Function | Signal / Pin | Arduino Pin |
|----------------------|--------------|-------------|
| LED Matrices (MAX7219) | DIN | D12 |
| | CLK | D11 |
| | CS (LOAD) | D10 |
| LCD Display (I2C - 0x27) | SDA | A4 |
| | SCL | A5 |
| Buzzer | Signal | D6 |
| Pause Button | Signal | D7 |
| Player 1 Joystick | X-axis (VRX) | A0 |
| | Y-axis (VRY) | A1 |
| | Button (SW) | D2 |
| Player 2 Joystick | X-axis (VRX) | A2 |
| | Y-axis (VRY) | A3 |
| | Button (SW) | D3 |
| Player 1 Sonar (HC-SR04) | TRIG | D4 |
| | ECHO | D5 |
| Player 2 Sonar (HC-SR04) | TRIG | D8 |
| | ECHO | D9 |

</details>

<details>
<summary><h3> Software Architecture</h3></summary>

## Object-Oriented Design
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

## Game State Machine

11 States:
- 5 menu states (main, settings, how to play, leaderboard, about)
- 3 player setup states (select P1, select P2, name input)
- 1 pause state
- 2 game states (placement, combat)
- 1 victory state

</details>

<details>
 <summary><h3>Memory Management</h3></summary>


### EEPROM Layout

Address 0-9:   GameSettings struct

Address 10:    Player count
 
Address 11+:   PlayerRecord array:
               - name (3 chars + null )
               - win count (int)


### PROGMEM Usage

- 8x8 font bitmaps 
- Menu strings, ship names, messages
- Imperial March note sequence

</details>

<details>
 <summary><h3> Game Features </h3></summary>

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
- Main Menu: Start Game, Settings, How to Play, Leaderboard, About
- Settings Menu: Matrix brightness (0-15), LCD brightness (0-10), Sound (ON/OFF), Reset Leaderboard
- Pause Menu: Resume, Restart, Exit
- How to Play: Interactive tutorial
- Leaderboard: Top 5 players by wins
- About: Shows game name, authors names and github usernames

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

</details>

<details>
 <summary><h3>Media</h3></summary>

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
</details>
</details>

<details>
<summary><h1>How to Play</h1></summary>

## Game Objective

Sink all 7 enemy ships before they sink yours!

### Fleet Composition
Each player deploys:
- 1x Carrier (5 cells)
- 1x Battleship (4 cells)
- 3x Cruiser (3 cells each)
- 2x Destroyer (2 cells each)

Total: 7 ships

## Phase 1: Player Setup

1. Main Menu -> Start Game
2. Select or create 3 letter name for Player 1
3. Select or create 3 letter name for Player 2


## Phase 2: Fleet Deployment

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

## Phase 3: Combat

### View Switching
Each player has two screens:
- Ships View: Your fleet
- Attack View: Your attacks on enemy (hits/misses)

Toggle between views: Push joystick to the right at screen edge

### Attack Method 1: Standard Fire

Controls:
- Move cursor with joystick
- Press button to fire at cursor position

Outcomes:

| Result | Visual | Sound | Next Turn |
|--------|--------|-------|-----------|
| HIT | Solid LED | Rising tone | You attack again |
| MISS | Blinking LED | Low buzz | Opponent's turn |
| Invalid | No change | Error buzz | Try again (no turn pass) |

Key Rule: Hitting a ship gives you another turn!


### Attack Method 2: Sonar Airstrike

Activation:
1. Switch to Attack View
2. Position cursor at target
3. Hold hand within 15cm of your sensor for 800ms
4. Airstrike launches automatically

Effects:
- Hits 3x3 area around cursor (9 cells)
- All ships in area marked as HIT
- All water in area marked as MISS
- LCD shows AIRSTRIKE!!!
- Explosion sound plays

Limitations:
- Only 2 airstrikes per game per player
- Turn always passes after airstrike (even on hits)
- Cannot recover used airstrikes
- Counter shown on LCD

## Phase 4: Victory

### Win Condition
Destroy all 7 enemy ships

### When Game Ends
- Winner's name displayed on LCD
- Scrolling victory message on LED matrices
- Victory fanfare plays
- Win count saved to leaderboard
- Press any button to return to menu


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
Press pause button anytime during gameplay:
- Resume: Continue game
- Restart: Reset to deployment phase
- Exit: Return to main menu

### Settings Menu
Customize your experience:
- Matrix Brightness (0-15)
- LCD Brightness (0-10)
- Sound (ON/OFF)
- Settings saved permanently
- Reset Leaderboard

### Leaderboard
- Tracks top 5 players by wins
- Sorted automatically by win count
- Data saved in EEPROM

</details>

<details>
<summary><h1>Project Progress Images and Videos</h1></summary>

<details>
<summary><h3>Hardware Prototyping & Drawing Tool</h3></summary> 

<img width="579" height="760" alt="Screenshot 2025-12-01 at 17 08 52" src="https://github.com/user-attachments/assets/8477afb4-51f0-4208-9ad7-c85690e8ece6" />

<img width="973" height="736" alt="Screenshot 2025-12-01 at 17 09 03" src="https://github.com/user-attachments/assets/327b9e30-196e-42ad-b16f-e699a9f0a0e0" />

<img width="973" height="736" alt="Screenshot 2025-12-01 at 17 09 09" src="https://github.com/user-attachments/assets/43beabde-8739-4edf-8c3e-44b6f6cf6b82" />

# Video Demonstration

 https://youtu.be/-X2TxVpON-k?si=WHG6nfVh5Rvq6QeS

</details>

<details>
<summary><h3>Game Logic</h3></summary> 
 
<img width="1458" height="664" alt="Screenshot 2025-12-08 at 01 18 48" src="https://github.com/user-attachments/assets/7133e7cb-c41f-4a8a-be4a-d87c3ee06e69" />

<img width="462" height="372" alt="Screenshot 2025-12-08 at 01 18 30" src="https://github.com/user-attachments/assets/8694de8f-ab44-4fed-a520-1b52b1cbd6b0" />

<img width="781" height="534" alt="Screenshot 2025-12-08 at 01 19 05" src="https://github.com/user-attachments/assets/88f195d9-bb4e-4b68-999d-e60595cf37ea" />

<img width="1114" height="819" alt="Screenshot 2025-12-08 at 01 19 46" src="https://github.com/user-attachments/assets/1b544d1a-290f-472a-89d5-62d4bd6b17bb" />

<img width="1114" height="819" alt="Screenshot 2025-12-08 at 01 19 39" src="https://github.com/user-attachments/assets/b4283da8-43c0-4aed-809a-36907ebe5281" />

<img width="1114" height="819" alt="Screenshot 2025-12-08 at 01 19 33" src="https://github.com/user-attachments/assets/1a765a73-ef44-4d89-82c0-9697a00a1870" />

<img width="1114" height="819" alt="Screenshot 2025-12-08 at 01 19 24" src="https://github.com/user-attachments/assets/2edeaeab-9b3c-4a5b-a566-c479c563fa95" />

Video Demonstration: https://youtu.be/d0ZnUgxy8m8?si=SY6sl1CiGXIer6cr

</details>
</details>
