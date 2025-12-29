#include <Servo.h>
#include <DHT.h>

// ---------------- SERVO SETUP ----------------
Servo horizontal;
Servo vertical;

int servohori = 180;
int servohoriLimitHigh = 175;
int servohoriLimitLow = 5;

int servovert = 45;
int servovertLimitHigh = 100;
int servovertLimitLow = 1;

// ---------------- LDR SETUP ----------------
int ldrlt = A0;   // Top Left
int ldrld = A1;   // Bottom Left
int ldrrd = A2;   // Bottom Right
int ldrrt = A3;   // Top Right

// ---------------- DHT11 SETUP ----------------
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

int humidityThreshold = 69;   // %RH

// ---------------- MOSFET / HVG ----------------
int mosfetPin = 8;

// ---------------- DAY / NIGHT ----------------
int lightThreshold = 200;

// ---------------- HVG TIMING ----------------
unsigned long onDuration  = 300000UL;   // 5 min ON
unsigned long offDuration = 60000UL;    // 1 min OFF
unsigned long lastSwitchTime = 0;
bool hvgState = false;   // false = OFF, true = ON

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(9600);

  horizontal.attach(2);
  vertical.attach(13);
  horizontal.write(servohori);
  vertical.write(servovert);

  pinMode(mosfetPin, OUTPUT);
  digitalWrite(mosfetPin, LOW);

  dht.begin();
  delay(2000);

  Serial.println("System Initialized");
}

// ---------------- LOOP ----------------
void loop() {

  // -------- READ LDRs --------
  int lt = analogRead(ldrlt);
  int ld = analogRead(ldrld);
  int rd = analogRead(ldrrd);
  int rt = analogRead(ldrrt);

  int avt = (lt + rt) / 2;
  int avd = (ld + rd) / 2;
  int avl = (lt + ld) / 2;
  int avr = (rt + rd) / 2;

  int dvert = avt - avd;
  int dhoriz = avl - avr;

  int tol = 90;
  int totalLight = (lt + rt + ld + rd) / 4;

  // ================= DAY MODE =================
  if (totalLight > lightThreshold) {

    digitalWrite(mosfetPin, LOW);
    hvgState = false;

    // Vertical tracking
    if (abs(dvert) > tol) {
      servovert += (dvert > 0) ? 1 : -1;
      servovert = constrain(servovert, servovertLimitLow, servovertLimitHigh);
      vertical.write(servovert);
    }

    // Horizontal tracking
    if (abs(dhoriz) > tol) {
      servohori += (dhoriz < 0) ? 1 : -1;
      servohori = constrain(servohori, servohoriLimitLow, servohoriLimitHigh);
      horizontal.write(servohori);
    }
  }

  // ================= NIGHT MODE =================
  else {

    float humidity = dht.readHumidity();

    if (isnan(humidity)) {
      Serial.println("DHT11 ERROR");
      digitalWrite(mosfetPin, LOW);
      return;
    }

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    unsigned long now = millis();

    if (humidity >= humidityThreshold) {

      if (!hvgState && (now - lastSwitchTime >= offDuration)) {
        hvgState = true;
        digitalWrite(mosfetPin, HIGH);
        lastSwitchTime = now;
        Serial.println("HVG ON");
      }

      if (hvgState && (now - lastSwitchTime >= onDuration)) {
        hvgState = false;
        digitalWrite(mosfetPin, LOW);
        lastSwitchTime = now;
        Serial.println("HVG OFF (cooling)");
      }
    }
    else {
      digitalWrite(mosfetPin, LOW);
      hvgState = false;
    }
  }

  delay(10);
}
