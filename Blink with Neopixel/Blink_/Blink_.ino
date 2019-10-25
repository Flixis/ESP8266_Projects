#include <Adafruit_NeoPixel.h>

#define NeoPIN D4
#define NUM_LEDS 60
int brightness = 10;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, NeoPIN, NEO_GRBW + NEO_KHZ800);



void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  strip.setBrightness(brightness);
  strip.begin();
  strip.show(); 
  delay(50);
  Serial.println("NeoPixel started");
}


int function(){
  for(int j=0;j<256;j++){
      strip.setBrightness(j);
      Serial.println("this is brlvl:" + j);
    }  
}

// the loop function runs over and over again forever
void loop() {
  for(int i=0; i< NUM_LEDS;){
      strip.setPixelColor(i, 255, 0, 0);
      strip.show();
      delay(200);
      Serial.println(i);
      if(i >= 59){
        for(int x=NUM_LEDS; x>0;){
            strip.setPixelColor(x, 0, 255, 0);
            strip.show();
            delay(200);
            x--;
          }      
      }
      i++;1
  }
}
