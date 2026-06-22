# Arduino Nano Autonomous Robot Platform

Arduino Nano autonomous robot platform using an L298N motor driver, SH1106 OLED, HC-SR04 ultrasonic sensor, push-button start, and dual wheel encoder feedback.

The sketch is designed for robotics experimentation and Science Olympiad EV-style development. It includes battery-friendly motor ramping, OLED status updates, obstacle-aware movement timing, and proportional encoder correction for straighter driving.

## Features

- Dual DC motor drive with an L298N motor driver
- SH1106 I2C OLED status display
- HC-SR04 ultrasonic obstacle detection
- Push-button start on `D12`
- Dual encoder counting on interrupt pins `D2` and `D3`
- Proportional motor balancing with encoder feedback
- Smart movement delays that continue checking sensors
- Reusable movement commands for forward, reverse, turns, and turn-around

## Hardware

- Arduino Nano
- L298N dual H-bridge motor driver
- SH1106 OLED display, I2C, address `0x3C`
- HC-SR04 ultrasonic sensor
- Two motor encoders, one channel each
- Momentary push button
- 8x AA alkaline battery pack

## Libraries

Install these through the Arduino IDE Library Manager:

- `Adafruit GFX Library`
- `Adafruit SH110X`

The sketch also uses Arduino's built-in `Wire` library.

## Quick Start

1. Open `electricVehicle.ino` in the Arduino IDE.
2. Select `Arduino Nano` as the board.
3. Install the required Adafruit libraries.
4. Wire the robot using [PinMapping.md](docs/PinMapping.md).
5. Remove the `ENA` and `ENB` jumpers from the L298N so PWM speed control works.
6. Upload the sketch.
7. Press the button connected to `D12` and `GND` to start the demo route.

## Current Demo Route

The default `loop()` runs:

1. Move forward for 14 seconds.
2. Turn right for 0.9 seconds.
3. Move forward for 5 seconds.
4. Turn left for 0.9 seconds.
5. Turn around for 1.7 seconds.
6. Stop.

If the ultrasonic sensor sees an obstacle within `10 cm`, the robot stops the current command, shows an OLED warning, and continues to the next command.

## Repository Structure

```text
electricVehicle/
|-- electricVehicle.ino
|-- README.md
|-- LICENSE
|-- .gitignore
|-- docs/
|   |-- PinMapping.md
|   |-- TuningGuide.md
|   `-- WiringDiagram.md
`-- images/
    `-- .gitkeep
```

## Future Ideas

- Full PID drive correction
- Encoder-based distance driving
- Encoder-based turning
- Command queue system
- Obstacle avoidance behaviors
- Battery voltage monitoring
- OLED menu interface
- Autonomous navigation routines
