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

// ---------- OLED Configuration ----------
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT  64
#define OLED_RESET     -1   // No reset pin
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ---------- Pin Definitions ----------
#define IR_SENSOR_PIN   2
#define TRIG_PIN        3
#define ECHO_PIN        4
#define RELAY_PIN       5
#define BUZZER_PIN      6
#define TOUCH_PIN       7

// ---------- Constants ----------
#define LOW_WATER_CM        5     // Threshold: pump cuts off below this
#define TANK_HEIGHT_CM     30     // Full tank height in cm
#define BUZZER_DURATION   2000   // Buzzer ON duration in ms
#define DEBOUNCE_DELAY     200   // Touch sensor debounce in ms

// ---------- Display Modes ----------
#define MODE_STATUS        0
#define MODE_WATER         1
#define MODE_SECURITY_LOG  2
#define TOTAL_MODES        3

// ---------- Global State ----------
int  currentMode      = MODE_STATUS;
int  intrusionCount   = 0;
bool pumpActive       = true;   // true = relay closed = pump ON
bool lowWater         = false;
bool buzzerOn         = false;
unsigned long buzzerStartTime = 0;
unsigned long lastTouchTime   = 0;

// ---------- Function Prototypes ----------
long  readDistanceCM();
void  checkIRSensor();
void  checkWaterLevel();
void  checkTouchSensor();
void  updateBuzzer();
void  updateDisplay();
void  displayStatusMode(float distCM);
void  displayWaterMode(float distCM);
void  displaySecurityMode();
void  showSplashScreen();

// ============================================================
//  SETUP
// ============================================================
void setup() {
  Serial.begin(9600);

  // Pin modes
  pinMode(IR_SENSOR_PIN, INPUT);
  pinMode(TRIG_PIN,       OUTPUT);
  pinMode(ECHO_PIN,       INPUT);
  pinMode(RELAY_PIN,      OUTPUT);
  pinMode(BUZZER_PIN,     OUTPUT);
  pinMode(TOUCH_PIN,      INPUT);

  // Default states
  digitalWrite(RELAY_PIN,  LOW);   // LOW = Relay ON (pump running) — adjust if active-HIGH relay
  digitalWrite(BUZZER_PIN, LOW);

  // OLED init
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 OLED not found! Check wiring."));
    while (true);  // Halt
  }

  showSplashScreen();
  delay(2500);
  display.clearDisplay();
}

// ============================================================
//  MAIN LOOP
// ============================================================
void loop() {
  checkIRSensor();
  checkWaterLevel();
  checkTouchSensor();
  updateBuzzer();
  updateDisplay();
  delay(200);  // Small delay for stability
}

// ============================================================
//  IR SENSOR — Animal Intrusion Detection
// ============================================================
void checkIRSensor() {
  int irState = digitalRead(IR_SENSOR_PIN);

  // Most IR modules output LOW when beam is broken
  if (irState == LOW) {
    intrusionCount++;
    buzzerOn = true;
    buzzerStartTime = millis();
    digitalWrite(BUZZER_PIN, HIGH);
    Serial.print(F("INTRUSION DETECTED! Count: "));
    Serial.println(intrusionCount);
    delay(500);  // Brief block to avoid multiple counts per single crossing
  }
}

// ============================================================
//  ULTRASONIC — Water Level Monitoring
// ============================================================
long readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30ms timeout
  if (duration == 0) return -1;  // No echo received
  return duration * 0.034 / 2;
}

void checkWaterLevel() {
  long dist = readDistanceCM();
  if (dist < 0) return;  // Invalid reading

  // Distance from sensor to water surface
  // Low distance = tank is full; High distance = tank is empty
  if (dist >= (TANK_HEIGHT_CM - LOW_WATER_CM)) {
    // Water level is critically low
    if (!lowWater) {
      lowWater = true;
      pumpActive = false;
      digitalWrite(RELAY_PIN, HIGH);  // HIGH = Relay OFF = pump stopped
      Serial.println(F("LOW WATER — Pump stopped to prevent dry run!"));
    }
  } else {
    // Enough water — ensure pump is running
    if (lowWater) {
      lowWater = false;
      pumpActive = true;
      digitalWrite(RELAY_PIN, LOW);   // LOW = Relay ON = pump running
      Serial.println(F("Water refilled — Pump restarted."));
    }
  }
}

// ============================================================
//  TOUCH SENSOR — Mode Switching
// ============================================================
void checkTouchSensor() {
  if (digitalRead(TOUCH_PIN) == HIGH) {
    unsigned long now = millis();
    if (now - lastTouchTime > DEBOUNCE_DELAY) {
      currentMode = (currentMode + 1) % TOTAL_MODES;
      lastTouchTime = now;
      Serial.print(F("Mode switched to: "));
      Serial.println(currentMode);
    }
  }
}

// ============================================================
//  BUZZER — Auto-Off After Duration
// ============================================================
void updateBuzzer() {
  if (buzzerOn && (millis() - buzzerStartTime >= BUZZER_DURATION)) {
    buzzerOn = false;
    digitalWrite(BUZZER_PIN, LOW);
  }
}

// ============================================================
//  OLED DISPLAY — Route to Current Mode
// ============================================================
void updateDisplay() {
  long dist = readDistanceCM();
  float waterCM = 0;
  if (dist > 0) {
    waterCM = TANK_HEIGHT_CM - dist;
    if (waterCM < 0) waterCM = 0;
  }

  switch (currentMode) {
    case MODE_STATUS:       displayStatusMode(waterCM);   break;
    case MODE_WATER:        displayWaterMode(waterCM);    break;
    case MODE_SECURITY_LOG: displaySecurityMode();        break;
  }
}

// ---- Mode 0: Status Overview ----
void displayStatusMode(float waterCM) {
  display.clearDisplay();

  // Title bar
  display.fillRect(0, 0, SCREEN_WIDTH, 12, WHITE);
  display.setTextColor(BLACK);
  display.setTextSize(1);
  display.setCursor(20, 2);
  display.print(F("AgroShield STATUS"));

  display.setTextColor(WHITE);

  // Pump status
  display.setCursor(0, 16);
  display.print(F("Pump: "));
  display.print(pumpActive ? F("RUNNING") : F("STOPPED"));

  // Water level
  display.setCursor(0, 28);
  display.print(F("Water: "));
  display.print((int)waterCM);
  display.print(F(" cm"));

  // Intrusion status
  display.setCursor(0, 40);
  display.print(F("Intrus: "));
  display.print(intrusionCount);

  // Low water warning
  if (lowWater) {
    display.setCursor(0, 52);
    display.print(F("! Low Water - Pump Off"));
  } else if (buzzerOn) {
    display.setCursor(0, 52);
    display.print(F("! ANIMAL DETECTED !"));
  }

  // Mode indicator dots
  for (int i = 0; i < TOTAL_MODES; i++) {
    if (i == currentMode)
      display.fillCircle(56 + i * 8, 62, 2, WHITE);
    else
      display.drawCircle(56 + i * 8, 62, 2, WHITE);
  }

  display.display();
}

// ---- Mode 1: Water Monitoring ----
void displayWaterMode(float waterCM) {
  display.clearDisplay();

  // Title bar
  display.fillRect(0, 0, SCREEN_WIDTH, 12, WHITE);
  display.setTextColor(BLACK);
  display.setTextSize(1);
  display.setCursor(15, 2);
  display.print(F("WATER MONITORING"));

  display.setTextColor(WHITE);

  // Water level value
  display.setTextSize(2);
  display.setCursor(30, 18);
  display.print((int)waterCM);
  display.print(F(" cm"));
  display.setTextSize(1);

  // Visual tank bar (50px tall, 20px wide)
  int barHeight = map((int)waterCM, 0, TANK_HEIGHT_CM, 0, 38);
  barHeight = constrain(barHeight, 0, 38);
  display.drawRect(100, 14, 22, 40, WHITE);
  display.fillRect(101, 14 + (38 - barHeight), 20, barHeight, WHITE);

  // Percentage
  int pct = map((int)waterCM, 0, TANK_HEIGHT_CM, 0, 100);
  pct = constrain(pct, 0, 100);
  display.setCursor(98, 56);
  display.print(pct);
  display.print(F("%"));

  // Status text
  display.setCursor(0, 38);
  if (lowWater) {
    display.print(F("Low Water!"));
    display.setCursor(0, 50);
    display.print(F("Pump Stopped"));
  } else {
    display.print(F("Pump: ACTIVE"));
    display.setCursor(0, 50);
    display.print(F("Level: OK"));
  }

  // Mode dots
  for (int i = 0; i < TOTAL_MODES; i++) {
    if (i == currentMode)
      display.fillCircle(56 + i * 8, 62, 2, WHITE);
    else
      display.drawCircle(56 + i * 8, 62, 2, WHITE);
  }

  display.display();
}

// ---- Mode 2: Security Log ----
void displaySecurityMode() {
  display.clearDisplay();

  // Title bar
  display.fillRect(0, 0, SCREEN_WIDTH, 12, WHITE);
  display.setTextColor(BLACK);
  display.setTextSize(1);
  display.setCursor(20, 2);
  display.print(F("SECURITY LOG"));

  display.setTextColor(WHITE);

  // Intrusion count (large)
  display.setTextSize(3);
  display.setCursor(38, 18);
  display.print(intrusionCount);
  display.setTextSize(1);

  display.setCursor(20, 44);
  display.print(F("Total Intrusions"));

  // Current alert
  if (buzzerOn) {
    display.setCursor(10, 54);
    display.print(F(">> ALERT ACTIVE <<"));
  } else {
    display.setCursor(22, 54);
    display.print(F("Boundary: SAFE"));
  }

  // Mode dots
  for (int i = 0; i < TOTAL_MODES; i++) {
    if (i == currentMode)
      display.fillCircle(56 + i * 8, 62, 2, WHITE);
    else
      display.drawCircle(56 + i * 8, 62, 2, WHITE);
  }

  display.display();
}

// ============================================================
//  SPLASH SCREEN
// ============================================================
void showSplashScreen() {
  display.clearDisplay();
  display.setTextColor(WHITE);

  display.setTextSize(2);
  display.setCursor(14, 8);
  display.print(F("AgroShield"));

  display.setTextSize(1);
  display.setCursor(8, 30);
  display.print(F("Smart Farm Protection"));

  display.setCursor(22, 42);
  display.print(F("Initializing..."));

  // Loading bar animation
  for (int i = 0; i <= 100; i += 10) {
    display.fillRect(10, 54, i + 8, 6, WHITE);
    display.display();
    delay(80);
  }
}
