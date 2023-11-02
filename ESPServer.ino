#include <ESP8266WiFi.h> // lalala
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>
#include "index.h"

SoftwareSerial nano(3,1); //RX, TX

const char* ssid = "Arpita";
const char* password = "arpita12";
ESP8266WebServer server(80); //Server on port 80

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}
void handledata() {
 int data;
 String test = "";
 String sendstring = "";
 if(nano.available()){
  test = nano.readStringUntil('\n'); //can use to check which test is being received
  data = nano.parseInt();
 }
 
  if(test[0] == 'v'){
    sendstring = "Visual Memory Level ->" + String(data);
  }
  else if(test[0] == 's'){
    sendstring = "Sequence Memory Level ->" + String(data);
  }
  else if(test[0] == 'r'){
    sendstring = "Reaction time ->" + String(data) + " ms";
  }

 server.send(200, "text/plain", sendstring); //Send ADC value only to client ajax request

}


//==============================================================
//                  SETUP
//==============================================================
void setup(void){
  Serial.begin(115200);
  nano.begin(9600);
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
  
  server.on("/", handleRoot);      //Which routine to handle at root location
  server.on("/index", handledata);      //Which routine to handle at root location

  server.begin();                  //Start server
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();          //Handle client requests
}
