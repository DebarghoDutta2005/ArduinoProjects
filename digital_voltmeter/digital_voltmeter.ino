#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>   //install the library
#include <Wire.h>                //install the library
LiquidCrystal_I2C lcd(0x27,16,2); 
#define Sensorpin A0
void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

}

void loop() {
  int value = analogRead(Sensorpin);
  double voltage = map(value, 0, 1024, 0, 2500);
  voltage /=100;
  lcd.setCursor(1,0);
  lcd.print("Voltage :");
  lcd.print(voltage);
  lcd.print("V");
  Serial.println(voltage);
  delay(500);
}
