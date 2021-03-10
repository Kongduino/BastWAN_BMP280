#undef max
#undef min
#include <string>

#include "Seeed_BMP280.h"
// http://librarymanager/All#Seeed_BMP280
#include <string>
#include <Wire.h>
#include <ArduinoJson.h>

#define INTERVAL 2000
double t0;
float p0 = 102110;
BMP280 bmp280;

void setup() {
  SerialUSB.begin(115200);
  SerialUSB.flush();
  delay(3000);
  SerialUSB.println("\n\nBMP280");
  if (!bmp280.init()) {
    Serial.println("Device error!");
  }
  t0 = millis();
}

void loop() {
  double t1 = millis();
  if (t1 - t0 > INTERVAL) {
    float p1;
    // TODO Add code to read from Serial (or else) and change the value

    //get and print temperatures
    Serial.print("Temp: ");
    float t = bmp280.getTemperature();
    Serial.print(t);
    Serial.println("C");
    //get and print atmospheric pressure data
    Serial.print("MSL: ");
    Serial.print(p0 / 100.0);
    Serial.println(" HPa");
    Serial.print("Pressure: ");
    p1 = bmp280.getPressure();
    Serial.print(p1 / 100.0);
    Serial.println(" HPa");
    //get and print altitude data
    Serial.print("Altitude according to the lib (snort): ");
    Serial.print(bmp280.calcAltitude(p1));
    Serial.println("m");
    //get and print altitude data
    Serial.print("Real altitude: ");
    Serial.print(calcAltitude(p0, p1, t));
    Serial.println("m");
    Serial.println("\n");
    t0 = millis();
  }
  if (Serial.available()) {
    char mb[255];
    uint8_t ix = 0;
    while (Serial.available()) {
      mb[ix++] = Serial.read();
    }
    mb[ix] = 0;
    Serial.println("Incoming:");
    Serial.println(mb);
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, mb);
    if (!error) {
      float newMSL = doc["MSL"];
      Serial.print("New MSL: "); Serial.println(newMSL);
      if (newMSL > 0.0) {
        p0 = newMSL * 100;
      }
    } else {
      Serial.println("Parse error!");
    }
  }
}
// the library's calcAltitude() method is useless: it uses a fixed MSL value that you cannot change
// Customized version: pass p0: MSL, p1: current Pa, and t: Temperature
// It calculates the altitude quite accurately
float B = 1 / 5.25588;
float calcAltitude(float p0, float p1, float t) {
  float C = pow((p0 / p1), B) - 1.0;
  // C = C / 0.0000225577;
  // The above seems to work slightly less accurately
  return (C * (t + 273.15)) / 0.0065;
}
