#ifndef USER_DATA_H
#define USER_DATA_H

// Wireless credential
const char *ssid = "bHome.Wifi";
const char *password = "0EvItyr6mSIFP1FpIDMuwWqb";

// Welcome text (scrolling)
String welcomeMessage = "Benvenuto in B-Grow!";

//time (NTP)
const long utcOffsetInSeconds = 3600;  //UTC +1 - Rome
// char daysOfTheWeek[7][12] = { "Domenica", "Lunedi", "Martedi", "Mercoledi", "Giovedi", "Venerdi", "Sabato" };


// Define ESP8266 GDPIO -> PIN
#define D0 16
#define D1 5
#define D2 4
#define D3 0
#define D4 2
#define D5 14
#define D6 12
#define D7 13
#define D8 15

// IC2 Pins
#define SCL D1
#define SDA D2  

// DHT22 Pin
#define DHTPIN D3  

// RELAYS PINS
#define RELE1 D0  // Riscaldamento
//inverto temporaneamente il rele 2 con il 3 per capire se ho un problema sul modulo relè ed il secondo si è bruciato
#define RELE2 D6  // Luce
#define RELE3 D5  // D6
#define RELE4 D7  // D7

// Temperature
// Temerature Range (Day)
float temperatureMin = 23;
float temperatureMax = 28;
float temperatureTol = 0.5;
// Temerature Range (Night)
float temperatureMinNight = 19;
float temperatureMaxNight = 23;
float temperatureTolNight = 0.4;
bool temeperatureControlActive = false;

// Temperature timing
int heatingActiveForMinutes = 1;
int heatingActiveForSeconds = 15;
int heatingIdleForMinutes = 2;
int heatingIdleForSeconds = 00;

// define variable to handle temperature & humidity
float readTemperature = 0.0;
bool readTemperatureNotValid = true;
float readHumidity = 0.0;
bool readHumidityNotValid = true;


// Lighting setting
int lightHStart = 0;
int lightMStart = 0;
int lightHStop = 0;
int lightMStop = 0;
bool lightManual = false;

byte checkLcdChar[] = {
  B00000,
  B00001,
  B00011,
  B10110,
  B11100,
  B01000,
  B00000,
  B00000
};

byte lockLcdChar[] = {
  B01110,
  B10001,
  B10001,
  B11111,
  B11011,
  B11011,
  B11111,
  B00000
};

#endif