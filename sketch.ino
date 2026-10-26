
#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

#define DHTPIN 15
#define DHTTYPE DHT22

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* apiKey = "ِAPI_KEY";
const char* studentID = "12323450";

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password, 6);

  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read DHT22!");
  } else {
    Serial.print("Student ID: ");
    Serial.println(studentID);

    Serial.print("Temperature: ");
    Serial.println(temperature);

    Serial.print("Humidity: ");
    Serial.println(humidity);

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;

      String url = "http://api.thingspeak.com/update?api_key=";
      url += apiKey;
      url += "&field1=" + String(temperature, 2);
      url += "&field2=" + String(humidity, 2);

      http.begin(url);

      int httpCode = http.GET();

      if (httpCode > 0) {
        String response = http.getString();

        Serial.print("ThingSpeak Response: ");
        Serial.println(response);
      } else {
        Serial.print("HTTP Error: ");
        Serial.println(http.errorToString(httpCode));
      }

      http.end();
    }
  }

  delay(20000);
}
