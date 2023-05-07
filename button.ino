#include <WiFi.h>
#include <PubSubClient.h>

#include <stdint.h> // to use uint32_t
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

uint32_t key[4] = {0xA56BABCD, 0x56781234, 0xDEF91234, 0x12345678}; // 128-bit key
char lon1[19];
char lon2[19];
const char* str1 = "kagotawa";
const char* str2 = "bapaogin";
const char* mqtt_topic = "button_signal";

// defining button used in the project
#define BUTTON_PIN1 21  // GIOP21 pin connected to button
#define BUTTON_PIN2 5  // GIOP5 pin connected to button

// Variables will change:
//int lastState = LOW;  // the previous state from the input pin
int calon1 = LOW;     // the current reading from the input pin
int calon2 = LOW; 

// Update these with values suitable for your network.

const char* ssid = "Yang itu";
const char* password = "passwordnya";
const char* mqtt_server = "91.121.93.94";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

char* encipher(unsigned int pengulangan, char buffer[17], uint32_t const key[4]) {
    unsigned int i;
    uint32_t v[2];
    char* encrypted_value = (char*) malloc(sizeof(char) * 17); // allocate memory for the string
    sscanf(buffer, "%8X%8X", &v[0], &v[1]); 
    uint32_t v0 = v[0], v1 = v[1], sum = 0, delta = 0x9E3779B9;
    for (i = 0; i < pengulangan; i++) {
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
        sum += delta;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum >> 11) & 3]);
    }
    v[0] = v0; v[1] = v1;
    sprintf(encrypted_value, "%08X%08X", v[0], v[1]);
    return encrypted_value;
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("mqtt_topic", "You are connected");
      // ... and resubscribe
      //client.subscribe("incobapmc");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(2000);
    }
  }
}

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(3600);
  pinMode(BUTTON_PIN1, INPUT_PULLUP);
  pinMode(BUTTON_PIN2, INPUT_PULLUP);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  int len = strlen(str1);
  for (int i = 0; i < len; i++) {
      sprintf(&lon1[2*i], "%02X", str1[i]); // convert each character in the input string to its hex representation
  }
  lon1[2*len] = '\0'; // add null terminator

  len = strlen(str2);
  for (int i = 0; i < len; i++) {
      sprintf(&lon2[2*i], "%02X", str2[i]); // convert each character in the input string to its hex representation
  }
  lon2[2*len] = '\0'; // add null terminator
  // read the state of the switch/button:
  calon1 = digitalRead(BUTTON_PIN1);
  calon2 = digitalRead(BUTTON_PIN2);

  if (calon1 == HIGH && calon2 == LOW){
    //snprintf("Voting untuk Pak Ogin");
    snprintf (msg, MSG_BUFFER_SIZE, "%s", encipher(32, lon1, key));
    Serial.println(msg);
    client.publish("mqtt_topic", msg);
  }

  else if (calon1 == LOW && calon2 == HIGH){
    //snprintf("Voting untuk Kak Gota");
    snprintf (msg, MSG_BUFFER_SIZE, "%s", encipher(32, lon2, key));
    Serial.println(msg);
    client.publish("mqtt_topic", msg);
  }
  delay(150);
}