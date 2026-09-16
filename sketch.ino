#include <Servo.h>

// FLEX SENSOR PINS
int flexThumb = A0;
int flexIndex = A1;
int flexMiddle = A2;
int flexRing = A3;
int flexLittle = A4;

// Thresholds
int openThreshold = 200;
int bentThreshold = 120;

// MOTOR DRIVER
int ENA = 10;   
int IN1 = 8;   
int IN2 = 7;

int ENB = 9;  
int IN3 = 12;  
int IN4 = 11;

// SERVO
Servo steering;
int servoPin = 5;

// BUZZER
int buzzer = 13;

// RGB LEFT
int leftR = 3;
int leftG = 6;

// RGB RIGHT
int rightR = 4;
int rightG = 2;

// SERVO POSITIONS
int centerPos = 90;
int leftPos = 135;
int rightPos = 45;

void setup() {
  Serial.begin(9600);
  steering.attach(servoPin);
  steering.write(centerPos);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(leftR, OUTPUT);
  pinMode(leftG, OUTPUT);
  pinMode(rightR, OUTPUT);
  pinMode(rightG, OUTPUT);

  pinMode(buzzer, OUTPUT);

  Serial.println("System Ready");
}

// RGB FUNCTIONS
void leftYellow() {
  digitalWrite(leftR, HIGH);
  digitalWrite(leftG, HIGH);
}

void rightYellow() {
  digitalWrite(rightR, HIGH);
  digitalWrite(rightG, HIGH);
}

void reverseRedBlink() {
  digitalWrite(leftR, HIGH);
  digitalWrite(rightR, HIGH);
  delay(150);
  digitalWrite(leftR, LOW);
  digitalWrite(rightR, LOW);
  delay(150);
}

void allRGBoff() {
  digitalWrite(leftR, LOW);
  digitalWrite(leftG, LOW);
  digitalWrite(rightR, LOW);
  digitalWrite(rightG, LOW);
}

// SOUND EFFECTS
void indicatorSound() {
  tone(buzzer, 2000, 200); // high
  delay(300);
  tone(buzzer, 1000, 200); // low
  delay(200);
}

void reverseSound() {
  tone(buzzer, 2000, 200); // high
  delay(150);
  tone(buzzer, 1000, 200); // low
  delay(200);
}

// MOTOR CONTROL
void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 200);
  analogWrite(ENB, 200);
}

void backward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 200);
  analogWrite(ENB, 200);
}

void stopCar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void loop() {

  int t = analogRead(flexThumb);
  int i = analogRead(flexIndex);
  int m = analogRead(flexMiddle);
  int r = analogRead(flexRing);
  int l = analogRead(flexLittle);

  Serial.print(t); Serial.print("  ");
  Serial.print(i); Serial.print("  ");
  Serial.print(m); Serial.print("  ");
  Serial.print(r); Serial.print("  ");
  Serial.println(l);

  bool thumbOpen = t > openThreshold;
  bool indexOpen = i > openThreshold;
  bool middleOpen = m > openThreshold;

  bool ringBent = r < bentThreshold;
  bool littleBent = l < bentThreshold;

  bool thumbBent = t < bentThreshold;
  bool indexBent = i < bentThreshold;
  bool middleBent = m < bentThreshold;

  // BACKWARD
  if (thumbOpen && indexOpen && middleOpen && ringBent && littleBent) {
    Serial.println("BACKWARD");
    backward();
    steering.write(centerPos);
    reverseRedBlink();
    reverseSound();
    return;
  }

  // FORWARD
  if (thumbOpen && indexOpen && middleOpen && !ringBent && !littleBent) {
    Serial.println("FORWARD");
    forward();
    steering.write(centerPos);
    allRGBoff();
    digitalWrite(buzzer, LOW);
    return;
  }

  // LEFT TURN
  if (thumbOpen && indexBent && middleBent && ringBent && littleBent) {
    Serial.println("LEFT TURN");
    forward();
    steering.write(leftPos);
    leftYellow();
    indicatorSound();
    return;
  }

  // RIGHT TURN
  if (indexOpen && thumbBent && middleBent && ringBent && littleBent) {
    Serial.println("RIGHT TURN");
    forward();
    steering.write(rightPos);
    rightYellow();
    indicatorSound();
    return;
  }

  // STOP
  if (thumbBent && indexBent && middleBent && ringBent && littleBent) {
    Serial.println("STOP");
    stopCar();
    steering.write(centerPos);
    digitalWrite(leftR, HIGH);
    digitalWrite(rightR, HIGH);
    digitalWrite(buzzer, LOW);
    return;
  }

  // DEFAULT
  Serial.println("IDLE/UNKNOWN");
  stopCar();
  allRGBoff();
  steering.write(centerPos);
  digitalWrite(buzzer, LOW);
}

/******************************************
🟢 1. FORWARD
All 5 fingers straight

✔ Thumb → Straight
✔ Index → Straight
✔ Middle → Straight
✔ Ring → Straight
✔ Little → Straight

🔸 Forward
🔸 No LED
🔸 No buzzer

🟥 2. STOP

All 5 fingers bent

✘ Thumb → Bent
✘ Index → Bent
✘ Middle → Bent
✘ Ring → Bent
✘ Little → Bent

🔸 Red ON
🔸 Stop motors
🔸 No buzzer

🔙 3. BACKWARD (updated as you said)

Thumb + Index + Middle → Straight

Ring + Little → Bent

✔ Thumb → Straight
✔ Index → Straight
✔ Middle → Straight
✘ Ring → Bent
✘ Little → Bent

🔸 Motors backward
🔸 Red blinking
🔸 Reverse beep

🟡 4. LEFT TURN

Only Thumb straight

✔ Thumb → Straight
✘ Index → Bent
✘ Middle → Bent
✘ Ring → Bent
✘ Little → Bent

🔸 Servo left
🔸 Left yellow
🔸 Indicator tick-tick
🔸 Forward movement

🟡 5. RIGHT TURN

Only Index straight

✘ Thumb → Bent
✔ Index → Straight
✘ Middle → Bent
✘ Ring → Bent
✘ Little → Bent

🔸 Servo right
🔸 Right yellow
🔸 Indicator tick-tick
🔸 Forward movement 
**********************************/