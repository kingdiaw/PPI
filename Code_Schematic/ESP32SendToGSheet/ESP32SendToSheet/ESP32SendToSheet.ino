#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"
#define DHTPIN 13
#define SOILPIN 34 //new code
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

int sensorValue = 0;//newcode

const char * ssid = "4GModem";  // type your Wifi name
const char * password = "12345678abc";  // Type your wifi password
String GOOGLE_SCRIPT_ID = "AKfycbxSNxfLt9ZE3MX8LzBtyP0Xotdq3vld4Poqsous8bJvM7cvPtK90kXbEyHxx0CrM1wnnw";  // Type your App Script id                  
const int sendInterval = 50;

WiFiClientSecure client;

void setup() {
 pinMode(2,OUTPUT);
  dht.begin();
  Serial.begin(115200);
  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(2,LOW);
    delay(500);
    Serial.print(".");
    digitalWrite(2,HIGH);
    delay(500);
  }
  Serial.println("OK");
  }
void loop() 
{
    //float h = dht.readHumidity();
    //float t = dht.readTemperature();
    float h = random(50,70);
    float t = random(25,40);
    sensorValue = analogRead(SOILPIN);
    sensorValue = map(sensorValue,0,4095,100,0);
    
    if (isnan(h) || isnan(t)) 
    {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }
     write_google_sheet( "value1="+String(h)+"&value2="+String(t)+"&value3="+String(sensorValue));
     delay(1000);
}
void write_google_sheet(String params) {
    HTTPClient http;
    String url="https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?"+params;
    Serial.println(url);
    Serial.println("Updating Temperature,Humidity,Soil Status");
    http.begin(url.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET();  
    Serial.print("HTTP Status Code: ");
    Serial.println(httpCode);
    String payload;
    if (httpCode > 0) {
        payload = http.getString();
        Serial.println("Payload: "+payload);     
    }
    http.end();
}
