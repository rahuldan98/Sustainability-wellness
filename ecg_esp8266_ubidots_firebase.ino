#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <FirebaseArduino.h>
#define WIFISSID "Rahul_Dan"                                // Put your WifiSSID here
#define PASSWORD "rahuldan@98"                                 // Put your wifi password here
#define TOKEN "BBFF-sNM3XlaQjxIe8FwNOgUx4CWBzOdGeR"         // Put your Ubidots' TOKEN
#define MQTT_CLIENT_NAME "myecgsensor"                       // MQTT client Name, please enter your own 8-12 alphanumeric character ASCII string; 
#define FIREBASE_HOST "sustainability-and-welln-b52d5-default-rtdb.firebaseio.com"  // the project name address from firebase id
#define FIREBASE_AUTH "Mn791wcBacviiMKhkcIncu2JQpOJixehhHOgjK4l"                                                         //it should be a random and unique ascii string and different from all other devices
 
/****************************************
 * Define Constants
 ****************************************/
#define VARIABLE_LABEL "ecg_sensor_data" // Assing the variable label
#define DEVICE_LABEL "ecg_system" // Assig the device label
 
#define SENSOR A0 // Set the A0 as SENSOR
 
char mqttBroker[]  = "industrial.api.ubidots.com";
char payload[100];
char topic[150];
// Space to store values to send
char str_sensor[10];
 
/****************************************
 * Auxiliar Functions
 ****************************************/
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
      delay(2000);
    }
  }
}
 
/****************************************
 * Main Functions
 ****************************************/
void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFISSID, PASSWORD);
  // Assign the pin as INPUT 
  pinMode(SENSOR, INPUT);
 
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
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
 
void loop() {
  if (!client.connected()) {
    reconnect();
  }
 
  sprintf(topic, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
  sprintf(payload, "%s", ""); // Cleans the payload
  sprintf(payload, "{\"%s\":", VARIABLE_LABEL); // Adds the variable label
  
  float myecg = analogRead(SENSOR); 
  
  /* 4 is mininum width, 2 is precision; float value is copied onto str_sensor*/
  dtostrf(myecg, 4, 2, str_sensor);
  
  sprintf(payload, "%s {\"value\": %s}}", payload, str_sensor); // Adds the value
  Serial.println("Publishing data to Ubidots Cloud");
  client.publish(topic, payload);
  client.loop();
   String ecgvalstr=String(myecg);
  Firebase.pushString("/ecg val", ecgvalstr);
  if (Firebase.failed()) 
    {
 
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error()); 
      return;
  }
  Serial.println("Waiting to upload next reading...");
  Serial.println();
  delay(1000);
}
