# Embedded Flight State Simulator - ATtiny1626

An embedded C flight-state simulator built for the QUTy ATtiny1626 development board.

The projects simulates a flight sequence based off physics SUVAT equations with constant acceleration and no resistive force except for those already included in the acceleration value. This was done through a use of timer driven updates, button input, a two-digit 7-segment display, serial telemetry and a basic flight mode state machine. It's goal n creation was to strengthen my understanding of low-level microcontroller programming, module C design, hardware timers, GPIO input, display control and serial communication 

## Overview 

The QUTy Embedded Flight State Simulator models a basic launch, ascent, descent, and landing sequence.

The system begins in an idle state. When the launch button is pressed (S1), the simulator assigns the flight object an initial velocity and a constant negative acceleration. The program then updates the simulated position and velocity once per second. When the object reaches its peak and begins descending, the system transitions into descent mode. Once the simulated position reaches ground level, the system enters the landed state.


- Hardware timer interrupts
- GPIO button input
- Internal pull-up resistors
- Falling-edge button detection
- Modular C source and header files
- Basic physics modelling
- State-machine logic
- 7-segment display output
- Serial telemetry output
- Python-based data plotting

## Key Features 

### Flight Mode State Machine 

- FLIGHT_IDLE which waits for the flight sequence to begin
- FLIGHT_ASCENT which simulates upward movement
- FLIGHT_DESCENT which simulates downward movement
- FLIGHT_LANDED which represents the completed flight

### Timer Driven Updates 

The simulator uses a hardware timer interrupt to create regular timing events.

A timer interrupt generates smaller ticks, which are counted until a one-second update flag is set. The main loop checks this flag and updates the flight simulation once per second.

This approach avoids relying on blocking delays for the main simulation timing and gives the program a more realistic embedded structure.

### Button Controls 


Button Function: 
- S1 = Launch from idle
- S2 = Reserved / Not heavily used in the final version
- S3 = Reset simulation
- S4 = Pause/Resume

The buttons have been configured as inputs with internal pull-up resistors. Meaning that the button reads HIGH when unpressed and LOW when pressed. 

The button system detects falling edges, meaning that it detects the moment a button changed from a unpressed state to a pressed state. This is useful as it prevents the program from repeatedly trigering the same action while the button is held down.

### 7-Segment Display Output

The project controls the QUTy two-digit 7-segment display. 

The display will show the current simulated position value. As the display only has two digits the program shows the position module 100


Position --	Display
- 0	-- 00
- 5 -- 05
- 34 -- 34
- 107 -- 07

The display uses a digit lookup table and rapidly alternates between the left and right digits. This technique is called multiplexing. The switching happens fast enough that the human eye sees both digits as being displayed at the same time.

### Serial Telemetry Output

The simulator can be used to print out the flight data over serial in a CSV style formate.

The header is: 
t,position,velocity,acceleration,mode

Example output:

|  t | Position | Velocity | Acceleration | Mode |
| -: | -------: | -------: | -----------: | ---: |
|  0 |        0 |       10 |           -1 |    1 |
|  1 |       10 |        9 |           -1 |    1 |
|  2 |       19 |        8 |           -1 |    1 |
|  3 |       27 |        7 |           -1 |    1 |
|  4 |       34 |        6 |           -1 |    1 |


This output makes the simulation easier to debug and allows the data to be imported into other programs.

### Python Telemetry Plotting 

The project contains a python script (flight_data.py), which is used to read the telemetry from the QUTy over a serial and convert it into graphs which is a more easily understood representation of the data 

The script stores and plots:

- Time
- Position
- Velocity
- Acceleration
- Flight mode

## Project Structure 

### File Descriptions 

| File             | Description                                                                                          |
| ---------------- | ---------------------------------------------------------------------------------------------------- |
| `main.c`         | Coordinates system initialisation, button handling, display refreshing, and the flight state machine |
| `flight_state.c` | Contains the simplified flight physics update function                                               |
| `flight_state.h` | Defines the flight state structure and related function prototypes                                   |
| `timer.c`        | Configures the hardware timer and one-second update flag                                             |
| `timer.h`        | Exposes the timer initialisation function and timer flag                                             |
| `input.c`        | Configures button inputs and detects button press events                                             |
| `input.h`        | Defines button bitmasks and input function prototypes                                                |
| `display.c`      | Handles two-digit 7-segment display output                                                           |
| `display.h`      | Defines display function prototypes                                                                  |
| `qutyserial.h`   | Provides QUTy serial function declarations                                                           |
| `flight_data.py` | Reads serial telemetry and plots flight data                                                         |

### Simulation model 

The simulator uses a simple integer based physics model 


The simulator uses a simplified integer-based physics model with dt set to 1: 

| Velocity Equation| Position Equation |
| ---------------- | --------------------------------------------------------------------------------|
| new_velocity = old_velocity + acceleration × dt | new_position = old_position + old_velocity × dt  |

Initial starting values: 
- state.position = 0;
- state.velocity = 10;
- state.acceleration = -1;

This creates a simple flight arc where the object rises, slows down, reaches a peak, descends, and eventually returns to ground level.

## Flight Mode Behaviour

### FLIGHT_IDLE

The simulator weights for the launch button to be pressed. 

Behaviour:

Prints telemetry
Displays 00
Waits for launch input
FLIGHT_ASCENT

### FLIGHT_ASCENT

The simulated object moves upward.

Behaviour:

Prints telemetry once per second
Displays the current position
Updates position and velocity
Checks whether the object has started descending

The system transitions to FLIGHT_DESCENT when the updated position becomes lower than the previous position.

### FLIGHT_DESCENT

The simulated object falls back toward ground level.

Behaviour:

Prints telemetry once per second
Displays the current position
Updates position and velocity
Checks whether the object has reached the ground

When the position reaches zero or below, the simulator resets position, velocity, and acceleration to zero, then enters FLIGHT_LANDED.

### FLIGHT_LANDED

The simulated flight is complete.

Behaviour:

Prints telemetry
Displays 00
Waits for reset

## Button Mapping 

- #define INPUT_ACCEL_UP_bm      PIN4_bm
- #define INPUT_ACCEL_DOWN_bm    PIN5_bm
- #define INPUT_RESET_bm         PIN6_bm
- #define INPUT_PAUSE_bm         PIN7_bm

## Running The Project
- Open the project folder in PlatformIO / VS Code.
- Connect the QUTy ATtiny1626 development board over USB.
- Build and upload the firmware using PlatformIO.
- Open the serial monitor.
- Press S1 to launch the simulation.
- Press S4 to pause or resume.
- Press S3 to reset.

## Python Telemetry Plotting Setup

The included Python script can be used to read and plot telemetry from the QUTy board.

1. Install the required Python packages:  pip install pyserial matplotlib
2. Run the script: python flight_data.py
3. The serial port is currently set as: SERIAL_PORT = "/dev/cu.usbserial-10" This may need to be changed depending on the connected device and computer.

### Example Flight Output

|  t | position | velocity | acceleration | mode |
| -: | -------: | -------: | -----------: | ---: |
|  0 |        0 |       10 |           -1 |    1 |
|  1 |       10 |        9 |           -1 |    1 |
|  2 |       19 |        8 |           -1 |    1 |
|  3 |       27 |        7 |           -1 |    1 |
|  4 |       34 |        6 |           -1 |    1 |
|  5 |       40 |        5 |           -1 |    1 |
|  6 |       45 |        4 |           -1 |    1 |
|  7 |       49 |        3 |           -1 |    1 |
|  8 |       52 |        2 |           -1 |    1 |
|  9 |       54 |        1 |           -1 |    1 |
| 10 |       55 |        0 |           -1 |    1 |
| 11 |       55 |       -1 |           -1 |    1 |
| 12 |       54 |       -2 |           -1 |    2 |
| 13 |       52 |       -3 |           -1 |    2 |


## Skills Demonstrated

- Embedded C programming
- Modular source/header file structure
- GPIO configuration
- Pull-up resistor usage
- Button event detection
- Hardware timer configuration
- Interrupt-driven timing
- State-machine design
- Basic physics simulation
- 7-segment display control
- Display multiplexing
- Serial telemetry
- Python data visualisation
- Debugging through serial output
- Microcontroller-based system design

## Known Limitations 

- The physics model is intentionally simplified.
- The simulation uses integer arithmetic.
- The model does not include drag, thrust curves, changing mass, or environmental effects.
- The display can only show two digits.
- Position values above 99 wrap visually on the display.
- Button debouncing uses a simple delay-based method.
- S2 is defined but reserved for future use.
- The project does not currently use real sensor data.

## Future Improvments 

- Add a more realistic flight model
- Add thrust and drag calculations
- Add changing acceleration during different flight phases
- Add sensor input
- Add altitude limits
- Improve button debouncing using timer-based sampling
- Add display modes for velocity, acceleration, and state
- Add buzzer feedback for different flight states
- Add serial commands from the computer
- Add data logging
- Add a live Python telemetry dashboard
- Add fixed-point arithmetic
- Add error states
- Expand the architecture toward a more advanced flight-computer style project

## Purpose 

I have designed and built this project to help practice embedded systems development through practical microcontroller based application. This has llowed me to cfomine timer interrupts, GPIO, display output, serial communication, modular C programming and state machine logic into one working system. 

Furthermore this project has helped granted me insight into how embedded software cane be structured into seperate modules so that the program is easier to test, deubg and expand upon


## Author

William Ludlow

Second-year Electrical Engineering student at Queensland University of Technology.
