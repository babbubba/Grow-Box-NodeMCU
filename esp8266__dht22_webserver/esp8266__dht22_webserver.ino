// Import required libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include "Countimer.h"
#include <Wire.h>;
#include <LiquidCrystal_I2C.h>;
#include "NTPClient.h"
#include "WiFiUdp.h"
#include <EEPROM.h>

#include "index_html.h";

#include "persist_data.h";
#include "user_data.h";

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

// count none for update NTP time
Countimer ntpClientUpdateTimer;

// Used to identify first loop
bool firstLoop = true;

// Used to identify the first time the Rcounter of the Rele1 in unactive status is completed
bool firstTimeRele1OffExecuted = true;

// Set LCD columns
int lcdChars = 20;

// Output value for Rele 1 (Heating)
bool rele1ActiveStatus = false;

// Output value for Rele 2 (Light)
bool rele2ActiveStatus = false;


Countimer scheduledLightTimer;

// Temperature relay counter
Countimer rele1OnTimer;
Countimer rele1OffTimer;

// Se variable to permit or not to turn on rele1 (HEATING)
bool rele1CanTurnOn = false;

// Setting DHT22
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Start up LCD
LiquidCrystal_I2C lcd(0x27, lcdChars, 4);

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// DHT22 read count
Countimer dhtReadTimer;

void scrollTextLcd(int row, String message, int delayTime) {
  for (int i = 0; i < lcdChars; i++) {
    message = " " + message;
  }
  message = message + " ";
  for (int pos = 0; pos < message.length(); pos++) {
    lcd.setCursor(0, row);
    lcd.print(message.substring(pos, pos + lcdChars));
    delay(delayTime);
  }
}

void alignRightLcd(int row, String message) {
  int startPosition = lcdChars - message.length();
  if (startPosition < 0) {
    startPosition = 0;
  }
  for (int pos = 0; pos < startPosition; pos++) {
    message = " " + message;
  }
  lcd.setCursor(0, row);
  lcd.print(message);
}

void alignCenterLcd(int row, String message) {
  int startPosition = (lcdChars - message.length()) / 2;
  if (startPosition < 0) {
    startPosition = 0;
  }
  for (int pos = 0; pos < startPosition; pos++) {
    message = " " + message;
  }
  int rightSpaces = lcdChars - message.length();
  if (rightSpaces < 0) {
    rightSpaces = 0;
  }
  for (int pos = 0; pos < rightSpaces; pos++) {
    message = message + " ";
  }

  lcd.setCursor(0, row);
  lcd.print(message);
}


void writeTempNHumiLcd() {
  String text = "t:" + String(readTemperature) + "C - h:" + String(readHumidity) + "%";
  alignCenterLcd(1, text);
}

String getRele1StatusString() {
  if (rele1ActiveStatus == true) {
    return "Acceso";
  }
  return "Spento";
}

String getRele1IdelString() {
  if (rele1CanTurnOn == true) {
    return "Attivabile";
  }
  return "Sospeso";
}

String getRele2StatusString() {
  if (rele2ActiveStatus == true) {
    return "Acceso";
  }
  return "Spento";
}

String getLightManualString() {
  if (lightManual == true) {
    return "1";
  } else {
    return "0";
  }
}
String getTemeperatureControlActiveString() {
  if (temeperatureControlActive == true) {
    return "1";
  } else {
    return "0";
  }
}

void printRelaysStatusLcd() {
  lcd.setCursor(0, 2);
  lcd.print(" 1");
  if (rele1ActiveStatus == true) {
    lcd.write(1);
  } else {
    lcd.write(0);
  }
  lcd.setCursor(4, 2);
  lcd.print("| 2");
  if (rele2ActiveStatus == true) {
    lcd.write(1);
  } else {
    lcd.write(0);
  }
  lcd.setCursor(9, 2);
  lcd.print("| 3");
  lcd.write(0);
  lcd.setCursor(14, 2);
  lcd.print("| 4");
  lcd.write(0);
}

String getTimeFormatedFromIntegers(int hour, int minute) {
  String hourStr = String(hour);
  if (hourStr.length() == 1) {
    hourStr = "0" + hourStr;
  }
  String minuteStr = String(minute);
  if (minuteStr.length() == 1) {
    minuteStr = "0" + minuteStr;
  }

  return hourStr + ":" + minuteStr;
}

// Replaces placeholder in HTML with values (is called only first time in the setup method)
String processor(const String &var) {
  //Serial.println(var);
  if (var == "TEMPERATURE") {
    return String(readTemperature);
  } else if (var == "HUMIDITY") {
    return String(readHumidity);
  } else if (var == "RELE1") {
    return getRele1StatusString();
  } else if (var == "RELE1IDLE") {
    return getRele1IdelString();
  } else if (var == "RELE2") {
    return getRele2StatusString();
  } else if (var == "temperatureMin") {
    return String(temperatureMin);
  } else if (var == "temperatureMax") {
    return String(temperatureMax);
  } else if (var == "temperatureTol") {
    return String(temperatureTol);
  } else if (var == "tempActive") {
    return getTemeperatureControlActiveString();
  } else if (var == "heatingActiveForSeconds") {
    return String(heatingActiveForSeconds);
  } else if (var == "heatingIdleForSeconds") {
    return String(heatingIdleForSeconds);
  } else if (var == "lightManual") {
    return getLightManualString();
  } else if (var == "lightManualOnTime") {
    return getTimeFormatedFromIntegers(lightHStart, lightMStart);
  } else if (var == "lightManualOffTime") {
    return getTimeFormatedFromIntegers(lightHStop, lightMStop);
  }

  return String();
}

// Called when the count of Rele1 in active status is completed (it must to be turned off now) or when temperature is too high
void turnOffHeating() {
  if (rele1ActiveStatus == true) {
    rele1OnTimer.stop();
    rele1ActiveStatus = false;
    digitalWrite(RELE1, HIGH);
    Serial.println("Riscaldamento spento (o tempo accensione scaduto)");
    rele1CanTurnOn = false;
    rele1OffTimer.start();
  }
}

void turnOnLight_auto() {
  if (rele2ActiveStatus == false && lightManual == false) {
    rele2ActiveStatus = true;
    digitalWrite(RELE2, LOW);
    Serial.println("Luce accesa (temporizzata)");
  }
}

void turnOffLight_auto() {
  if (rele2ActiveStatus == true && lightManual == false) {
    rele2ActiveStatus = false;
    digitalWrite(RELE2, HIGH);
    Serial.println("Luce spenta (temporizzata)");
  }
}

void turnOnLight_manual() {
  if (rele2ActiveStatus == false) {
    rele2ActiveStatus = true;
    digitalWrite(RELE2, LOW);
    Serial.println("Luce accesa");
  }
}

void turnOffLight_manual() {
  if (rele2ActiveStatus == true) {
    rele2ActiveStatus = false;
    digitalWrite(RELE2, HIGH);
    Serial.println("Luce spenta");
  }
}

int getMinutesFromMidNight(int hour, int minute) {
  return (hour * 60) + minute;
}

void checkLightTimer() {
  timeClient.update();
  int currentMinute = timeClient.getMinutes();
  int currentHour = timeClient.getHours();

  int currentMinutes = getMinutesFromMidNight(currentHour, currentMinute);
  int lightStartMinutes = getMinutesFromMidNight(lightHStart, lightMStart);
  int lightStopMinutes = getMinutesFromMidNight(lightHStop, lightMStop);

  if (currentMinutes >= lightStartMinutes && currentMinutes < lightStopMinutes) {
    turnOnLight_auto();
  } else {
    turnOffLight_auto();
  }
}

// Called when need to turn on heating
void turnOnHeating() {
  if (rele1ActiveStatus == false) {
    rele1OnTimer.start();
    Serial.println("Riscaldamento attivato");
    rele1ActiveStatus = true;
    digitalWrite(RELE1, LOW);
  }
}

void updateTime() {
  timeClient.update();
  // Serial.print(daysOfTheWeek[timeClient.getDay()]);
  // Serial.print(", ");
  // Serial.print(timeClient.getHours());
  // Serial.print(":");
  // Serial.print(timeClient.getMinutes());
  // Serial.print(":");
  // Serial.println(timeClient.getSeconds());
  // Serial.println(timeClient.getFormattedTime());
  alignRightLcd(3, timeClient.getFormattedTime());
}
// Handle Rele 1 activation by temerature status (on if under the rage otherwise off)
void checkTemperature() {
  if (temeperatureControlActive == false) {
    turnOffHeating();
    return;
  }

  if ((readTemperature - temperatureTol) >= temperatureMin && (readTemperature + temperatureTol) <= temperatureMax) {
    // Temperature is in the range turn of the rele 1
    turnOffHeating();
  } else if ((readTemperature - temperatureTol) < temperatureMin) {
    //temeperature too low... turn on rele 1 if possible and if is not already active
    if (rele1OnTimer.isStopped() && rele1CanTurnOn == true && readTemperatureNotValid == false) {
      turnOnHeating();
    }
  } else {
    // Temperature is too high... turn off rele 1
    turnOffHeating();
  }
  // Print relays statuses on LCD
  printRelaysStatusLcd();
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

// Read the sensor temperature and humidity
void readDHT22() {
  // Read temperature
  float justReadTemperature = dht.readTemperature();
  if (isnan(justReadTemperature)) {
    // if temperature read failed, don't change the value
    Serial.println("Errore rilevando la temperatura");
    readTemperatureNotValid = true;
  } else {
    readTemperature = justReadTemperature;
    readTemperatureNotValid = false;
  }
  // Read Humidity
  float justReadHumidity = dht.readHumidity();
  // if humidity read failed, don't change the value
  if (isnan(justReadHumidity)) {
    Serial.println("Errore rilevando l'umidità");
    readHumidityNotValid = true;
  } else {
    readHumidity = justReadHumidity;
    readHumidityNotValid = false;
  }
  writeTempNHumiLcd();
  // after reading sensor elaborate relay 1 status
  checkTemperature();
}

void loadPersistentData() {
  Serial.println("Recupero dati persistenti...");
  lightManual = read_lightManual();
  if (lightManual == true) {
    Serial.println("Avvio luci: manuale");
  } else {
    Serial.println("Avvio luci: automatico");
  }

  lightHStart = read_lightManualOnTimeHour();
  lightMStart = read_lightManualOnTimeMinute();

  lightHStop = read_lightManualOffTimeHour();
  lightMStop = read_lightManualOffTimeMinute();

  temeperatureControlActive = read_temeperatureControlActive();
  temperatureMin = read_temperatureMin();
  temperatureMax = read_temperatureMax();
}

void handleSettingsPost(AsyncWebServerRequest *request) {
  bool needCommit = false;
  //lightManual
  if (request->hasParam("lightManual", true)) {
    String lightManualStr = request->getParam("lightManual", true)->value();
    if (lightManualStr == "1") {
      if (lightManual != true) {
        lightManual = true;
        write_lightManual(lightManual);
        needCommit = true;
      }
    } else {
      if (lightManual != false) {
        lightManual = false;
        write_lightManual(lightManual);
        needCommit = true;
      }
    }
  }

  //lightManualOnTime
  if (request->hasParam("lightManualOnTime", true)) {
    String lightManualOnTimeStr = request->getParam("lightManualOnTime", true)->value();
    int lightManualOnTimeHour = lightManualOnTimeStr.substring(0, 2).toInt();
    int lightManualOnTimeMinute = lightManualOnTimeStr.substring(3, 5).toInt();
    if (lightManualOnTimeHour != lightHStart || lightManualOnTimeMinute != lightMStart) {
      lightHStart = lightManualOnTimeHour;
      lightMStart = lightManualOnTimeMinute;
      write_lightManualOnTime(lightHStart, lightMStart);
      needCommit = true;
    }
  }

  //lightManualOffTime
  if (request->hasParam("lightManualOffTime", true)) {
    String lightManualOffTimeStr = request->getParam("lightManualOffTime", true)->value();
    int lightManualOffTimeHour = lightManualOffTimeStr.substring(0, 2).toInt();
    int lightManualOffTimeMinute = lightManualOffTimeStr.substring(3, 5).toInt();
    if (lightManualOffTimeHour != lightHStop || lightManualOffTimeMinute != lightMStop) {
      lightHStop = lightManualOffTimeHour;
      lightMStop = lightManualOffTimeMinute;
      write_lightManualOffTime(lightHStop, lightMStop);
      needCommit = true;
    }
  }

  //temeperatureControlActive (tempActive)
  if (request->hasParam("tempActive", true)) {
    String value = request->getParam("tempActive", true)->value();
    if (value == "1") {
      if (temeperatureControlActive != true) {
        temeperatureControlActive = true;
        write_temeperatureControlActive(temeperatureControlActive);
        needCommit = true;
      }
    } else {
      if (temeperatureControlActive != false) {
        temeperatureControlActive = false;
        write_temeperatureControlActive(temeperatureControlActive);
        needCommit = true;
      }
    }
  }

  //temperatureMin (tempMin)
  if (request->hasParam("tempMin", true)) {
    String value = request->getParam("tempMin", true)->value();
    int newTemperatureMin = value.toInt();
    if (newTemperatureMin != temperatureMin) {
      temperatureMin = newTemperatureMin;
      write_temperatureMin(temperatureMin);
      needCommit = true;
    }
  }

  //temperatureMax (tempMax)
  if (request->hasParam("tempMax", true)) {
    String value = request->getParam("tempMax", true)->value();
    int newTemperatureMax = value.toInt();
    if (newTemperatureMax != temperatureMax) {
      temperatureMax = newTemperatureMax;
      write_temperatureMax(temperatureMax);
      needCommit = true;
    }
  }

  if (needCommit) {
    if (EEPROM.commit()) {
      Serial.println("EEPROM successfully committed");
    } else {
      Serial.println("ERROR! EEPROM commit failed");
    }
  }

  AsyncWebServerResponse *response = request->beginResponse(302, "text/plain", "Ok");
  response->addHeader("Location", "/");
  request->send(response);
}

// The SETUP
void setup() {
  // Serial port
  Serial.begin(115200);

  // Initialize EEProm
  EEPROM.begin(EEPROM_SIZE);

  // Read data from EEPROM
  loadPersistentData();

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

  //Init LCD
  Wire.begin(SDA, SCL);
  lcd.begin();
  lcd.createChar(0, lockLcdChar);
  lcd.createChar(1, checkLcdChar);
  lcd.backlight();

  // Welcome Message
  scrollTextLcd(1, welcomeMessage, 300);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connessione a Wifi in corso");
  lcd.clear();
  lcd.print("Connessione Wifi");
  lcd.blink();
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    // lcd.print(".");
  }
  Serial.println(".");
  lcd.noBlink();

  // Print IP Address To Serial port
  Serial.print("Connessione Wifi riuscita! IP: ");
  Serial.println(WiFi.localIP());
  // Print ip address to LCD
  lcd.clear();
  String ip = "IP:" + WiFi.localIP().toString();
  alignCenterLcd(0, ip);

  // Start NTP client
  timeClient.begin();

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
    request->send_P(200, "text/plain", getRele1StatusString().c_str());
  });
  server.on("/rele1idle", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", getRele1IdelString().c_str());
  });

  //rele1CanTurnOn
  server.on("/rele2", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/plain", getRele2StatusString().c_str());
  });

  server.on("/settings", HTTP_POST, handleSettingsPost);

  // Start server
  server.begin();

  // Set the "Rele 1 active count down" (it will leave on Rele 1 for 'tot' seconds)
  rele1OnTimer.setCounter(0, 0, heatingActiveForSeconds, rele1OnTimer.COUNT_DOWN, turnOffHeating);
  // Set the callback function to call every second while "Rele 1 active count down" is active
  rele1OnTimer.setInterval(refreshRele1OnClock, 1000);

  // Set the "Rele 1 inactive count down" (it will leave of Rele 1 at least for 'tot' seconds)
  rele1OffTimer.setCounter(0, 0, heatingIdleForSeconds, rele1OffTimer.COUNT_DOWN, onRele1OffComplete);
  // Set the callback function to call every second while "Rele 1 inactive count down" is active
  // rele1OffTimer.setInterval(refreshRele1OffClock, 1000);

  // Execute the update of NTP client time every 1 second
  ntpClientUpdateTimer.setInterval(updateTime, 1000);

  // Ecxecute the check for the light scheduled timer
  scheduledLightTimer.setInterval(checkLightTimer, 1000);

  // Execute the read of dht 22 every 2 seconds
  dhtReadTimer.setInterval(readDHT22, 2000);
}

void loop() {
  // Execute the two counts down for Rele 1
  rele1OnTimer.run();
  rele1OffTimer.run();

  // Execute the none count (refreshing interval) for NTP time update
  ntpClientUpdateTimer.run();

  // Execute the none count (refreshing interval) counter for read dht22 every tot seconds
  dhtReadTimer.run();

  scheduledLightTimer.run();

  // Only if it is the first loop after power on NodeMcu starts the "Rele 1 inactive count down" (so heating can start untill this periond when u just poer on this module)
  // and the read DHT22 interval
  if (firstLoop == true) {
    // int the first loop this count down is used to mantain an idle period waiting sensor ar more stable
    rele1OffTimer.start();
    dhtReadTimer.start();
    ntpClientUpdateTimer.start();
    scheduledLightTimer.start();
    firstLoop == false;
  }
}