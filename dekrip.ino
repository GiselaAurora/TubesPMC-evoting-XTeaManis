#include <WiFi.h>
#include <PubSubClient.h>

#include <stdint.h> // to use uint32_t
#include <string.h>
#include <stdlib.h>
int calon1 = 0;
int calon2 = 0;
int suara1 = 1;
int suara2 = 2;
int len = 17;
int pengulangan = 32;
char str1[17] = "kagotawa"; // 
char str2[17] = "bapaogin";
char decrypted[33];

uint32_t key[4] = {0xA56BABCD, 0x56781234, 0xDEF91234, 0x12345678}; // 128-bit key
// Update these with values suitable for your network.

const char* ssid = "TubagusLT2++";
const char* password = "kosanxv1b2";
const char* mqtt_server = "91.121.93.94"; //test.mosqutto.org

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	17
char msg[MSG_BUFFER_SIZE];
int value = 0;

void hexToString(char* hex, char* str, int hex_len) {
    int i;
    for (i = 0; i < hex_len; i += 2) {
        unsigned int hex_val;
        sscanf(&hex[i], "%2x", &hex_val);
        sprintf(&str[i/2], "%c", hex_val);
    }
}

int decipher(unsigned int pengulangan, char password[17], uint32_t const key[4]) {
    unsigned int i;
    char buffer[17];
    char output[9]; // hex to string
    uint32_t v[2];
    char decrypted_str[17];
    sscanf(password, "%8X%8X", &v[0], &v[1]); // dari password pada akun.txt dipisah jadi dua
    uint32_t v0=v[0], v1=v[1], delta=0x9E3779B9, sum=delta*pengulangan;
    for (i=0; i < pengulangan; i++) {
        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
        sum -= delta;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
    }
    v[0]=v0; v[1]=v1;
    sprintf(decrypted_str, "%08X%08X", v[0], v[1]);
    /*for (i = 0; i < sizeof(decrypted_value)/sizeof(char)/2; i++) {
        sscanf(&decrypted_str[2*i], "%2X", (unsigned int*)&output[i]);  // Read two hex characters at a time
    }*/
    hexToString(decrypted_str, output, 16);
    if (strcmp(output, str1)==0){
      return suara1;
    }
    else if (strcmp(output, str2)==0){
      return suara2;
    }
    
    //printf("Hasil decrypt : %08X%08X\n", v[0], v[1]);
    //printf("Password anda: %s\n", output);
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

void callback(char* topic, unsigned char* payload, unsigned int len) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  // Print the payload as a string
  for (int i = 0; i < len; i++) {
    Serial.print((char)payload[i]);
  }
  strncpy(msg, (char*)payload, len);
  //snprintf (msg, MSG_BUFFER_SIZE, "%s", decipher(32, msg, key));
  int temp = decipher(pengulangan, msg, key);
  if (temp == 1)
    calon1 ++;
  else if (temp == 2)
    calon2 ++;
  Serial.print(" == ");

  Serial.print(calon1);
  Serial.print(calon2);
  printf("\n");

  if(calon1>calon2){
    digitalWrite(18, HIGH);
    digitalWrite(4, LOW);
    }
  else if(calon1<calon2){
    digitalWrite(18, LOW);
    digitalWrite(4, HIGH); 
    }
  else {
    digitalWrite(18, LOW);
    digitalWrite(4, LOW); 
  }

  delay(1000);
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      //karena esp32 ini hanya menerima pesan maka 
      //hanya dihubungkan dengan subscribe
      client.subscribe("mqtt_topic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(18, OUTPUT);
  pinMode(4, OUTPUT);    // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}