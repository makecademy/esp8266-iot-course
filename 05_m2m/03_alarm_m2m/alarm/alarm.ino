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
char* device_id = "25tyif";

// WiFi parameters
const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";

// Alarm
bool alarmState;
int ledState = LOW; 
unsigned long previousMillis = 0; 
const long interval = 1000;

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
  rest.set_name("alarm");

  // Function
  rest.function("alarm", alarm);

  // Alarm state
  alarmState = false;

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
  
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    if (alarmState == true) {
      digitalWrite(D5, ledState);
    }
    
  }

  // Connect to the cloud
  rest.handle(client);

}

// Handles message arrived on subscribed topic(s)
void callback(char* topic, byte* payload, unsigned int length) {

  // Handle
  rest.handle_callback(client, topic, payload, length);

}

// Activate alarm
int alarm(String command) {

  alarmState = true;
  return 1;
}
