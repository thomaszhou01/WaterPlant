#define BLYNK_PRINT Serial

#include <Arduino.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "";
char pass[] = "";

BlynkTimer timer;
BlynkTimer timer2;
bool writeData = false;
int sensorVal = 0;
int pinData = 0;

void sendSensor()
{
  Blynk.virtualWrite(V6, sensorVal);  
}

void activatePump(){
  if(writeData){
    Serial.write(1);
    writeData = false;
  }
}

//read button
BLYNK_WRITE(V5) 
{
  pinData = param.asInt();
  if(pinData == 1){
    writeData = true;
  } 
}

void setup()
{
  Serial.begin(9600); // See the connection status in Serial Monitor
  Blynk.begin(auth, ssid, pass);

  // Setup a function to be called every second
  timer.setInterval(1500L, sendSensor);
  timer2.setInterval(100L, activatePump);
}

void loop()
{
  Blynk.run(); // Initiates Blynk
  timer.run(); // Initiates SimpleTimer  
  timer2.run();

  //receive values from arduino sensor
  if(Serial.available()>=2){
    byte byte1 = Serial.read();
    byte byte2 = Serial.read();

    sensorVal = (byte1*256) + byte2;
  }
}
