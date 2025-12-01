## Matrix Draw Project
Introduction to Robotics (2025-2026)

This repository contains the Matrix Draw project developed for the Introduction to Robotics course during the 2025-2026 academic year at the Faculty of Mathematics and Informatics, University of Bucharest, Computer Science domain.

<details>
<summary><h2>Checkpoint #1 -Interactive LED Matrix Drawing System</h2></summary>

Matrix Draw is an interactive LED drawing application that allows users to create pixel art on an 8x8 LED matrix using:

-Joystick controller. 
-Multiple LED states (fixed, slow blink, fast blink)
-Intuitive menu system displayed on an LCD screen
-Audio feedback through a buzzer

# Task Requirements:
Design and implement an interactive matrix project with the following features:
-Working 8x8 LED matrix controlled via joystick
-LCD screen with menu navigation -Controlled by joystick when the player isn't currently in a game
-One interactive mode: "Start Game" -Allows player to draw on led matrix
-External input that directly influences matrix output -Joystick
-Multiple LED states with visual feedback -First button press --LED FIXED--, second button press --LED SLOW BLINKING--, third button press --LED FAST BLINKING--
-Audio feedback for user actions

# Components:

-Arduino Uno
-8x8 LED Matrix
-MAX7219
-LCD Display (16x2) : Menu display and information
-Joystick Module : User input (menu navigation and drawing on matrix)
-Buzzer : Audio feedback
-Resistors : 220Ω for LCD and 100kΩ for matrix MAX7219	
-LCD contrast and connections
-Wires : Circuit connections

# Pin Configuration:

LED Matrix:

- DIN -> Pin 12
- CLOCK -> Pin 11
- LOAD -> Pin 10

LCD Display:

- RS -> Pin 9
- Enable -> Pin 8
- D4 -> Pin 7
- D5 -> Pin 6
- D6 -> Pin 5
- D7 -> Pin 4
- Contrast (PWM) -> Pin 3

Joystick:
- VRx (X-axis) -> A0
- VRy (Y-axis) -> A1
- Button -> A2

- Buzzer -> Pin 2

# System Features:

Main Menu:

Three options for the menu:

1. Start Game - Drawing mode where you can create pixel art
2. Clear Draw - Clears the entire drawing
3. About - Displays project information

Navigation:

1. Joystick Up/Down - Navigate between menu items
2. Button Press - Select current menu option
3. Visual cursor indicator blinks to show selected option

Draw Mode:

- Once "Start Game" is selected, the system enters interactive drawing mode:

Controls:
1. Joystick -> Move cursor on 8x8 matrix
2. Short Button Press -> Cycle through LED states at cursor position
3. Long Press (3 seconds) -> Exit to menu

LED States:
1. OFF - LED is turned off
2. FIXED - LED is constantly on
3. SLOW - LED blinks slowly
4. FAST - LED blinks rapidly

Display Information:
- Line 1: "DRAW MODE"
- Line 2: Current cursor position (X, Y) and LED state

Audio Feedback:
The buzzer provides distinct tones for different actions:

- Menu navigation
- Menu selection
- Cursor movement
- Drawing cleared
- Exit draw mode

Visual Feedback:
- Cursor Blinking - Indicates current position in menu and draw mode
- LCD Contrast - Adjusts for optimal visibility (normal/dim modes)
- Matrix Blinking - Two independent blink rates for creative patterns

System States:

1. Splash Screen
- Displays "Matrix Draw" and "Loading..." on startup
- Duration: 1 second
- Automatically transitions to main menu

2. Main Menu
- Shows menu options with blinking cursor indicator
- Displays current drawing on the matrix
- Joystick navigation

3. Draw Mode
- Interactive drawing interface
- Cursor position and state display
- LED animations

4. Message Display
- Temporary screens for "Drawing cleared!" and "About" information
- Returns to menu after set duration

Code Architecture:
Key Functions:
- setup() : Initialize all components and display splash screen
- loop() : Main loop handling timing and state management
- showMenu() : Display and update menu on LCD
- menuLoop() : Handle menu navigation and selection
- startGame() : Initialize draw mode
- gameLoop() : Handle drawing interactions and cursor movement
- updateMatrix() : Refresh LED matrix based on current states
- displayDrawing() : Show drawing while in menu
- updateDisplay() : Update LCD with cursor info and LED state
- clearAll() : Reset all LEDs to OFF state
- showAbout() : Display project information

Non-Blocking Timing
All timing operations use millis() to avoid blocking delays:
- Independent blink timers for slow/fast LED states
- Cursor blink timer for menu and draw mode
- Debounce timers for joystick and button inputs
- Duration timers for splash and message screens


Physical Implementation Checkpoint #1
[Include photos of your physical setup here]

Video Demonstration
[Link to your YouTube video showing the project in action]


Matrix Draw successfully implements an interactive LED matrix drawing system with intuitive joystick control, multiple LED states, and comprehensive menu navigation. The project demonstrates proficiency in hardware integration, non-blocking programming techniques, and user interface design on embedded systems.

</details>