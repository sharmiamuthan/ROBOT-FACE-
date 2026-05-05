#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Motor pins
int IN1 = 26;
int IN2 = 27;
int ENA = 25;

int IN3 = 14;
int IN4 = 12;
int ENB = 33;


// Ultrasonic pins
int trigPin = 5;
int echoPin = 18;

long duration;
int distance;

//  Eyes
void drawEyes(int lid) {
  display.drawCircle(40, 25, 12, WHITE);
  display.drawCircle(88, 25, 12, WHITE);

  display.fillCircle(40, 25, 4, WHITE);
  display.fillCircle(88, 25, 4, WHITE);

  display.fillRect(28, 13, 24, lid, BLACK);
  display.fillRect(76, 13, 24, lid, BLACK);
}

//  Mouth
void drawMouth(bool open) {
  if (open)
    display.fillRect(40, 45, 48, 10, WHITE);
  else
    display.drawLine(40, 50, 88, 50, WHITE);
}

//  Face
void showFace(int lid, bool mouthOpen) {
  display.clearDisplay();
  drawEyes(lid);
  drawMouth(mouthOpen);
  display.display();
}

//  Distance reading
int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  return distance;
}

//  Motors
void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  ledcWrite(ENA, 200);
  ledcWrite(ENB, 200);
}

void stopMotor() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void backward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  ledcWrite(ENA, 200);
  ledcWrite(ENB, 200);
}

void leftTurn() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void setup() {
  Serial.begin(115200);

  Wire.begin(21, 22);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // PWM setup
  ledcAttach(ENA, 5000, 8);
  

  ledcAttach(ENB, 5000, 8);
}

void loop() {

  int d = getDistance();
  Serial.println(d);

  //  OBSTACLE DETECTION
  if (d > 0 && d < 20) {

    stopMotor();
    showFace(20, false);   // scared face
    delay(500);

    backward();
    showFace(10, true);
    delay(800);

    leftTurn();
    showFace(0, false);
    delay(600);

  } else {

    forward();
    showFace(0, false); // normal face
  }

  delay(100);
}

