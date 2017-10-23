// Import required libraries
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <aREST.h>

// Clients
WiFiClient espClient;
PubSubClient client(espClient);

// Create aREST instance
aREST rest = aREST(client);

// Unique ID to identify the device for cloud.arest.io
char* device_id = "23ddcc";

// WiFi parameters
const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";

// LED state
bool ledState;

// Functions
void callback(char* topic, byte* payload, unsigned int length);
int toggle(String command);

void setup(void)
{
  
  // Start Serial
  Serial.begin(115200);

  // Set callback
  client.setCallback(callback);

  // Give name and ID to device
  rest.set_id(device_id);
  rest.set_name("led");

  // Function
  rest.function("toggle", toggle);

  // LED state
  ledState = false;

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Pin 5 as output
  pinMode(D5, OUTPUT);
  
}

void loop() {

  // Connect to the cloud
  rest.handle(client);

}

// Handles message arrived on subscribed topic(s)
void callback(char* topic, byte* payload, unsigned int length) {

  // Handle
  rest.handle_callback(client, topic, payload, length);

}

// Toggle LED
int toggle(String command) {

  ledState = !ledState;
  digitalWrite(D5, ledState);
  return 1;
}
