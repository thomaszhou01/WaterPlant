#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2); // set the LCD address to 0x3F for a 16 chars and 2 line display

int relayPin = 6;
int SensPin = A0;

unsigned long time1 = 0;
unsigned long time2 = 0;
int waitingPeriod1 = 5000;
int waitingPeriod2 = 3000;

int value = 0;
int turnOnPump = 0;
int readingCount = 0;
float sensorVal = 0;
float finalSensorVal = 0;
bool isPumping = false;
byte value1 = 0;
byte value2 = 0;
String sentVal = "";

//activates the pump for 3 seconds
void waterPlant()
{
  digitalWrite(relayPin, LOW);
  time2 = millis();
  isPumping = true;
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.init();
  lcd.clear();
  lcd.backlight();
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);
}

void loop()
{
  if (isPumping && (millis() >= time2 + waitingPeriod2))
  {
    digitalWrite(relayPin, HIGH);
    turnOnPump = 0;
  }

  if (Serial.available())
  {
    turnOnPump = Serial.read();
    if(turnOnPump == 1){
      waterPlant();
    }
  }

  //gather sensor readings
  sensorVal = sensorVal + analogRead(SensPin);
  readingCount++;
  if (readingCount == 100)
  {
    finalSensorVal = sensorVal / 100.0;
    readingCount = 0;
    sensorVal = 0;
  }

  //refresh LCD
  if (millis() >= time1 + waitingPeriod1)
  {
    time1 += waitingPeriod1;
    lcd.clear();
    if (finalSensorVal < 500)
    {
      waterPlant();
    }

    //send value to ESP8266
    value = finalSensorVal;
    value1 = value / 256;
    value2 = value % 256;
    Serial.write(value1);
    Serial.write(value2);

    // output to LCD
    lcd.print("Water: ");
    lcd.print(value);

    lcd.setCursor(0, 1);

    lcd.print("Test: ");
  }
}