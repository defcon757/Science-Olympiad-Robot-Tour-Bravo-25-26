#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// =====================================================
// OLED SETUP
// =====================================================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// =====================================================
// MOTOR PINS (L298N)
// =====================================================
#define ENA 5
#define IN1 7
#define IN2 8

#define ENB 6
#define IN3 9
#define IN4 10

// =====================================================
// BUTTON
// =====================================================
#define BUTTON_PIN 12

// =====================================================
// ULTRASONIC
// =====================================================
#define TRIG_PIN 4
#define ECHO_PIN 11
#define OBSTACLE_DISTANCE_CM 10

// =====================================================
// ENCODERS (ONE CHANNEL EACH)
// =====================================================
#define LEFT_ENCODER 2   // Interrupt 0
#define RIGHT_ENCODER 3  // Interrupt 1

volatile long leftCount = 0;
volatile long rightCount = 0;

// =====================================================
// SPEED SETTINGS
// =====================================================
#define START_SPEED 70

#define BASE_FORWARD_LEFT_PWM 150
#define BASE_FORWARD_RIGHT_PWM 165

#define BASE_BACKWARD_LEFT_PWM 150
#define BASE_BACKWARD_RIGHT_PWM 165

#define TURN_SPEED 130

#define MIN_PWM 120
#define MAX_PWM 200

// =====================================================
// TIMING / CONTROL
// =====================================================
#define RAMP_TIME_MS 300
#define ENCODER_CORRECTION_INTERVAL_MS 1000

float Kp = 0.08;

// =====================================================
// GLOBAL MOTOR STATE
// =====================================================
int leftMotorPWM = 0;
int rightMotorPWM = 0;

// =====================================================
// MOTION MODE
// =====================================================
enum MotionMode {
  MOTION_NONE,
  MOTION_STRAIGHT,
  MOTION_TURN
};

// =====================================================
// ENCODER ISR
// =====================================================
void leftEncoderISR() {
  leftCount++;
}

void rightEncoderISR() {
  rightCount++;
}

// =====================================================
// HELPER: SNAPSHOT ENCODERS
// =====================================================
void getEncoderSnapshot(long &l, long &r) {
  noInterrupts();
  l = leftCount;
  r = rightCount;
  interrupts();
}

// =====================================================
// HELPER: RESET ENCODERS
// =====================================================
void resetEncoders() {
  noInterrupts();
  leftCount = 0;
  rightCount = 0;
  interrupts();
}

// =====================================================
// HELPER: SET MOTOR PWM
// =====================================================
void setMotorPWM(int leftPWM, int rightPWM) {
  leftMotorPWM = constrain(leftPWM, 0, 255);
  rightMotorPWM = constrain(rightPWM, 0, 255);

  analogWrite(ENA, leftMotorPWM);
  analogWrite(ENB, rightMotorPWM);
}

// =====================================================
// HELPER: STOP MOTORS
// =====================================================
void stopMotors() {
  setMotorPWM(0, 0);
}

// =====================================================
// OLED STATUS
// =====================================================
void updateOLEDStatus(const char* action, long distanceCM) {
  long encL, encR;
  getEncoderSnapshot(encL, encR);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);

  display.print("Action: ");
  display.println(action);

  display.print("Dist: ");
  if (distanceCM < 0) {
    display.println("-- cm");
  } else {
    display.print(distanceCM);
    display.println(" cm");
  }

  display.print("L PWM: ");
  display.print(leftMotorPWM);
  display.print("  R PWM: ");
  display.println(rightMotorPWM);

  display.print("Enc L/R: ");
  display.print(encL);
  display.print(" / ");
  display.println(encR);

  display.display();
}

// =====================================================
// ULTRASONIC DISTANCE
// =====================================================
long getDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30 ms timeout
  if (duration == 0) return -1;

  long distance = duration * 0.034 / 2;
  return distance;
}

// =====================================================
// MOTION FUNCTIONS
// =====================================================
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  setMotorPWM(BASE_FORWARD_LEFT_PWM, BASE_FORWARD_RIGHT_PWM);
}

void moveBackwards() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  setMotorPWM(BASE_BACKWARD_LEFT_PWM, BASE_BACKWARD_RIGHT_PWM);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  setMotorPWM(TURN_SPEED, TURN_SPEED);
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  setMotorPWM(TURN_SPEED, TURN_SPEED);
}

void turnAround() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  setMotorPWM(TURN_SPEED, TURN_SPEED);
}

// =====================================================
// BUTTON START
// =====================================================
void waitForStart() {
  updateOLEDStatus("Press Button", -1);

  while (digitalRead(BUTTON_PIN) == HIGH) {
    // Wait for button press.
  }

  delay(200); // debounce

  while (digitalRead(BUTTON_PIN) == LOW) {
    // Wait for button release.
  }

  delay(200); // debounce after release
}

// =====================================================
// SMART DELAY
// =====================================================
bool smartDelay(unsigned long durationMs, void (*action)(), const char* actionName, MotionMode mode) {
  resetEncoders();
  action();

  unsigned long startTime = millis();
  unsigned long lastOLEDUpdate = 0;
  unsigned long lastCorrectionTime = millis();

  long lastLeftSample = 0;
  long lastRightSample = 0;

  int runtimeLeftPWM = leftMotorPWM;
  int runtimeRightPWM = rightMotorPWM;

  while (millis() - startTime < durationMs) {
    unsigned long now = millis();
    long distance = getDistanceCM();

    if (distance > 0 && distance <= OBSTACLE_DISTANCE_CM) {
      stopMotors();
      updateOLEDStatus("Obstacle!", distance);
      delay(300);
      return false;
    }

    if (mode == MOTION_STRAIGHT && (now - lastCorrectionTime >= ENCODER_CORRECTION_INTERVAL_MS)) {
      long encL, encR;
      getEncoderSnapshot(encL, encR);

      long leftDelta = encL - lastLeftSample;
      long rightDelta = encR - lastRightSample;

      lastLeftSample = encL;
      lastRightSample = encR;

      long error = leftDelta - rightDelta;
      int correction = (int)(error * Kp);

      runtimeLeftPWM = constrain(runtimeLeftPWM - correction, MIN_PWM, MAX_PWM);
      runtimeRightPWM = constrain(runtimeRightPWM + correction, MIN_PWM, MAX_PWM);

      setMotorPWM(runtimeLeftPWM, runtimeRightPWM);

      lastCorrectionTime = now;
    }

    if (mode == MOTION_STRAIGHT) {
      unsigned long elapsed = now - startTime;

      if (elapsed < RAMP_TIME_MS) {
        int rampLeft = map((long)elapsed, 0, RAMP_TIME_MS, START_SPEED, runtimeLeftPWM);
        int rampRight = map((long)elapsed, 0, RAMP_TIME_MS, START_SPEED, runtimeRightPWM);
        setMotorPWM(rampLeft, rampRight);
      } else {
        setMotorPWM(runtimeLeftPWM, runtimeRightPWM);
      }
    }

    if (now - lastOLEDUpdate >= 120) {
      updateOLEDStatus(actionName, distance);
      lastOLEDUpdate = now;
    }

    delay(10);
  }

  stopMotors();
  updateOLEDStatus("Done", -1);
  return true;
}

// =====================================================
// SETUP
// =====================================================
void setup() {
  Serial.begin(9600);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(LEFT_ENCODER, INPUT_PULLUP);
  pinMode(RIGHT_ENCODER, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(LEFT_ENCODER), leftEncoderISR, RISING);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ENCODER), rightEncoderISR, RISING);

  stopMotors();

  Wire.begin();
  if (!display.begin(0x3C, true)) {
    while (true) {
      // OLED init failed.
    }
  }

  display.clearDisplay();
  display.display();

  updateOLEDStatus("Robot Booting", -1);
  delay(1000);

  waitForStart();
}

// =====================================================
// LOOP
// =====================================================
void loop() {
  smartDelay(14000, moveForward, "Moving Forward", MOTION_STRAIGHT);

  smartDelay(900, turnRight, "Turning Right", MOTION_TURN);

  smartDelay(5000, moveForward, "Moving Forward", MOTION_STRAIGHT);

  smartDelay(900, turnLeft, "Turning Left", MOTION_TURN);

  smartDelay(1700, turnAround, "Turning Around", MOTION_TURN);

  stopMotors();
  updateOLEDStatus("Robot Stopped", -1);

  while (true) {
    // End demo route.
  }
}
