#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SoftwareSerial.h>
using namespace std;

char readbuffer [64];
const char* ssid = "Arpita";
const char* password = "arpita12";
WiFiClient clientGet;
AsyncWebServer server(80);

SoftwareSerial nano(3,1); //RX, TX
//WEBSITE CODE
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>PBU Dashboard</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 3.0rem;}
    p {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 6px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 3px}
    input:checked+.slider {background-color: #b30000}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  </style>
</head>
<body>
  <h2>Welcome to Analytics Dashboard</h2><br><br>
  %GRAPHPLACEHOLDER%
</body>
</html>
)rawliteral";

String processor(String var){
  //Serial.println(var);
  if(var == "GRAPHPLACEHOLDER"){
    String graph = "";
    
    int i = 0; //initilaize iterator
    while(nano.available()){
      readbuffer[i] = nano.read();
      i++;
    }
    readbuffer[i+1] = '\0';
    graph += "<h4>Reaction Time: " + String(readbuffer);
    return graph;
  }
  return String();
}

void setup(){
  nano.begin(115200);
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(".");
  }
  Serial.println(WiFi.localIP());
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  // Send a GET request to <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
  // server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
  //   request->send(200, "text/plain", "OK");
  // });
  server.begin(); // Start server
}

void loop() {
  // float reactiontime = nano.read();
  // clientGet.print(reactiontime);
}
