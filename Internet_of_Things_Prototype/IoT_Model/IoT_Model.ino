#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "DHT.h"
#define DHTPIN D2       // pin DHT connected to
#define DHTTYPE DHT11   // define type of sensor DHT 11
DHT dht (DHTPIN, DHTTYPE);

#define drip_irrigate D5    // connect LED at D5 to demonstrate
const int sensor_pin = A0;  // Connect Soil moisture analog sensor pin to A0 of NodeMCU 

//DEVICE CREDENTIALS
#define ORG "*****"
#define DEVICE_TYPE "NODEMCU"
#define DEVICE_ID "*****"
#define TOKEN "*******"

char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char pubtopic[] = "iot-2/evt/data/fmt/json";
char topic[] = "iot-2/cmd/status/fmt/String";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
//Serial.println(clientID);
String command;

const char* ssid = "*****";
const char* password = "*****";

WiFiClient wifiClient;
void callback(char* topic, byte* payload, unsigned int payloadLength);
PubSubClient client(server, 1883, callback, wifiClient);

void setup() {
    pinMode(drip_irrigate, OUTPUT); // Drip irrigation turned on and off depending on moisture level
    Serial.begin(115200);
    dht.begin();

    Serial.println();
    wifiConnect();
    mqttConnect();
}

void loop() {
    delay(5000);
    
    float moisture_percentage;
    moisture_percentage = ( 100.00 - ( (analogRead(sensor_pin)/1023.00) * 100.00 ) );
    Serial.print("Soil Moisture(in Percentage) = ");
    Serial.print(moisture_percentage);
    Serial.println("%");
    
    if(moisture_percentage < 50){
      digitalWrite(drip_irrigate, HIGH);
      Serial.println("Drip Irrigation is turned ON");
    }
    
    else if(moisture_percentage > 50){
      Serial.println("Drip Irrigation is turned OFF");  
      digitalWrite(drip_irrigate, LOW);
    }

    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
  
    Serial.print("Humidity: ");
    Serial.println(humidity);
    Serial.print("Temperature: ");
    Serial.println(temperature);    
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
     }

    if (!client.loop()) {
      mqttConnect();
      }
    PublishData(temperature, humidity, moisture_percentage);
    delay(100);
}

void wifiConnect() {
    Serial.print("Connecting to "); Serial.print(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
    Serial.print("\n WiFi connected, IP address: "); 
    Serial.println(WiFi.localIP());
}

void mqttConnect() {
    if (!client.connected()) {
    Serial.print("Reconnecting MQTT client to "); 
    Serial.println(server);
    while (!client.connect(clientId, authMethod, token)) {
        Serial.print(".");
        delay(500);
      }
    initManagedDevice();
    Serial.println();
    }
  }

void initManagedDevice() {
    if (client.subscribe(topic)) {
        Serial.println("subscribe to cmd OK");
    } else {
        Serial.println("subscribe to cmd FAILED");
  }
}

void callback(char* topic, byte* payload, unsigned int payloadLength) {
    Serial.print("callback invoked for topic: "); 
    Serial.println(topic);

    for (int i = 0; i < payloadLength; i++) {
        command += (char)payload[i];
    }
    Serial.println(command);
    if(command == "{\"status\":\"on\"}"){
      digitalWrite(drip_irrigate, HIGH);
      Serial.println("Drip Irrigation is ON");
    }
    else if(command == "{\"status\":\"off\"}"){
      Serial.println("Drip Irrigation is OFF");  
      digitalWrite(drip_irrigate, LOW);
    }
    command = "";
}

void PublishData(float temperature, float humidity, float moisture_percentage){
    if (!!!client.connected()) {
      Serial.print("Reconnecting client to ");
      Serial.println(server);
      while (!!!client.connect(clientId, authMethod, token)) {
          Serial.print(".");
          delay(500);
      }
      Serial.println();
    }
    
    String payload = "{\"d\":{\"temperature\":";
    payload += temperature;
    payload += "," "\"humidity\":";
    payload += humidity;
    payload += "," "\"moisture_percentage\":";
    payload += moisture_percentage;
    payload += "}}";
    Serial.print("Sending payload: ");
    Serial.println(payload);
  
    if (client.publish(pubtopic, (char*) payload.c_str())) {
        Serial.println("Publish ok");
    } else {
        Serial.println("Publish failed");
    }
}
