#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <stdint.h>


#define ApSSID    "IHD"
#define ApPass    "1234567890"


IPAddress local_IP(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);


int RxIndex;


WiFiServer server(1217);
WiFiClient client;


/*=========================================================
  this function check the eeprom to find the SSID and PASS
  if find it ,set value on the Global variable and report 1,
  else report 0*/


void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.setDebugOutput(false);
    WiFi.setAutoConnect(false);
    wifi_set_phy_mode(PHY_MODE_11G);
    WiFi.mode(WIFI_AP);
    WiFi.disconnect(true);
    WiFi.softAPdisconnect(false);

    Serial.print("Setting soft-AP ... ");
    WiFi.softAPConfig(local_IP, gateway, subnet);
    boolean result = WiFi.softAP(ApSSID, ApPass);
    if (result == true)
    {
      //Serial.println("Ready");
     // Serial.println(WiFi.softAPIP());
    }
    else
    {
      //Serial.println("Failed!");
    }
}


void serialCheck(){
 if (Serial.available() > 0) {
        client.print((char)(Serial.read()));  
     //   client.flush();
      }  
  
  }
 void clientCheck(){
    if (client.available()) {
            Serial.print((char)client.read());
            Serial.flush();  
          }  
    
    
    
    }
void loop() {
  
    if (WiFi.softAPgetStationNum() > 0) {
      server.begin();
      while (!client) {
        client = server.available();
        //Serial.print(".");
       // delay(1000);
      }
      if (client)
      {
        while (client.connected()) {
          clientCheck();
          serialCheck();
        
        }
        client.stop();
      }
    }
  

 
}
