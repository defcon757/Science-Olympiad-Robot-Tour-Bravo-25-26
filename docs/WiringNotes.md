# Wiring Notes

Use a common ground between the Arduino, motor driver, sensors, button, encoders, and battery system.

## L298N

```text
ENA -> D5
IN1 -> D7
IN2 -> D8
ENB -> D6
IN3 -> D9
IN4 -> D10
GND -> GND
```

Remove the `ENA` and `ENB` jumpers if your L298N board has them installed.

## OLED

```text
VCC -> 5V
GND -> GND
SDA -> A4
SCL -> A5
```

## HC-SR04

```text
VCC  -> 5V
GND  -> GND
TRIG -> D4
ECHO -> D11
```

## Button

```text
Signal -> D12
Other side -> GND
```

## Encoders

```text
Left encoder channel  -> D2
Right encoder channel -> D3
```

If an encoder count is not changing on the OLED, check power, ground, signal pin, and whether the selected encoder channel is producing pulses.
