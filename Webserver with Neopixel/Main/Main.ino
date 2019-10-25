/*
 * NeoPixel Webserver
 * Copyright (c) 2017, Agis Wichert
 * https://www.youtube.com/watch?v=6U4JssgWjr8
 * 
 * 
 * ESP Webserver
 * Copyright (c) 2015, Majenko Technologies
 * 
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * * Neither the name of Majenko Technologies nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */


// Neopixel
#include <Adafruit_NeoPixel.h>

// ESP8266
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// Webserver Config
const char *ssid = "SitecomA2354C";
const char *password = "VSX-924-K";
ESP8266WebServer server ( 80 );

// Neopixel Config
#define NeoPIN D4
#define NUM_LEDS 80
int brightness = 10;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, NeoPIN, NEO_GRBW + NEO_KHZ800);


const int led = 13;

void setup ( void ) {

  Serial.begin ( 115200 );

  // ##############
  // NeoPixel start
  Serial.println();
  strip.setBrightness(brightness);
  strip.begin();
  strip.show(); 
  delay(50);
  Serial.println("NeoPixel started");
  
  // #########
  // Webserver
  pinMode ( led, OUTPUT );
  digitalWrite ( led, 0 );
  
  WiFi.begin ( ssid, password );
  Serial.println ( "" );

  // Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  Serial.println ( "" );
  Serial.print ( "Connected to " );
  Serial.println ( ssid );
  Serial.print ( "IP address: " );
  Serial.println ( WiFi.localIP() );

  if ( MDNS.begin ( "esp8266" ) ) {
    Serial.println ( "MDNS responder started" );
  }

  // what to do with requests
  server.on ( "/", handleRoot );
  server.onNotFound ( handleNotFound );
  server.begin();

  
  
  Serial.println ( "HTTP server started" );
}

void loop ( void ) {
  // waiting fo a client
  server.handleClient();
}


void handleRoot() {
  Serial.println("Client connected");
  digitalWrite ( led, 1 );
  
  // data from the colorpicker (e.g. #FF00FF)
  String color = server.arg("c");
  Serial.println("Color: " + color);
  // setting the color to the strip 
  setNeoColor(color);

  // building a website
  char temp[5000];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;
  char clr [7];
  color.toCharArray(clr, 7);
  snprintf ( temp, 5000,

"<!DOCTYPE html>\n<html>\n\
  <head>\n\
    <title>Esp8266 NeoPixel Control</title>\n\
    <style>\
      body { background-color: #cccccc; font-family: Arial; Color: #008; }\
    </style>\n\
    <meta name=\"viewport\" content=\"width=device-width, height=device-height, initial-scale=1.0, user-scalable=0, minimum-scale=1.0, maximum-scale=1.0\" />\n\
  </head>\n\
  <body>\n\
    <h1>NeoPixel control</h1>\n\
    <p>Uptime: %02d:%02d:%02d</p>\n\
    \n\
    <form action=\"\" name=\"pick\" method=\"post\">\n\
    <input type=\"color\" name=\"c\" value=\"%02d\" onchange=\"document.forms['pick'].submit();\" />\n\
    &nbsp;<span onclick=\"document.forms['pick'].submit();\" style=\"font-size:16pt;\"> CHANGE </span>\n\
    </form>\n\
    \n\
  </body>\
</html>",

    hr, min % 60, sec % 60, clr
  );
  server.send ( 200, "text/html", temp );
  digitalWrite ( led, 0 );
}

void handleNotFound() {
  digitalWrite ( led, 1 );
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }

  server.send ( 404, "text/plain", message );
  digitalWrite ( led, 0 );
}



void setNeoColor(String value){
  Serial.print("Setting Neopixel...");
  // converting Hex to Int
  int number = (int) strtol( &value[1], NULL, 16);
  
  // splitting into three parts
  int r = number >> 16;
  int g = number >> 8 & 0xFF;
  int b = number & 0xFF;
  
  // DEBUG
  Serial.print("RGB: ");
  Serial.print(r, DEC);
  Serial.print(" ");
  Serial.print(g, DEC);
  Serial.print(" ");
  Serial.print(b, DEC);
  Serial.println(" ");
  
  // setting whole strip to the given color
  for(int i=0; i < NUM_LEDS; i++) {
    //strip.setPixelColor(i, strip.Color( g, r, b ) );
    strip.fill(strip.Color( g, r, b ,0));
  }
  
  // init
  strip.show();
  
  Serial.println("on.");
}
