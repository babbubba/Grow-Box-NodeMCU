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
#define RELE2 D5  // Luce
#define RELE3 D6  // D6
#define RELE4 D7  // D7

// Temperature
// Temerature Range
const float temperatureMin = 21;
const float temperatureMax = 24;
const float temperatureTol = 0.5;

// Temperature timing
const int heatingActiveForSeconds = 20;
const int heatingIdleForSeconds = 100;

// define variable to handle temperature & humidity
float readTemperature = 0.0;
bool readTemperatureNotValid = true;
float readHumidity = 0.0;
bool readHumidityNotValid = true;

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

