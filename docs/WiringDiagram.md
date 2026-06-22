# Wiring Diagram

This file describes the wiring in text form. Add photos or a drawn diagram to the `images/` folder as the robot build evolves.

## Power

- Arduino Nano `5V` powers the OLED and HC-SR04 sensor.
- Arduino Nano `GND`, L298N `GND`, OLED `GND`, HC-SR04 `GND`, button `GND`, and encoder `GND` must share a common ground.
- The motor battery pack powers the L298N motor input.

## L298N to Arduino Nano

```text
ENA -> D5
IN1 -> D7
IN2 -> D8
ENB -> D6
IN3 -> D9
IN4 -> D10
GND -> GND
```

Remove the `ENA` and `ENB` jumpers before uploading this sketch.

## OLED to Arduino Nano

```text
VCC -> 5V
GND -> GND
SDA -> A4
SCL -> A5
```

## HC-SR04 to Arduino Nano

```text
VCC  -> 5V
GND  -> GND
TRIG -> D4
ECHO -> D11
```

## Button to Arduino Nano

```text
Signal -> D12
Other side -> GND
```

## Encoders to Arduino Nano

```text
Left encoder channel  -> D2
Right encoder channel -> D3
```
