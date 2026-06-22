# Robot Tour

Arduino Nano robot tour platform for driving a small two-motor robot through a programmed route while showing status on an OLED display and checking for nearby obstacles.

The project is intentionally general-purpose. It can be used as a starting point for classroom demos, robotics experiments, route testing, or a custom autonomous robot build.

## What It Does

- Drives two DC motors through an L298N motor driver
- Starts only after a push button press
- Shows movement status, distance, PWM values, and encoder counts on an SH1106 OLED
- Reads distance from an HC-SR04 ultrasonic sensor
- Stops a movement early if an obstacle is detected
- Counts left and right wheel encoder pulses
- Applies simple proportional correction during straight driving
- Uses a gentle startup ramp to reduce sudden motor load

## Hardware

- Arduino Nano
- L298N dual motor driver
- Two DC motors
- SH1106 I2C OLED display
- HC-SR04 ultrasonic sensor
- Two wheel encoders, one channel each
- Momentary push button
- Battery pack suitable for the motors and driver

## Required Arduino Libraries

Install these with the Arduino IDE Library Manager:

- `Adafruit GFX Library`
- `Adafruit SH110X`

The built-in `Wire` library is also used.

## Quick Start

1. Open `robotTour.ino` in the Arduino IDE.
2. Select `Arduino Nano` as the board.
3. Install the required Adafruit libraries.
4. Wire the robot using [PinMapping.md](docs/PinMapping.md).
5. Remove the `ENA` and `ENB` jumpers from the L298N so PWM speed control works.
6. Upload the sketch.
7. Press the button connected to `D12` and `GND` to start the route.

## Default Route

The demo route currently:

1. Moves forward.
2. Turns right.
3. Moves forward again.
4. Turns left.
5. Turns around.
6. Stops.

Timing values can be changed in the `loop()` function.

## Obstacle Behavior

The ultrasonic sensor is checked during movement. If an object is detected within the configured threshold, the robot stops the current movement, shows an obstacle warning on the OLED, and continues to the next route step.

Current threshold:

```cpp
#define OBSTACLE_DISTANCE_CM 10
```

## Repository Structure

```text
robotTour/
|-- robotTour.ino
|-- README.md
|-- LICENSE
|-- .gitignore
|-- docs/
|   |-- PinMapping.md
|   |-- TuningGuide.md
|   `-- WiringNotes.md
`-- images/
    `-- .gitkeep
```

## Possible Improvements

- Tune encoder correction for smoother straight driving
- Add route presets
- Add encoder-based turns
- Add encoder-based distance targets
- Add a basic OLED menu
- Add battery voltage monitoring
- Add a better obstacle avoidance routine
