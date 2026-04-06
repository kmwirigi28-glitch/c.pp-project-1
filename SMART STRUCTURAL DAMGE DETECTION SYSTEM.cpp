/*
 * Structural Damage Detection System
 * Detects cracks, rust, corrosion, overpressure, and thermal damage
 * Author: Kelvin Mwirigi 
 */

#include <LiquidCrystal.h>
 
// LCD pins: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

// ----- Sensor Pins -----
const int tempPin = A0;      // LM35 Temperature Sensor
const int pressurePin = A1;  // Pressure Sensor (MPX5010)
const int vibrationPin = A2; // Piezo Vibration Sensor
const int corrosionPin = A3; // Corrosion Sensor (analog)

// ----- Output Pins -----
const int buzzer = 8;
const int led = 9;

// ----- Threshold Values -----
float tempThreshold = 60.0;       // °C
float pressureThreshold = 2.5;    // Volts (adjust per sensor calibration)
int vibrationThreshold = 300;     // Analog value (shock detection)
int corrosionThreshold = 400;     // Analog value (wet/corroded)

void setup() {
  // Initialize LCD
  lcd.begin(16, 2);
  lcd.print("Structural Health");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  
  // Setup Serial Monitor
  Serial.begin(9600);
  
  // Output pins
  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);
  
  delay(2000);
  lcd.clear();
  lcd.print("System Ready");
  delay(1000);
}

void loop() {
  // ---- Read Sensors ----
  int tempRaw = analogRead(tempPin);
  float tempVolt = tempRaw * (5.0 / 1023.0);
  float temperature = tempVolt * 100.0; // LM35 = 10mV per °C

  int pressureRaw = analogRead(pressurePin);
  float pressureVolt = pressureRaw * (5.0 / 1023.0);

  int vibration = analogRead(vibrationPin);
  int corrosion = analogRead(corrosionPin);

  // ---- Flags for each damage type ----
  bool overTemp = false, overPress = false, crack = false, rust = false;

  // ---- Decision Logic ----
  if (temperature > tempThreshold) overTemp = true;
  if (pressureVolt > pressureThreshold) overPress = true;
  if (vibration > vibrationThreshold) crack = true;
  if (corrosion > corrosionThreshold) rust = true;

  // ---- Display results on Serial Monitor ----
  Serial.println("----- SENSOR READINGS -----");
  Serial.print("Temp: "); Serial.print(temperature); Serial.println(" °C");
  Serial.print("Pressure Voltage: "); Serial.print(pressureVolt); Serial.println(" V");
  Serial.print("Vibration: "); Serial.println(vibration);
  Serial.print("Corrosion: "); Serial.println(corrosion);
  Serial.println("----------------------------");

  // ---- Display on LCD ----
  lcd.clear();
  lcd.setCursor(0, 0);

  if (overTemp) {
    lcd.print("Thermal Damage!");
    alarm();
  } 
  else if (overPress) {
    lcd.print("Overpressure!");
    alarm();
  } 
  else if (crack) {
    lcd.print("Crack Detected!");
    alarm();
  } 
  else if (rust) {
    lcd.print("Corrosion/Rust!");
    alarm();
  } 
  else {
    lcd.print("All Systems OK");
    digitalWrite(buzzer, LOW);
    digitalWrite(led, LOW);
  }

  lcd.setCursor(0, 1);
  lcd.print("T:"); lcd.print(temperature); lcd.print("C ");
  lcd.print("P:"); lcd.print(pressureVolt, 1); lcd.print("V");

  delay(1000); // Wait 1 second before next check
}

// ----- Alert Function -----
void alarm() {
  digitalWrite(buzzer, HIGH);
  digitalWrite(led, HIGH);
  delay(500);
  digitalWrite(buzzer, LOW);
  digitalWrite(led, LOW);
  delay(500);
}
