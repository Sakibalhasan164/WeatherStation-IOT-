//libraries
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include "icons.h"

const int xpos=0;
const int ypos=5;
const int iconWidth=64;
const int iconHeight=64;
const int color=1;

//wifi credentials
const char* ssid = "";
const char* password = "";

//datas to display
float current_temp_c;

//screen
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);




//server url
String serverURL = "http://api.weatherapi.com/v1/current.json?key=apiKey&q=Dhaka";
//test server
//String serverURL= "http://192.168.1.2:3000/req";

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //connect to wifi
  WiFi.begin(ssid, password);
  Serial.println("connecting to wifi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("connected to wifi with IP :");
  Serial.println(WiFi.localIP());
   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
//draw the bitmap
//  testdrawbitmap();

  
}

void loop() {
  Serial.println("sending request");
  // Send an HTTP POST request depending on timerDelay
//  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;

//      String serverPath = serverURL + "example.json";

      // Your Domain name with URL path or IP address with path
      http.begin(client, serverURL.c_str());

      // Send HTTP GET request
      int httpResponseCode = http.GET();

      if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
        //deal with json
       DynamicJsonDocument doc(1536);

DeserializationError error = deserializeJson(doc, payload);

if (error) {
  Serial.print(F("deserializeJson() failed: "));
  Serial.println(error.f_str());
  return;
}

JsonObject location = doc["location"];
const char* location_name = location["name"]; // "Dhaka"
const char* location_region = location["region"]; // nullptr
const char* location_country = location["country"]; // "Bangladesh"
float location_lat = location["lat"]; // 23.72
float location_lon = location["lon"]; // 90.41
const char* location_tz_id = location["tz_id"]; // "Asia/Dhaka"
long location_localtime_epoch = location["localtime_epoch"]; // 1628494127
const char* location_localtime = location["localtime"]; // "2021-08-09 13:28"
//the current data values
JsonObject current = doc["current"];
float current_temp_c=current["temp_c"];
//weather condition
JsonObject current_condition = current["condition"];
const char* condition_F= current_condition["text"];
//const char* condition="Sunny";
//char condition=condition_F;
std::string condition = condition_F;

//int is_day = current["is_day"]; // 1

//checkCondition(condition);

Serial.println(location_name);
Serial.println(location_region);
Serial.println(current_temp_c);
  display.clearDisplay();
  //date
  display.setTextSize(0.5);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(10,0);             // Start at top-left corner
  display.println(location_localtime);
  //temp
  display.setTextSize(2.5);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(64,30);             // Start at top-left corner
  display.print(current_temp_c);
//  Serial.println(condition);

//  checkCondition(condition);
if(condition == "Sunny"){
    //call the function to draw the icon
    display.drawBitmap(xpos,ypos, sunny,iconWidth, iconHeight, color); 
    Serial.print("does it works");
 }
if(condition =="Clear"){
     display.drawBitmap(xpos,ypos, clearSky,iconWidth, iconHeight, color);}
//      display.display();
//cloud
if(condition == "Overcast" || condition == "Partly Cloud" || condition == "Cloudy" || condition == "Mist" || condition == "Fog" || condition== "Freezing fog" ){
     display.drawBitmap(xpos,ypos, cloud,iconWidth, iconHeight, color);
//      display.display();
}
//rain
 if(condition == "Patchy rain possible" || condition == "Patchy rain nearby" || condition == "Patchy freezing drizzle nearby" || condition == "Patchy light drizzle" || condition == "Light drizzle" || condition == "Freezing drizzle" || condition == "Heavy freezing drizzle" || condition == "Patchy light rain" || condition == "Light rain" || condition == "Moderate rain at times" || condition == "Heavy rain at times" || condition == "Heavy rain" || condition == "Light freezing rain" || condition =="Moderate or heavy freezing rain" || condition == "Light rain shower" || condition == "Moderate or heavy rain shower"){
  display.drawBitmap(xpos,ypos, rain,iconWidth, iconHeight, color);
//   display.display();
  Serial.println("what");
}   
//thunder
if(condition == "Patchy light rain in area with thunder" || condition == "Moderate or heavy rain in area with thunder" || condition == "Thundery outbreaks in nearby" ){
    display.drawBitmap(xpos,ypos, thunder,iconWidth, iconHeight, color);
//     display.display();
    } 
display.display(); 
     
  }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }

//requesting every 900000 miliSeconds or 15 min as the api updates the data every 15 min
delay(60000);
}
//display.drawBitmap(0,0,bitmap, 64, 64, 1);




////draw the bitmap
//void checkCondition(const char* condition){
//  Serial.print(condition);
//  //sunny and clear
//}
