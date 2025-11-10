#include <Servo.h>

// ===========================
// Motor Pins
// ===========================
#define ENA 3     // Motor A Speed Control (PWM)
#define ENB 11    // Motor B Speed Control (PWM)
#define IN1 8     // Motor A Direction 1
#define IN2 9     // Motor A Direction 2
#define IN3 10    // Motor B Direction 1
#define IN4 12    // Motor B Direction 2

// ===========================
// Ultrasonic Sensor Pins
// ===========================
#define TRIG 6
#define ECHO 7

// ===========================
// Servo
// ===========================
#define SERVO_PIN 5
Servo myServo;

// ===========================
// Analog Flame Sensors
// ===========================
const int flameSensorLeft  = A1; // Left Flame Sensor
const int flameSensorFront = A0; // Front Flame Sensor
const int flameSensorRight = A2; // Right Flame Sensor
const int flameThreshold   = 400; // Flame detection threshold (calibrate per sensor)

bool obstacleDetected = false; // Flag to track if obstacle was detected
String lastTurn = "";          // Store direction of last turn for realignment

void setup() {
  Serial.begin(9600);

  // Motor setup
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Ultrasonic setup
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  // Servo initialization
  myServo.attach(SERVO_PIN);
  myServo.write(60);  // Center the servo (adjust center if needed)
  delay(500);
}

// Function to read ultrasonic distance in cm
long readDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duration = pulseIn(ECHO, HIGH, 30000); // timeout 30ms
  if (duration == 0) return 999; // no echo
  return duration * 0.034 / 2;
}

// ===========================
// Motor Control Functions
// ===========================
void moveForward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, 103);
  analogWrite(ENB, 100);
}

void stopMoving() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void turnLeft() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, 53);
  analogWrite(ENB, 50);
  delay(500);
  stopMoving();
}

void turnRight() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, 53);
  analogWrite(ENB, 50);
  delay(500);
  stopMoving();
}

void loop() {
  // ===========================
  // Flame Sensor Readings
  // ===========================
  int valueLeft  = analogRead(flameSensorLeft);  // Left
  int valueFront = analogRead(flameSensorFront); // Front
  int valueRight = analogRead(flameSensorRight); // Right

  Serial.println("---- Flame Sensor Readings ----");
  Serial.print("Sensor LEFT  (A1): "); Serial.println(valueLeft);
  Serial.print("Sensor FRONT (A0): "); Serial.println(valueFront);
  Serial.print("Sensor RIGHT (A2): "); Serial.println(valueRight);

  // If any sensor reads below threshold (indicating flame)
  if (valueLeft < flameThreshold || valueFront < flameThreshold || valueRight < flameThreshold) {
    stopMoving();

    if (valueLeft < flameThreshold) {
      Serial.println("🔥 Flame detected on LEFT");
      myServo.write(120); // point servo to left
    } else if (valueRight < flameThreshold) {
      Serial.println("🔥 Flame detected on RIGHT");
      myServo.write(0);   // point servo to right
    } else {
      Serial.println("🔥 Flame detected in FRONT");
      myServo.write(60);  // center servo
    }

    delay(500);
    return; // skip obstacle avoidance when flame is detected
  }

  // ===========================
  // Obstacle Avoidance (Front Only)
  // ===========================
  myServo.write(60);  // Keep servo centered for distance measurement
  delay(200);
  long frontDist = readDistance();

  Serial.print("Front Distance (cm): "); Serial.println(frontDist);

  if (frontDist < 40) {
    stopMoving();
    obstacleDetected = true;

    // Default avoidance: turn left first
    turnLeft();
    lastTurn = "left";

    moveForward();
    delay(1000);
  } else if (obstacleDetected) {
    // Re-align after avoidance
    stopMoving();

    if (lastTurn == "left") {
      turnRight();
    } else if (lastTurn == "right") {
      turnLeft();
    }

    delay(500);
    moveForward();

    obstacleDetected = false;
    lastTurn = "";
  } else {
    moveForward();
  }
}
