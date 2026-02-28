/*
 * ============================================================
 *  AgroShield - Smart Farm Protection Node
 * ============================================================
 *  Features:
 *    - IR Sensor  : Animal intrusion detection + buzzer alert
 *    - Ultrasonic : Water level monitoring + pump auto-cutoff
 *    - Relay      : Controls water pump
 *    - OLED       : Displays system status (128x64, I2C)
 *    - Touch Sensor: Cycles between 3 display modes
 *
 *  Libraries Required (install via Arduino Library Manager):
 *    - Adafruit SSD1306
 *    - Adafruit GFX Library
 *
 *  Pin Connections:
 *    IR Sensor OUT  --> D2
 *    Ultrasonic TRIG--> D3
 *    Ultrasonic ECHO--> D4
 *    Relay IN       --> D5
 *    Buzzer         --> D6
 *    Touch Sensor   --> D7
 *    OLED SDA       --> A4 (Uno) / SDA pin
 *    OLED SCL       --> A5 (Uno) / SCL pin
 * ============================================================
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>

#define TRIG_PIN 2
#define ECHO_PIN 3
#define LED_PIN 4
#define BUZZER_PIN 5
#define TOUCH_PIN 6
#define RELAY_PIN 8
#define SERVO_PIN 9
#define SOIL_PIN A0

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Servo scareServo;

bool intrusionDetected = false;
bool displayMode = false;
bool lastTouchState = LOW;

const float tankHeight = 20.0;
const int waterLowPercent = 10;

unsigned long lastSendTime = 0;

void setup() {

  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(TOUCH_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, HIGH);  // Pump OFF initially

  scareServo.attach(SERVO_PIN);
  scareServo.write(0);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true); // OLED fail safe
  }

  display.clearDisplay();
  display.display();
}

void loop() {

  // ===================================
  // RECEIVE DATA FROM PYTHON
  // ===================================
  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');
    data.trim();

    if (data == "INTRUSION") {
      intrusionDetected = true;
    }
    else if (data == "CLEAR") {
      intrusionDetected = false;
    }
  }

  // ===================================
  // TOUCH SENSOR (EDGE DETECTION FIX)
  // ===================================
  bool currentTouch = digitalRead(TOUCH_PIN);

  if (currentTouch == HIGH && lastTouchState == LOW) {
    displayMode = !displayMode;
    delay(200);
  }

  lastTouchState = currentTouch;

  // ===================================
  // WATER LEVEL
  // ===================================
  long duration;
  float distance;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;

  float waterLevel = tankHeight - distance;
  float waterPercent = (waterLevel / tankHeight) * 100;

  if (waterPercent > 100) waterPercent = 100;
  if (waterPercent < 0) waterPercent = 0;

  // ===================================
  // SOIL SENSOR (Mapped Properly)
  // ===================================
  int soilRaw = analogRead(SOIL_PIN);

  int soilPercent = map(soilRaw, 1023, 300, 0, 100);
  if (soilPercent > 100) soilPercent = 100;
  if (soilPercent < 0) soilPercent = 0;

  // ===================================
  // SMART PUMP CONTROL
  // ===================================
  if (waterPercent < waterLowPercent) {
    digitalWrite(RELAY_PIN, HIGH);
  }
  else {
    if (soilPercent < 35) {
      digitalWrite(RELAY_PIN, LOW);
    } else {
      digitalWrite(RELAY_PIN, HIGH);
    }
  }

  // ===================================
  // INTRUSION DEFENSE
  // ===================================
  if (intrusionDetected) {
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    scareServo.write(90);
  }
  else {
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    scareServo.write(0);
  }

  // ===================================
  // OLED DISPLAY (Same as Earlier Style)
  // ===================================
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  if (!displayMode) {

    display.setCursor(0, 0);
    display.println("SURVEILLANCE MODE");

    display.setCursor(0, 15);
    if (intrusionDetected)
      display.println("INTRUSION DETECTED!");
    else
      display.println("System Normal");

    display.setCursor(0, 30);
    display.print("Water: ");
    display.print(waterPercent);
    display.println("%");

  } else {

    display.setCursor(0, 0);
    display.println("SOIL & WATER MODE");

    display.setCursor(0, 15);
    display.print("Soil: ");
    display.print(soilPercent);
    display.println("%");

    display.setCursor(0, 30);
    display.print("Water: ");
    display.print(waterPercent);
    display.println("%");

    display.setCursor(0, 45);
    if (digitalRead(RELAY_PIN) == LOW)
      display.println("Pump: ON");
    else
      display.println("Pump: OFF");
  }

  display.display();

  // ===================================
  // SEND DATA TO PYTHON
  // ===================================
  if (millis() - lastSendTime > 2000) {

    Serial.print("SOIL:");
    Serial.print(soilPercent);
    Serial.print(",WATER:");
    Serial.println(waterPercent);

    lastSendTime = millis();
  }

}

