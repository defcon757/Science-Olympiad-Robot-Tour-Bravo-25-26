# Tuning Guide

## Motor Speed

The current base speeds are:

```cpp
#define BASE_FORWARD_LEFT_PWM 150
#define BASE_FORWARD_RIGHT_PWM 165
```

Use these values to correct for a motor that is naturally slower or faster before encoder correction begins. If the robot consistently curves right, the left wheel is usually moving faster than the right wheel. If it curves left, the right wheel is usually moving faster.

## Encoder Correction

The sketch uses proportional correction:

```cpp
long error = leftDelta - rightDelta;
int correction = (int)(error * Kp);

runtimeLeftPWM = constrain(runtimeLeftPWM - correction, MIN_PWM, MAX_PWM);
runtimeRightPWM = constrain(runtimeRightPWM + correction, MIN_PWM, MAX_PWM);
```

Current gain:

```cpp
float Kp = 0.08;
```

Suggested range:

```text
0.03 - 0.10
```

Start low and increase slowly. If the robot wiggles or over-corrects, reduce `Kp`.

## Correction Interval

Current interval:

```cpp
#define ENCODER_CORRECTION_INTERVAL_MS 1000
```

A shorter interval, such as `200`, may produce smoother correction once the robot is reliably reading encoder counts.

## PWM Limits

Current limits:

```cpp
#define MIN_PWM 120
#define MAX_PWM 200
```

Increase `MAX_PWM` if the robot needs more top speed. Increase `MIN_PWM` if the motors stall after correction. Lower `MIN_PWM` if the robot moves too aggressively at low speed.

## Startup Ramp

The robot ramps from:

```cpp
#define START_SPEED 70
```

to the runtime PWM over:

```cpp
#define RAMP_TIME_MS 300
```

This reduces startup current draw and helps with alkaline battery voltage sag. If the robot hesitates, try raising `START_SPEED` gradually.

## Obstacle Threshold

Current threshold:

```cpp
#define OBSTACLE_DISTANCE_CM 10
```

Increase this value if the robot needs more stopping distance.
