/*
********************************************
  ULTRASONIC DISTANCE MQTT SENSOR

  Author: Björn Makowe <bjoern.makowe@online.de>
  Date: 24th February 2018

  Version: 1.0
********************************************
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>


#define ultrasonic_echo         4 // D1
#define ultrasonic_trigger      5 // D2

#define pump_groundWater       14 // D5 groundwater pump
#define pump_gardenIrrigation  12 // D6 irrigation pump


const char* wifi_ssid = "...";
const char* wifi_password = "...";

const char* broker_uri = "openhabianpi";
const char* topic_groundwaterPump = "/italy/lestans/garden/irrigation/pumps/groundWater";
const char* topic_irrigationPump = "/italy/lestans/garden/irrigation/pumps/gardenIrrigation";
const char* topic_waterLevel = "/italy/lestans/garden/irrigation/sensors/waterLevel";


WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;



void setup() {
  pinMode(ultrasonic_echo, INPUT);
  pinMode(ultrasonic_trigger, OUTPUT);

  pinMode(pump_groundWater, OUTPUT);
  digitalWrite(pump_groundWater, HIGH);

  pinMode(pump_gardenIrrigation, OUTPUT);
  digitalWrite(pump_gardenIrrigation, HIGH);


  Serial.begin(115200);

  setup_wifi();

  client.setServer(broker_uri, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();

  if (now - lastMsg > 5000) {
    lastMsg = now;

    client.publish(topic_waterLevel, String(getDistance()).c_str());
  }
}

void setup_wifi() {
  delay(10);

  Serial.println("Connecting to ");
  Serial.print(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("...");
  }

  randomSeed(micros());

  Serial.println("connected");

  Serial.println();

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

double getDistance() {
  double duration, distance;

  digitalWrite(ultrasonic_trigger, LOW);
  delayMicroseconds(2);

  digitalWrite(ultrasonic_trigger, HIGH);
  delayMicroseconds(10);

  digitalWrite(ultrasonic_trigger, LOW);

  duration = pulseIn(ultrasonic_echo, HIGH);

  distance = ((duration / 2) / 29.1) / 100;

  return distance;
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if(strcmp(topic,topic_groundwaterPump) == 0) {
    if ((char)payload[0] == '1') {
      Serial.println("Pump ground water on.");
      digitalWrite(pump_groundWater, LOW);

    } else if ((char)payload[0] == '0') {
      Serial.println("Pump ground water off.");
      digitalWrite(pump_groundWater, HIGH);
    }
    
  } else if(strcmp(topic,topic_irrigationPump) == 0) {
    if ((char)payload[0] == '1') {
      Serial.println("Pump garden irrigation on.");
      digitalWrite(pump_gardenIrrigation, LOW);

    } else if ((char)payload[0] == '0') {
      Serial.println("Pump garden irrigation off.");
      digitalWrite(pump_gardenIrrigation, HIGH);
    }
} 
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    String clientId = "Irrigation-Controller";

    if (client.connect(clientId.c_str())) {
      Serial.println("connected");

      client.subscribe(topic_groundwaterPump);
      client.subscribe(topic_irrigationPump);

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      delay(5000);
    }
  }
}
