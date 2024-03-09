#include <WiFi.h>

const char* url = ""; // enter ip address here 
const char* ssid = ""; // wifi connected to (hotspot)
const char* password = ""; //hotspot password
const char* SLIME = "slime";

void wifiSetup() {
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

    // Read and process the server response here
    // For simplicity, you can print it to the serial monitor
    while (client.connected()) {
        String line = client.readStringUntil('\n');
        const char* inp = line.c_str();
        Serial.println(line);
        // Check if we are interacting with a slime
        if (strstr(inp, SLIME) != NULL) {
          action = 1; // action for haptics
        }
    }
    client.stop();
    return action;
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
  makeGetRequest();
}
*/
