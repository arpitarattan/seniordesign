#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <SoftwareSerial.h>

SoftwareSerial nano(3,1); //RX, TX
// Replace with your network credentials
const char* ssid     = "Arpita";
const char* password = "arpita12";

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "https://pbu.com/post-esp-data.php";

// Keep this API Key value to be compatible with the PHP code provided in the project page. 
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key 
String apiKeyValue = "tPmAT5Ab3j7F9";

void setup() {
  nano.begin(9600);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
  }
}

void loop() {
  //Check WiFi connection status
  int data;
  String test = "";

  if(nano.available()){
    test = nano.readStringUntil('\n'); //can use to check which test is being received
    data = nano.parseInt();
  }

  if(WiFi.status()== WL_CONNECTED){

    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

    // Ignore SSL certificate validation
    client->setInsecure();
    
    //create an HTTPClient instance
    HTTPClient https;
    
    // Your Domain name with URL path or IP address with path
    https.begin(*client, serverName);
    
    // Specify content-type header
    https.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Prepare your HTTP POST request data
    String httpRequestData =  "api_key" + apiKeyValue + "&test=" + test + "&value1=" + String(data) +  "";
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
 
    // Send HTTP POST request
    int httpResponseCode = https.POST(httpRequestData);

    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    https.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  //Send an HTTP POST request every 30 seconds
  delay(30000);  
}
