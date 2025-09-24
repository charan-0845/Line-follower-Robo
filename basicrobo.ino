// ===== Motor driver pins (L298N) =====
#define ENA 12   // PWM for Motor A
#define IN1 25
#define IN2 26
#define IN3 27
#define IN4 14
#define ENB 13   // PWM for Motor B

// ===== Sensor array pins =====
int sensorsFront[5] = {32, 33, 34, 35, 36}; // Front 5 sensors
int sensorSideLeft = 4;  // Left side sensor
int sensorSideRight = 15; // Right side sensor
int nearPin = 39;         // Obstacle sensor

// ===== Settings =====
int frontThreshold = 3000;     // Front sensors threshold (black < threshold)
int sideThreshold = 3000;      // Side sensors threshold (black > threshold)
int normalSpeed = 200;         // Normal forward speed
int sharpTurnSpeed = 180;  
int vturnspeed = 190;          // Sharp turn speed
int moderateTurnSpeed = 100;   // Moderate correction speed
int rescueSpeed = 120;         // Speed during rescue mode

// ===== Setup =====
void setup() {
  Serial.begin(115200);

  // Motor control pins
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Sensor input pins
  for (int i = 0; i < 5; i++) {
    pinMode(sensorsFront[i], INPUT);
  }
  pinMode(sensorSideLeft, INPUT);
  pinMode(sensorSideRight, INPUT);
  pinMode(nearPin, INPUT);

  Serial.println("Robot Ready!");
}

// ===== Main Loop =====
void loop() {
  int frontValues[5];
  int sideLeftValue = analogRead(sensorSideLeft);
  int sideRightValue = analogRead(sensorSideRight);
  int nearSensorValue = digitalRead(nearPin);

  // Obstacle detection
  if (nearSensorValue == HIGH) { // Obstruction detected
    stopMotors();
    Serial.println("Obstacle detected! Stopping.");
    return;
  }

  // Read front sensors
  for (int i = 0; i < 5; i++) {
    frontValues[i] = analogRead(sensorsFront[i]);
  }

  // Rescue Mode - Off track
  bool rescueMode = false;
  if (frontValues[0] > frontThreshold && frontValues[1] > frontThreshold &&
      frontValues[2] > frontThreshold && frontValues[3] > frontThreshold &&
      frontValues[4] > frontThreshold) {
    rescueMode = true;
  }

  if (rescueMode) {
    rescueLine(sideLeftValue, sideRightValue);
  } else {
    followLine(frontValues, sideLeftValue, sideRightValue);
  }

  delay(5);
}

// ===== Line Following Logic =====
void followLine(int frontValues[], int sideLeftValue, int sideRightValue) {
  // Count how many sensors are detecting black
  int blackCount = 0;
  for (int i = 0; i < 5; i++) {
    if (frontValues[i] < frontThreshold) {
      blackCount++;
    }
  }

  if (blackCount >= 2) {
    moveForward(normalSpeed);
    Serial.println("Multiple sensors on line → Moving straight");
    return;
  }

  // Individual sensor logic
  if (frontValues[0] < frontThreshold) {
    sharpLeft();
    Serial.println("Sharp Left Turn");
  }
  else if (frontValues[1] < frontThreshold) {
    moderateLeft();
    Serial.println("Moderate Left Turn");
  }
  else if (frontValues[2] < frontThreshold) {
    moveForward(normalSpeed);
    Serial.println("Moving Straight");
  }
  else if (frontValues[3] < frontThreshold) {
    moderateRight();
    Serial.println("Moderate Right Turn");
  }
  else if (frontValues[4] < frontThreshold) {
    sharpRight();
    Serial.println("Sharp Right Turn");
  }
  else if (sideLeftValue > sideThreshold && sideRightValue < sideThreshold) {
    VLeft();
    Serial.println("Rescue: Turning Left");
  }
  else if (sideRightValue > sideThreshold && sideLeftValue < sideThreshold) {
    VRight();
    Serial.println("Rescue: Turning Right");
  }
  else {
    stopMotors();
    Serial.println("No line detected. Stopping.");
  }
}

// ===== Rescue Logic =====
void rescueLine(int sideLeftValue, int sideRightValue) {
  Serial.println("Rescue Mode Activated!");
  if (sideLeftValue > sideThreshold) {
    VLeft();
    Serial.println("Rescue: VLeft()");
  }
  else if (sideRightValue > sideThreshold) {
    VRight();
    Serial.println("Rescue: VRight()");
  }
  else {
    stopMotors();
    Serial.println("Rescue: Still searching...");
  }
}

// ===== Motor control functions =====
void moveForward(int speed) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

void moderateLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, moderateTurnSpeed);
  analogWrite(ENB, moderateTurnSpeed);
}

void moderateRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, moderateTurnSpeed);
  analogWrite(ENB, moderateTurnSpeed);
}

void sharpLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, sharpTurnSpeed);
  analogWrite(ENB, sharpTurnSpeed);
}

void sharpRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, sharpTurnSpeed);
  analogWrite(ENB, sharpTurnSpeed);
}

void VRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, vturnspeed);
  analogWrite(ENB, vturnspeed);
}

void VLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, vturnspeed);
  analogWrite(ENB, vturnspeed);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
