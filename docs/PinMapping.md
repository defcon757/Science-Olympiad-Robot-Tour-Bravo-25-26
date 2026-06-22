# Pin Mapping

## L298N Motor Driver

| L298N Pin | Arduino Nano |
| --- | --- |
| ENA | D5 PWM |
| IN1 | D7 |
| IN2 | D8 |
| ENB | D6 PWM |
| IN3 | D9 |
| IN4 | D10 |
| GND | GND |

Remove the `ENA` and `ENB` jumpers from the L298N board. The sketch uses `D5` and `D6` for PWM speed control.

## OLED Display

| OLED Pin | Arduino Nano |
| --- | --- |
| VCC | 5V |
| GND | GND |
| SDA | A4 |
| SCL | A5 |

OLED I2C address:

```cpp
0x3C
```

## Push Button

| Button Pin | Arduino Nano |
| --- | --- |
| Signal | D12 |
| Other side | GND |

The sketch uses the Arduino internal pull-up resistor:

```cpp
pinMode(BUTTON_PIN, INPUT_PULLUP);
```

Button behavior:

- `HIGH`: released
- `LOW`: pressed

## Ultrasonic Sensor

| HC-SR04 Pin | Arduino Nano |
| --- | --- |
| VCC | 5V |
| GND | GND |
| TRIG | D4 |
| ECHO | D11 |

## Encoders

Only one channel per encoder is used in this version.

| Encoder | Arduino Nano |
| --- | --- |
| Left encoder channel | D2 |
| Right encoder channel | D3 |

The sketch counts rising edges:

```cpp
attachInterrupt(digitalPinToInterrupt(LEFT_ENCODER), leftEncoderISR, RISING);
attachInterrupt(digitalPinToInterrupt(RIGHT_ENCODER), rightEncoderISR, RISING);
```
