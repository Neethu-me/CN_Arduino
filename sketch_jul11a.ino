// ----------(c) Electronics-Project-Hub -----------//
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
// #include <ACS712.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
uint32_t currentFrequency;
int int_busvoltage = 0;
int x, y, z;
float busvoltage = 10;
float current_mA = 0;
float power_mW = 0;
unsigned long previousMillis_V = 0;
unsigned long previousMillis_I = 0;
unsigned long previousMillis_P = 0;
const int Field_number_1 = 1;
const int Field_number_2 = 2;
const int Field_number_3 = 3;
bool measure_V = true;
bool measure_I = true;
bool measure_P = true;

//------- WI-FI details ----------//
char ssid[] = "Neethu"; //SSID here
char pass[] = "pvdt1668"; // Password here
//--------------------------------//

//----------- Channel details ----------------//
unsigned long Channel_ID =2217651; // Your Channel ID
const char * WriteAPIKey = "FUQVQ8TSBCNR3SA5"; //Your write API key
//-------------------------------------------//

WiFiClient  client;

void setup()
{
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("IoT Based Energy");
  lcd.setCursor(0, 1);
  lcd.print("Monitoring System");
  delay(2000);
  // if (! acs712.begin())
  // {
  //   lcd.clear();
  //   lcd.setCursor(0, 0);
  //   lcd.print("FAILED TO FIND");
  //   lcd.setCursor(0, 1);
  //   lcd.print("ACS712 MODULE");
  //   while (1)
  //   {
  //     delay(10);
  //   }
  // }
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
}

void loop()
{
  internet();
  measure();
  upload();
}

void internet()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    while (WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, pass);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("CONNECTING TO");
      lcd.setCursor(0, 1);
      lcd.print("Wi-Fi NETWORK...");
      delay(5000);
    }
  }
}

void measure()
{
  busvoltage = 10;
  int_busvoltage = busvoltage * 1000;
  // current_mA = acs712.getCurrent_mA();
  // power_mW = acs712.getPower_mW();
  current_mA = 9;
  power_mW = 4;
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("V:");
  lcd.print(busvoltage);
  lcd.setCursor(9, 1);
  lcd.print("mA:");
  (current_mA < 3) ? (lcd.print("000")) : (current_mA < 10 && current_mA > 3) ? (lcd.print("00"), lcd.print((int)current_mA)) : (current_mA < 100 && current_mA >= 10) ?
  (lcd.print('0'), lcd.print((int)current_mA)) : (lcd.print((int)current_mA));
  lcd.setCursor(0, 0);
  lcd.print("POWER(mW):");
  lcd.print((int)power_mW);
  delay(2000);
}

void upload()
{
  // unsigned long currentMillis_V = millis();
  unsigned long currentMillis_V = 20000;
  if (currentMillis_V - previousMillis_V >= 15000 && measure_V == true)
  {
    previousMillis_V = currentMillis_V;
    x = ThingSpeak.writeField(Channel_ID, Field_number_1, busvoltage, WriteAPIKey);
    // if (x == 200)
    // {
    //   measure_V = false;
    //   measure_I = true;
    // }
  }

  unsigned long currentMillis_I = 23000;
  if (currentMillis_I - previousMillis_I >= 15000 && measure_I == true)
  {
    previousMillis_I = currentMillis_I;
    y = ThingSpeak.writeField(Channel_ID, Field_number_2, current_mA , WriteAPIKey);
    // if (y == 200)
    // {
    //   measure_I = false;
    //   measure_P = true;
    // }
  }

  unsigned long currentMillis_P = 30000;
  if (currentMillis_P - previousMillis_P >= 15000 && measure_P == true)
  {
    previousMillis_P = currentMillis_P;
    z = ThingSpeak.writeField(Channel_ID, Field_number_3, power_mW, WriteAPIKey);
    // if (z == 200)
    // {
    //   measure_P = false;
    //   measure_V = true;
    // }
  }
}
// ----------(c) Electronics-Project-Hub -----------//
