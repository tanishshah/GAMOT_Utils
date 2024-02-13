
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

  int count = 0;

  Serial.println("\nConnecting to Wifi");
  while (WiFi.status() != WL_CONNECTED) {
      count++;
      connectWifi();
      
    }
    
  
  //client.setCACert(test_root_ca);

}

unsigned long prevWifiAttemptTime = 0;
int WifiConnectionAttempts = 0;

//Checks if connected to wifi. If not, tries different fixes. If these do not work then the board will eventually restart
void connectWifi(){

//try various things to fix the wifi connection
  if(WiFi.status() != WL_CONNECTED){
    unsigned long curTime = millis();
    int interval = 5000;
  //WiFi takes real time to connect. Dont try starting anything new if we havent given enough time to establish a connection
    if (curTime-prevWifiAttemptTime >= interval){
        prevWifiAttemptTime=curTime;
      //actually try connecting again
      if(WiFi.status() != WL_CONNECTED){
        WiFi.disconnect(true);
        WiFi.begin(ssid);
        WifiConnectionAttempts++;
      }
    }
  }
//reset WifiConnectionAttempts iff the board is now connected to wifi
  if(WiFi.status()==WL_CONNECTED){
    WifiConnectionAttempts=0;
  }

//Restart the board if still cannot get a wifi connection. At some point may want to change this to a different criteria
  if(WifiConnectionAttempts>50 && WiFi.status() != WL_CONNECTED){
    Serial.print("\n\nERROR: Could not connect to WiFi\n\n");
    ESP.restart();
  }

}


void wifiExampleTest(){

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
  wifiExampleTest();
}

void loop() {
 
}