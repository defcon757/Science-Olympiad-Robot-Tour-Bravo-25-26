# Tuning Guide

This robot uses time-based route steps with simple encoder correction during straight movement. Tune one thing at a time and test on the same surface when possible.

## Base Speed

Forward base speed:

```cpp
#define BASE_FORWARD_LEFT_PWM 150
#define BASE_FORWARD_RIGHT_PWM 165
```

If the robot curves before correction has much time to react, adjust these values first. Raise the slower side or lower the faster side.

## Correction Gain

Encoder correction uses:

```cpp
long error = leftDelta - rightDelta;
int correction = (int)(error * Kp);
```

Current value:

```cpp
float Kp = 0.08;
```

Suggested starting range:

```text
0.03 - 0.10
```

If the robot over-corrects or wiggles, lower `Kp`. If it still drifts slowly, raise `Kp` a little.

## Correction Interval

Current value:

```cpp
#define ENCODER_CORRECTION_INTERVAL_MS 1000
```

A lower value, such as `200`, can make correction feel smoother if encoder readings are stable.

## PWM Limits

Current limits:

```cpp
#define MIN_PWM 120
#define MAX_PWM 200
```

Raise `MIN_PWM` if the motors stall at low speeds. Lower `MAX_PWM` if the robot is too fast or hard to control.

## Startup Ramp

The sketch ramps from:

```cpp
#define START_SPEED 70
```

over:

```cpp
#define RAMP_TIME_MS 300
```

Raise `START_SPEED` if the robot hesitates at the beginning of a move. Increase `RAMP_TIME_MS` if starts are too abrupt.

## Route Timing

Route timing is in `loop()`:

```cpp
smartDelay(14000, moveForward, "Moving Forward", MOTION_STRAIGHT);
smartDelay(900, turnRight, "Turning Right", MOTION_TURN);
```

Increase or decrease each duration to change the route.
