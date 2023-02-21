// Import required libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include "Countimer.h"

// Used to identify first loop
bool firstLoop = true;

// Used to identify the first time the Rcounter of the Rele1 in unactive status is completed
bool firstTimeRele1OffExecuted = true;

// Wireless credential
const char *ssid = "bHome.Wifi";
const char *password = "0EvItyr6mSIFP1FpIDMuwWqb";

// DHT Pin
#define DHTPIN 5  // Digital pin connected to the DHT sensor (D1)

// RELAYS PINS
#define RELE1 16  // D0 (Heating)
#define RELE2 14  // D5
#define RELE3 12  // D6
#define RELE4 13  // D7

// Setting Temerature Range
const float temperatureMin = 23;
const float temperatureMax = 24;
const float temperatureTol = 0.4;

// Output value for Rele 1 (Heating)
char rele1On[] = { "N/D" };

// Temperature relay counter
Countimer rele1OnTimer;
Countimer rele1OffTimer;
const int heatingActiveForSeconds = 30;
const int heatingIdleForSeconds = 60;

// Se variable to permit or not to turn on rele1 (HEATING)
bool rele1CanTurnOn = false;

// Setting DHT22
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// define variable to handle temperature & humidity
float readTemperature = 0.0;
bool readTemperatureNotValid = true;
float readHumidity = 0.0;
bool readHumidityNotValid = true;


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// DHT22 read count
Countimer dhtReadTimer;

unsigned long previousMillis = 0;  // will store last time DHT was updated
// Set the updates interval of DHT readings
const long interval = 10000;

// the HTML page
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p.measure { font-size: 3.0rem; }
    p.small { font-size: 0.8rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>GrowBox bHome</h2>
  <p class="small">
  Il riscladamento rimane attivo per %heatingActiveForSeconds% secondi ed ha un periodo di inattivit&aacute; minimo di %heatingIdleForSeconds% secondi (all'avvio sar&aacute; inattivo per %heatingIdleForSeconds% secondi).
  </p>
  <p class="small">
  Range temperatura: %temperatureMin% &deg;C / %temperatureMax% &deg;C (&plusmn; %temperatureTol% &deg;C).
  </p>
  <p class="measure">
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="dht-labels">Temperatura</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p class="measure">
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="dht-labels">Umidit&#224</span>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units">&#37;</sup>
  </p>
  <p class="measure">
    <i class="fas fa-fire" style="color:#ff6600;"></i> 
    <span class="dht-labels">Riscaldamento</span>
    <span id="rele1">%RELE1%</span>
  </p>
  

</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 1000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 1000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("rele1").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/rele1", true);
  xhttp.send();
}, 1000 ) ;
</script>
</html>)rawliteral";

// Replaces placeholder in HTML with values (is called only first time in the setup method)
String processor(const String &var) {
  //Serial.println(var);
  if (var == "TEMPERATURE") {
    return String(readTemperature);
  } else if (var == "HUMIDITY") {
    return String(readHumidity);
  } else if (var == "RELE1") {
    return String(rele1On);
  } else if (var == "temperatureMin") {
    return String(temperatureMin);
  } else if (var == "temperatureMax") {
    return String(temperatureMax);
  } else if (var == "temperatureTol") {
    return String(temperatureTol);
  } else if (var == "heatingActiveForSeconds") {
    return String(heatingActiveForSeconds);
  } else if (var == "heatingIdleForSeconds") {
    return String(heatingIdleForSeconds);
  }


  //temperatureTol
  return String();
}

// Handle Rele 1 activation by temerature status (on if under the rage otherwise off)
void checkTemperature() {
  if ((readTemperature - temperatureTol) >= temperatureMin && (readTemperature + temperatureTol) <= temperatureMax) {
    // Temperature is in the rage turn of the rele 1
    rele1OnTimer.stop();
    strcpy(rele1On, "OFF");
    digitalWrite(RELE1, HIGH);
    rele1CanTurnOn = true;
  } else if ((readTemperature - temperatureTol) < temperatureMin) {
    //temeperature too low... turn on rele 1 if possible and if is not already active
    if (rele1OnTimer.isStopped() && rele1CanTurnOn == true && readTemperatureNotValid == false) {
      rele1OnTimer.start();
      Serial.println("Riscaldamento attivato");
      strcpy(rele1On, "ON");
      digitalWrite(RELE1, LOW);
    }
  } else {
    // Temperature is too high... turn off rele 1
    rele1OnTimer.stop();
    strcpy(rele1On, "OFF");
    digitalWrite(RELE1, HIGH);
    rele1CanTurnOn = true;
  }
}

// Called when the count of Rele1 in active status is completed (it must to be turned off now)
void onRele1OnComplete() {
  strcpy(rele1On, "OFF");
  digitalWrite(RELE1, HIGH);
  Serial.println("Tempo di accensione del riscaldamento scaduto");
  rele1CanTurnOn = false;
  rele1OffTimer.start();
}

// Called when the count of Rele1 in unactive status is completed (it can be turned on now)
void onRele1OffComplete() {
  rele1CanTurnOn = true;
  firstTimeRele1OffExecuted = false;
}

// Called every second to show Rele1 in active status on serial monitor
void refreshRele1OnClock() {
  Serial.print("Tempo di accensione del riscaldamento rimasto: ");
  Serial.println(rele1OnTimer.getCurrentTime());
}

// Called every second to show Rele1 in unactive status on serial monitor (only first time)
void refreshRele1OffClock() {
  if (firstTimeRele1OffExecuted == false) return;
  Serial.print("Tempo di attesa del riscaldamento rimasto: ");
  Serial.println(rele1OffTimer.getCurrentTime());
}

// Read the sensor temperature and humidity
void readDHT22() {
  // Read temperature
  float justReadTemperature = dht.readTemperature();
  if (isnan(justReadTemperature)) {
    // if temperature read failed, don't change the value
    readTemperatureNotValid = true;
  } else {
    readTemperature = justReadTemperature;
    readTemperatureNotValid = false;
  }
  // Read Humidity
  float justReadHumidity = dht.readHumidity();
  // if humidity read failed, don't change the value
  if (isnan(justReadHumidity)) {
    readHumidityNotValid = true;
  } else {
    readHumidity = justReadHumidity;
    readHumidityNotValid = false;
  }

  // after reading sensor elaborate relay 1 status
  checkTemperature();
}

// The SETUP
void setup() {
  // Serial port
  Serial.begin(115200);

  // Init the DHT 22 sensor
  dht.begin();

  // Set the pin mode for relays output digital pin and set them as default to HIGH (rele is open so it is off)
  pinMode(RELE1, OUTPUT);
  digitalWrite(RELE1, HIGH);
  pinMode(RELE2, OUTPUT);
  digitalWrite(RELE2, HIGH);
  pinMode(RELE3, OUTPUT);
  digitalWrite(RELE3, HIGH);
  pinMode(RELE4, OUTPUT);
  digitalWrite(RELE4, HIGH);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connessione a Wifi in corso");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(".");

  // Print IP Address To Serial port
  Serial.print("Connessione Wifi riuscita! IP: ");
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html, processor);
  });

  // Routes to other values
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", String(readTemperature).c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", String(readHumidity).c_str());
  });
  server.on("/rele1", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", String(rele1On).c_str());
  });

  // Start server
  server.begin();

  // Set the "Rele 1 active count down" (it will leave on Rele 1 for '30' seconds)
  rele1OnTimer.setCounter(0, 0, heatingActiveForSeconds, rele1OnTimer.COUNT_DOWN, onRele1OnComplete);
  // Set the callback function to call every second while "Rele 1 active count down" is active
  rele1OnTimer.setInterval(refreshRele1OnClock, 1000);

  // Set the "Rele 1 inactive count down" (it will leave of Rele 1 at least for '60' seconds)
  rele1OffTimer.setCounter(0, 0, heatingIdleForSeconds, rele1OffTimer.COUNT_DOWN, onRele1OffComplete);
  // Set the callback function to call every second while "Rele 1 inactive count down" is active
  rele1OffTimer.setInterval(refreshRele1OffClock, 1000);

  // Execute the read of dht 22 every 2 seconds
  dhtReadTimer.setInterval(readDHT22, 2000);
}

void loop() {
  // Execute the two counts down for Rele 1
  rele1OnTimer.run();
  rele1OffTimer.run();

  // Execute the none count (refreshing interval) counter for read dht22 every tot seconds
  dhtReadTimer.run();

  // Only if it is the first loop after power on NodeMcu starts the "Rele 1 inactive count down" (so heating can start untill this periond when u just poer on this module)
  // and the read DHT22 interval
  if (firstLoop == true) {
    rele1OffTimer.start();
    dhtReadTimer.start();
    firstLoop == false;
  }
}