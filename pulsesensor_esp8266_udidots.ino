#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define ssid "Rahul_Dan"                                // Put your WifiSSID here
#define password "rahuldan@98" 
#define TOKEN "BBFF-sNM3XlaQjxIe8FwNOgUx4CWBzOdGeR"
#define MQTT_CLIENT_NAME "myecgsensor"
#define VARIABLE_LABEL "pulse_sensor_data"                          // Assing the variable label
#define DEVICE_LABEL "ecg_system"                                      // Assig the device label
#define PulseSensorPin A0
char mqttBroker[]  = "industrial.api.ubidots.com";
char payload[100];
char topic[150];
// Space to store values to send
char str_sensor[10];                                   
WiFiClient ubidots;
PubSubClient client(ubidots);

void callback(char* topic, byte* payload, unsigned int length) {
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  Serial.write(payload, length);
  Serial.println(topic);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    
    // Attemp to connect
    if (client.connect(MQTT_CLIENT_NAME, TOKEN, "")) {
      Serial.println("Connected");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      // Wait 2 seconds before retrying
      delay(4000);
    }
  }
}
  void setup()
  {​​​​
   Serial.begin(115200);
  WiFi.begin(ssid, password);
  // Assign the pin as INPUT 
  pinMode(PulseSensorPin,INPUT);
 
  Serial.println();
  Serial.print("Waiting for WiFi...");
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    
  }
  
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqttBroker, 1883);
  client.setCallback(callback);  
 
  }
   
  void loop()
 /* {​​​
    if (!client.connected()) {
    reconnect();
  }
  sprintf(topic, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
  sprintf(payload, "%s", ""); // Cleans the payload
  sprintf(payload, "{\"%s\":", VARIABLE_LABEL); // Adds the variable label
  
  val = analogRead(PulseSensorpin); //Read Analog values and Store in val variable
  Serial.println("Pulse Sensorvalue= "); // Start Printing on Pulse sensor value on LCD
  Serial.println(val); // Start Printing on Pulse sensor value on LCD
   dtostrf(val, 4, 2, str_sensor);
   sprintf(payload, "%s {\"value\": %s}}", payload, str_sensor); // Adds the value
  Serial.println("Publishing data to Ubidots Cloud");
  client.publish(topic, payload);
  client.loop();
  delay(1000);
  }*/
  {
  if (!client.connected()) {
    reconnect();
  }
 
  sprintf(topic, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
  sprintf(payload, "%s", ""); // Cleans the payload
  sprintf(payload, "{\"%s\":", VARIABLE_LABEL); // Adds the variable label
  
  int val = analogRead(PulseSensorpin);
  Serial.print("Pulse Sensorvalue= "); // Start Printing on Pulse sensor value on LCD
  Serial.println(val); // Start Printing on Pulse sensor value on LCD
  /* 4 is mininum width, 2 is precision; float value is copied onto str_sensor*/
  dtostrf(val, 4, 2, str_sensor);
  
  sprintf(payload, "%s {\"value\": %s}}", payload, str_sensor); // Adds the value
  Serial.println("Publishing data to Ubidots Cloud");
  client.publish(topic, payload);
  client.loop();
}
 
