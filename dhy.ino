#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>
#include <DHT.h>
#include <stdlib.h>

#define RELAYPIN 4
#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_PCF8574 lcd(0x27);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  int error;

  Serial.begin(9600);
  Serial.println("LCD...");

  while (! Serial);

  Serial.println("Dose: check for LCD");

  // See http://playground.arduino.cc/Main/I2cScanner
  Wire.begin();
  Wire.beginTransmission(0x27);
  error = Wire.endTransmission();
  Serial.print("Error: ");
  Serial.print(error);

  if (error == 0) {
    Serial.println(": LCD found.");

  } else {
    Serial.println(": LCD not found.");
  } // if

  lcd.begin(16, 2); // initialize the lcd
  
  lcd.setBacklight(64);
  
  dht.begin();

  pinMode(RELAYPIN, OUTPUT);
} // setup()

void loop()
{
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  bool on = (t < 45 || h > 50) && (h > 10) && (t < 60);

  digitalWrite(RELAYPIN, on ? HIGH : LOW);
  
  char buff[100];
  
  lcd.clear();
  lcd.setCursor(0, 0);
  
  snprintf(buff, sizeof(buff), "T: %dC, %dF", (int)(t), (int)(f));
  lcd.print(buff);
  
  snprintf(buff, sizeof(buff), "H: %d%%  R: %s", (int)(h), on ? "ON" : "OFF");
  lcd.setCursor(0, 1);
  lcd.print(buff);


  delay(2000);
} // loop()
