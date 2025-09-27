#include <Servo.h>

// --- Servo Setup ---
Servo horizontal;
Servo vertical;

int servohori = 180;
int servohoriLimitHigh = 175;
int servohoriLimitLow = 5;

int servovert = 45;
int servovertLimitHigh = 100;
int servovertLimitLow = 1;

// --- LDR Setup ---
int ldrlt = A0;  // Top Left
int ldrrt = A3;  // Top Right
int ldrld = A1;  // Bottom Left
int ldrrd = A2;  // Bottom Right

// --- Humidity Sensor ---
int humidityPin = A4;  // Analog input
int humidityThreshold = 600;  // Adjust experimentally (0-1023)

// --- MOSFET Control ---
int mosfetPin = 8;  

// --- Day/Night Detection ---
int lightThreshold = 200; // Adjust experimentally

// --- HVG Timing Control ---
unsigned long onDuration = 300000UL;   // 5 minutes (ms)
unsigned long offDuration = 60000UL;   // 1 minute (ms)
unsigned long lastSwitchTime = 0;
bool hvgState = false;

void setup() {
  Serial.begin(9600);

  horizontal.attach(2);
  vertical.attach(13);
  horizontal.write(servohori);
  vertical.write(servovert);

  pinMode(mosfetPin, OUTPUT);
  digitalWrite(mosfetPin, LOW);  // HVG OFF initially

  delay(2000);
}

void loop() {
  // --- Read LDR values ---
  int lt = analogRead(ldrlt);
  int rt = analogRead(ldrrt);
  int ld = analogRead(ldrld);
  int rd = analogRead(ldrrd);

  int avt = (lt + rt) / 2;
  int avd = (ld + rd) / 2;
  int avl = (lt + ld) / 2;
  int avr = (rt + rd) / 2;

  int dvert = avt - avd;
  int dhoriz = avl - avr;

  int tol = 90;
  int dtime = 10;

  int totalLight = (lt + rt + ld + rd) / 4;

  if (totalLight > lightThreshold) {
    // ---------- DAY MODE ----------
    digitalWrite(mosfetPin, LOW);  // HVG OFF during daytime
    hvgState = false;              // reset state machine

    // Sun Tracking
    if (abs(dvert) > tol) {
      if (avt > avd) {
        servovert++;
        if (servovert > servovertLimitHigh) servovert = servovertLimitHigh;
      } else {
        servovert--;
        if (servovert < servovertLimitLow) servovert = servovertLimitLow;
      }
      vertical.write(servovert);
    }

    if (abs(dhoriz) > tol) {
      if (avl > avr) {
        servohori--;
        if (servohori < servohoriLimitLow) servohori = servohoriLimitLow;
      } else {
        servohori++;
        if (servohori > servohoriLimitHigh) servohori = servohoriLimitHigh;
      }
      horizontal.write(servohori);
    }
  } 
  else {
    // ---------- NIGHT MODE ----------
    int humidityValue = analogRead(humidityPin);
    Serial.print("Humidity: ");
    Serial.println(humidityValue);

    if (humidityValue > humidityThreshold) {
      unsigned long currentMillis = millis();

      if (hvgState) { 
        // HVG currently ON
        if (currentMillis - lastSwitchTime >= onDuration) {
          hvgState = false;
          digitalWrite(mosfetPin, LOW); 
          lastSwitchTime = currentMillis;
          Serial.println("HVG OFF (cooling)");
        }
      } else {
        // HVG currently OFF
        if (currentMillis - lastSwitchTime >= offDuration) {
          hvgState = true;
          digitalWrite(mosfetPin, HIGH);
          lastSwitchTime = currentMillis;
          Serial.println("HVG ON (fog collection)");
        }
      }
    } 
    else {
      digitalWrite(mosfetPin, LOW); // Low humidity → HVG stays OFF
      hvgState = false;
    }
  }

  delay(dtime);
}
