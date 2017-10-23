// Import required libraries
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <aREST.h>
#include "DHT.h"

// Clients
WiFiClient espClient;
PubSubClient client(espClient);

// Create aREST instance
aREST rest = aREST(client);

// Pin
#define DHTPIN 4

// Use DHT11
#define DHTTYPE DHT11

// Create instance
DHT dht(DHTPIN, DHTTYPE);

// Unique ID to identify the device for cloud.arest.io
char* device_id = "87fcff";

// WiFi parameters
const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";

// Functions
void callback(char* topic, byte* payload, unsigned int length);

// Variables to be exposed to the API
float temperature;
float humidity;

void setup(void)
{
  // Start Serial
  Serial.begin(115200);

   // Init DHT
  dht.begin();

  // Set callback
  client.setCallback(callback);

  // Give name & ID to the device (ID should be 6 characters long)
  rest.set_id(device_id);
  rest.set_name("esp8266");

  rest.variable("temperature", &temperature);
  rest.variable("humidity", &humidity);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

}

void loop() {

  // Reading temperature & humidity
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  // Connect to the cloud
  rest.handle(client);

}

// Handles message arrived on subscribed topic(s)
void callback(char* topic, byte* payload, unsigned int length) {

  rest.handle_callback(client, topic, payload, length);

}
