//==============================================================
// Project : WifiGotchi
// Version : 1.0
// Creator : Elwan Mayencourt
// Date    : 01.12.2019
//==============================================================


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "[WifiGotchi]";
const char* password = "Emf12345";
int pushButton = 4;
String wifi = "";
int nbVlan  = 0;
ESP8266WebServer server(80); //Server on port 80


//===============================================================
//                     RESET
//===============================================================
void(* resetFunc) (void) = 0; //declare reset function @ address 0


//==============================================================
//     This rutine is exicuted when you open its IP in browser
//==============================================================
void handleRoot() {
  server.send(200, "text/plain", "\n""Wifi captured : " + String(nbVlan) + "\n" + wifi);
}


//===============================================================
//                     SCAN WIFI
//===============================================================
void scanWifi() {
  int n = WiFi.scanNetworks();
  //Serial.println("scan done");
  if (n == 0) {
  } else {
    for (int i = 0; i < n; ++i) {
      //result = result + WiFi.SSID(i) + "\n";
        wifi = wifi + WiFi.SSID(i) + "\n";
      nbVlan = nbVlan + 1;

    }
  }

  // Attend avant de tout faire peter
  digitalWrite(LED_BUILTIN, LOW);  
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);  
}

//===============================================================
//                     SETUP
//===============================================================
void setup(void) {

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  WiFi.mode(WIFI_AP);           //Only Access point
  WiFi.softAP(ssid, password);  //Start HOTspot removing password will disable security

  IPAddress myIP = WiFi.softAPIP(); //Get IP address

  //Start server

  while (digitalRead(pushButton) == 0) {
    scanWifi();
  }

  server.on("/", handleRoot);      //Which routine to handle at root location

  server.begin();
}

//===============================================================
//                     LOOP
//===============================================================
void loop(void) {
  int buttonState = digitalRead(pushButton);

  if (buttonState == 0) {
    resetFunc();
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);  
    server.handleClient();          

  }

}
