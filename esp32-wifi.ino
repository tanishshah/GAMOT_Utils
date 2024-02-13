
/*
Code is a combination of (https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFiClientSecure/examples/WiFiClientSecureEnterprise/WiFiClientSecureEnterprise.ino)
and https://randomnerdtutorials.com/esp32-https-requests/          (ESP32 HTTPS Requests without Certificate)

*/

#include <WiFi.h>
#include <WiFiClientSecure.h>
#if __has_include ("esp_eap_client.h")
#include "esp_eap_client.h"
#else
#include "esp_wpa2.h"
#endif
#include <Wire.h>
#define EAP_IDENTITY "<user>@uwaterloo.ca"                  //user identity
#define EAP_ANONYMOUS_IDENTITY EAP_IDENTITY //anonymous identity, turns out eduroam doesnt like anonymous identities
#define EAP_PASSWORD "<password>" //eduroam user password
const char* ssid = "eduroam"; // eduroam SSID

const char* host = "https://www.howsmyssl.com"; //external server domain for HTTPS connection
int counter = 0;


WiFiClientSecure client;
void setup() {
  Serial.begin(115200);
  wifiExample();
}

void connectToWifi(){
    WiFi.disconnect(true);  //disconnect form wifi to set new wifi connection
  WiFi.mode(WIFI_STA); //init wifi mode
#if __has_include ("esp_eap_client.h")
  esp_eap_client_set_identity((uint8_t *)EAP_ANONYMOUS_IDENTITY, strlen(EAP_ANONYMOUS_IDENTITY)); //provide identity
  esp_eap_client_set_username((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY)); //provide username
  esp_eap_client_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD)); //provide password
  esp_wifi_sta_enterprise_enable();
#else
  esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_ANONYMOUS_IDENTITY, strlen(EAP_ANONYMOUS_IDENTITY)); //provide identity
  esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY)); //provide username
  esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD)); //provide password
  esp_wifi_sta_wpa2_ent_enable();
#endif
  WiFi.begin(ssid); //connect to wifi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    counter++;
    if (counter >= 60) { //after 30 seconds timeout - reset board (on unsucessful connection)
      ESP.restart();
    }
  }
  //client.setCACert(test_root_ca);

}

void wifiTest(){
   if (WiFi.status() == WL_CONNECTED) { //if we are connected to eduroam network
    counter = 0; //reset counter
    Serial.println("Wifi is still connected with IP: ");
    Serial.println(WiFi.localIP());   //inform user about his IP address
  } else if (WiFi.status() != WL_CONNECTED) { //if we lost connection, retry
    WiFi.begin(ssid);
  }
  while (WiFi.status() != WL_CONNECTED) { //during lost connection, print dots
    delay(500);
    Serial.print(".");
    counter++;
    if (counter >= 60) { //30 seconds timeout - reset board
      ESP.restart();
    }
  }
/*
    Serial.print("\nLocal ESP32 IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("\n");
  */
  Serial.print("Connecting to website: ");
  Serial.println(host);
  client.setInsecure();

  Serial.println("\nStarting connection to server...");
  if (!client.connect(host, 443))
    Serial.println("Connection failed!");
  else {
    Serial.println("Connected to server!");
    // Make a HTTP request:
    client.println("GET https://www.howsmyssl.com/a/check HTTP/1.0");
    client.println("Host: www.howsmyssl.com");
    client.println("Connection: close");
    client.println();

    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        Serial.println("headers received");
        break;
      }
    }
    // if there are incoming bytes available
    // from the server, read them and print them:
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }

    client.stop();

  }
}

void wifiExample(){
delay(10);
  Serial.println();
  Serial.print("Connecting to network: ");
  Serial.println(ssid);
  connectToWifi();
  //client.setCertificate(test_client_cert); // for client verification - certificate
  //client.setPrivateKey(test_client_key);  // for client verification - private key
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address set: ");
  Serial.println(WiFi.localIP()); //print LAN IP
  wifiTest();
}

void loop() {
 
}