
#define MY_WIFI_NAME "YOUR_WIFI_NAME"
#define MY_WIFI_PASS "YOUR_WIFI_PASS"

#define SUBSCRIBE_TOPIC ""
#define PUBLISH_TOPIC ""


#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#define DHTPIN D1
#define DHTTYPE DHT11
#define LED1 16

const char* ssid = MY_WIFI_NAME;
const char* pass = MY_WIFI_PASS;
const char* mqtt_server = "broker.netpie.io";
const int mqtt_port = 1883;
const char* mqtt_Client = "";
const char* mqtt_username = "";
const char* mqtt_password = "";

WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);
char msg[50];

void reconnect() {
  while(!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if(client.connect(mqtt_Client, mqtt_username, mqtt_password) {
      Serial.println("Connected");
      client.subscribe(SUBSCRIBE_TOPIC);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String message;
  for(int i = 0; i < length; i++) {
    message = message + (char)payload[i];
  }
  Serial.println(message);
  if(String(topic) == SUBSCRIBE_TOPIC) {
    if(message == "on") {
      digitalWrite(LED1, 0);
      client.publish(PUBLISH_TOPIC, "");
      Serial.println("LED on");
    }
    else if(message == "off") {
      digitalWrite(LED1, 1);
      client.publish(PUBLISH_TOPIC, "");
      Serial.println("LED off");
    }
  }
}
void setup(){
  
  Serial.begin(115200);

  pinMode(LED1, OUTPUT);
  
  Serial.print("\nConnecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP address : ");
  Serial.println(WiFi.localIP());
  
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  
  dht.begin();
}

void loop() {
  int humidity = dht.readHumidity();
  int temperature = dht.readTemperature();
  if (!client.connected()) {
    reconnect();
  }
  client.stop();
  String data = "";
  Serial.println(data);
  data.toCharArray(msg, (data.length() + 1);
  client.publish(PUBLISH_TOPIC, msg);
  delay(2000);
  
}
