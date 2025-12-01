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
- Multiple LED states with visual feedback - First button press **LED FIXED** , second button press **LED SLOW BLINKING**, third button press **LED FAST BLINKING**
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
-  D6->Pin 5
-  D7->Pin 4
-  Contrast (PWM)->Pin 3

Joystick:
-  VRx (X- axis)->A0
-  VRy (Y- axis)->A1
-  Button->A2

-  Buzzer->Pin 2

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

![IMG_4957](https://github.com/user-attachments/assets/04d9cae8-de4f-475b-bbc1-be4b2ff94a30)
![IMG_4959](https://github.com/user-attachments/assets/00bdc518-92e2-44da-8865-cf4cc94d921f)
![IMG_4958](https://github.com/user-attachments/assets/3de3168b-7126-4b71-a954-ca5040e61359)


# Video Demonstration


</details>
