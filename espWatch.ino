#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2 (U8G2_R0);

//SSID and Password to your ESP Access Point
const char* ssid = "SpeedGasm";
const char* password = "animaleo";

ESP8266WebServer server(80); //Server on port 80

void handleRoot() {
  String currentSpeed = server.arg("s") ;
  String measure = server.arg("m") ;
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_fub42_tf);
  u8g2.setCursor(0, 50);
  u8g2.print(currentSpeed);
  u8g2.setFont(u8g2_font_etl16thai_t);
  u8g2.setCursor(100, 10);
  u8g2.print(measure);
  u8g2.sendBuffer();
  server.send(202, "text/plain", ""); 
}

IPAddress    apIP(198,168,45,249);
void setup(void) {
  Wire.begin(5, 4);
  u8g2.begin();
  Serial.begin(9600);
  
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));   // subnet FF FF FF 00
  //WiFi.mode(WIFI_AP); //Only Access point
  WiFi.softAP(ssid, password); //Start HOTspot removing password will disable security

  IPAddress myIP = WiFi.softAPIP(); //Get IP address
  Serial.print("HotSpt IP:");
  Serial.println(myIP);

  server.on("/eo", handleRoot); //Which routine to handle at root location

  server.begin(); //Start server
  Serial.println("HTTP server started");
  
   u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_fub30_tf);
  u8g2.setCursor(0, 35);
  u8g2.print("SPEED ");
  u8g2.setCursor(0, 64);
  u8g2.print("GASM ");
  u8g2.setFont(u8g2_font_6x13_me);
  u8g2.setCursor(50, 35);
  u8g2.print("0,1");
  u8g2.sendBuffer();
  delay(5000);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x13_me);
  u8g2.setCursor(0, 15);
  u8g2.print("name: " + String(ssid));
  u8g2.setCursor(0, 30);
  u8g2.print("pssw: " + String(password));
  u8g2.setCursor(0, 45);
  u8g2.print(myIP.toString());
  u8g2.setCursor(0, 60);
  u8g2.print("/eo?s=<val>&m=<val>");
  u8g2.sendBuffer();
}

void loop(void) {
  server.handleClient(); //Handle client requests
}
