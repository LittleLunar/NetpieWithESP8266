
#define MY_WIFI_NAME "YourWiFiName"
#define MY_WIFI_PASS "YourWiFiPass"

// Use the topic as you wish
#define SUBSCRIBE_TOPIC ""
#define PUBLISH_TOPIC ""

#define mqtt_server "broker.netpie.io"
#define mqtt_Client ""
#define mqtt_username ""
#define mqtt_password ""
#define mqtt_port 1883

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#define DHTPIN D1
#define DHTTYPE DHT11
#define LED1 16

const char* ssid = MY_WIFI_NAME;
const char* pass = MY_WIFI_PASS;

WiFiClient espClient;
PubSubClient client(espClient);

DHT dht(DHTPIN, DHTTYPE);

void reconnect() 
{
  
  while( !client.connected() ) 
  {
    Serial.print("Reconnecting...");
    
    if( client.connect(mqtt_Client, mqtt_username, mqtt_password ) )
    {
      Serial.println("Connected");
      client.subscribe(SUBSCRIBE_TOPIC);
    } 
    else
    {
      Serial.println("Connection failed... try again in 5 seconds");
      delay(5000);
    }
       
  }
       
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  String msg;
  
  for(int i = 0; i < length; i++)
    msg += (char)payload[i];
  
  Serial.println(msg);
  
  if(msg == "on") 
  {
    digitalWrite(LED1, 0);
    client.publish(PUBLISH_TOPIC, "");
    Serial.println("LED on");
  }
  else if(msg == "off") 
  {
    digitalWrite(LED1, 1);
    client.publish(PUBLISH_TOPIC, "");
    Serial.println("LED off");
  }
    
  
}
void setup()
{
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

void loop() 
{
  int humidity = dht.readHumidity();
  int temperature = dht.readTemperature();
  
  if ( !client.connected() ) 
    reconnect();
  
  String data = ""; // data to be sent that depend on schema properties in netpie device
  char msg[data.length() + 1];
  data.toCharArray(msg, data.length());
  
  client.publish(PUBLISH_TOPIC, msg);
  
  Serial.println("Send : [" + data + "]");
  delay(2000);
  
}
