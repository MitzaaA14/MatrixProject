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
