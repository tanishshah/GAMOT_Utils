#include <WiFi.h>


const char* SLIME = "slime";
const char* PELTH = "pelth";
const char* PELTC = "peltc";
const char* PIN = "pin";

void wifiSetup() {
  Serial.println("CONNECTING WIFI....");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500); // TODO: Create a way to make the wifi connection non-blocking during calibration
        Serial.print(".");
    }
    Serial.println("WiFi connected");
}

int makeGetRequest() {
  int action = -1; // by default do nothing
    WiFiClient client;
    if (client.connect(url, 8080)) {
        // Make a GET request
        Serial.println("Connected???");
        client.println("GET /get HTTP/1.1");
        client.println("Host: " + String(url));
        client.println("Connection: close");
        client.println();
    }

    // For simplicity, you can print it to the serial monitor
    while (client.connected()) {
        String line = client.readStringUntil('\n');
        const char* inp = line.c_str();
        Serial.println(line);
        // Check if we are interacting with a slime
        if (strstr(inp, SLIME) != NULL || strstr(inp, PIN) != NULL) {
          action = 1; // action for haptics
        } else if (strstr(inp, PELTH) != NULL) {
          action = 2; // action for peltier hot
        } 
         else if (strstr(inp, PELTC) != NULL) {
          action = 3; // action for peltier cold
        }
    }
    client.stop();
    return action;
}

void updateDB() {
if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;

    if (client.connect(url, 8080)) {
      // sample post data
      String postData = "{\"username\":\"your_username\",\"game\":\"your_game_name\",\"score\":100,\"duration\":120}";

      // TODO: Change the endpoint
      client.println("POST /sessions HTTP/1.1");
      client.println("Host: " + String(url));
      client.println("Content-Type: application/json");
      client.println("Content-Length: " + String(postData.length()));
      client.println();
      client.println(postData);
    }
  
    while (client.connected()) {
        String line = client.readStringUntil('\n');
        Serial.println(line);
    }
    client.stop();
  }
}

void sendSigState(int arr[]) {
  Serial.println("trying to send signal...");
  if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;

    if (client.connect(url, 8080)) {
      // sample post data
      String s1 = String(arr[0]);
      String s2 = String(arr[1]);
      String s3 = String(arr[2]);
      String s4 = String(arr[3]);
      String s5 = String(arr[4]);
      String s6 = String(arr[5]);
  
      String postData = "{\"flex1\":" + s1 + ",\"flex2\":" + s2 + ",\"flex3\":" + s3 + ",\"flex4\":" + s4 + ",\"pelt\":" + s5 + ",\"haptics\":" + s6 + "}";
      Serial.println(postData);
      // TODO: Change the endpoint
      client.println("POST /postmcu HTTP/1.1");
      client.println("Host: " + String(url));
      client.println("Content-Type: application/json");
      client.println("Content-Length: " + String(postData.length()));
      client.println();
      client.println(postData);
    }

    Serial.println("done printing");
    client.stop();
  }
}

/*

// use this code to test this code in a standalone way
void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected");
}

void loop() {
  sendSigState();
  delay(2000);
}
*/
