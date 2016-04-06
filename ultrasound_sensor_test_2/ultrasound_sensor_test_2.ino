#define LED 2
#define BLUE   0x0000FF
#define BLACK  0x000000

#include <OctoWS2811.h>

const int ledsPerStrip = 300;

DMAMEM int displayMemory[ledsPerStrip*6];
int drawingMemory[ledsPerStrip*6];

const int config = WS2811_GRB | WS2811_800kHz;

OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

void setup()
{                
  Serial.begin(38400);
  pinMode(12, OUTPUT);
  pinMode(LED, OUTPUT);


  leds.begin();
  leds.show();
}

int val;

void loop()  {
  int pixelsInOneStrip = leds.numPixels() / 8;
  for (int i=0; i < pixelsInOneStrip; i++) {
    int previousPixel = (i - 1 + pixelsInOneStrip) % pixelsInOneStrip;
    leds.setPixel(previousPixel, BLACK);
    leds.setPixel(i, BLUE);
    leds.show();
    delay(analogRead(9));   
  }
}


