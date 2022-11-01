#include <DHTesp.h>

#include <Sparkfun_APDS9301_Library.h>
#include <SparkFun_SCD30_Arduino_Library.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>


// Replace the next variables with your SSID/Password combination
const char* ssid = "redmadrobot";
const char* password = "a1b2c3d4e5";

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "90.188.115.54";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

SCD30 airSensor;
DHTesp dht;
APDS9301 apds;

float temperature = 0;
float humidity = 0;
float tempDHT = 0;
float humDHT = 0;
float co2 = 0;

// Pins assignment
const int ledPin = 4;
int dhtPin = 18;

void setup() {

  Wire.setPins(26,27);
  Wire.begin();
  Serial.begin(115200);
  dht.setup(dhtPin, DHTesp::DHT11);
  
  // Initialize light sensor
  // apds.begin(0x39);
  // apds.setGain(APDS9301::LOW_GAIN);
  // apds.setIntegrationTime(APDS9301::INT_TIME_13_7_MS);
  // apds.setLowThreshold(0);
  // apds.setHighThreshold(50);
  // Serial.println(apds.getLowThreshold());
  // Serial.println(apds.getHighThreshold());

  // Test light sensor
  // while(1){
  //   Serial.print("Luminous flux: ");
  //   Serial.println(apds.readCH0Level(),6);
  //   delay(500);
  // }
  

  //Try to initialize SCD30
  Serial.println("Try to initialize!");
  if (airSensor.begin() == false)
  {
    Serial.println("Air sensor not detected. Please check wiring. Freezing...");
    while (1)
      ;
  }
  Serial.println("SCD30 Found!");

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "esp32/output") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(ledPin, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(ledPin, LOW);
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    //if (client.connect("ESP32Client","esp32","123456_654321")) {
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 3000) {
    lastMsg = now;

  	// Check if any reads failed and exit early (to try again).
	if (dht.getStatus() != 0) {
		Serial.println("DHT11 error status: " + String(dht.getStatusString()));		
	}
    
  // Temperature in Celsius
  temperature = airSensor.getTemperature();   
  
  char tempString[8];
  dtostrf(temperature, 1, 2, tempString);
  Serial.print("Temperature: ");
  Serial.println(tempString);

  humidity = airSensor.getHumidity();
  
  char humString[8];
  dtostrf(humidity, 1, 2, humString);
  Serial.print("Humidity: ");
  Serial.println(humString);
  
  char co2String[8];
  dtostrf(co2, 1, 2, co2String);
  Serial.print("CO2: ");
  Serial.println(co2String);

  TempAndHumidity newValues = dht.getTempAndHumidity();

  sprintf(msg, "{\"temperature\":%2.1f,\"humidity\":%2.0f, \"co2\":%3.0f, \"tempDHT\":%3.0f, \"humDHT\":%3.0f}", temperature, humidity, co2, newValues.temperature, newValues.humidity);

  Serial.println(msg);
  client.publish("mqtt_1/data", msg);    
    
  }
}