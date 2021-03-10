/*
   bmp280_example.ino
   Example sketch for BMP280
   Copyright (c) 2016 seeed technology inc.
   Website    : www.seeedstudio.com
   Author     : Lambor, CHN
   Create Time:
   Change Log :
   The MIT License (MIT)
   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:
   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
*/
#include "Seeed_BMP280.h"
// http://librarymanager/All#Seeed_BMP280
#include <Wire.h>

BMP280 bmp280;

void setup() {
  SerialUSB.begin(115200);
  SerialUSB.flush();
  delay(3000);
  SerialUSB.println("\n\nBMP280");
  if (!bmp280.init()) {
    Serial.println("Device error!");
  }
}

void loop() {
  float p0, p1;
  p0 = 102180; // MSL
  // TODO Add code to read from Serial (or else) and change the value

  //get and print temperatures
  Serial.print("Temp: ");
  float t = bmp280.getTemperature();
  Serial.print(t);
  Serial.println("C");
  //get and print atmospheric pressure data
  Serial.print("Pressure: ");
  p1 = bmp280.getPressure();
  Serial.print(p1 / 100.0);
  Serial.println("HPa");
  //get and print altitude data
  Serial.print("Altitude according to the lib (snort): ");
  Serial.print(bmp280.calcAltitude(p1));
  Serial.println("m");
  //get and print altitude data
  Serial.print("Real altitude: ");
  Serial.print(calcAltitude(p0, p1, t));
  Serial.println("m");
  Serial.println("\n");
  delay(1000);
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
