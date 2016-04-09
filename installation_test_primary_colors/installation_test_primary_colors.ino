#include <OctoWS2811.h>

const int red = 0xFF0000;
const int yellow = 0xFFFF00;
const int blue = 0x0000FF;

const int ledsPerStrip = 900;
const int totalLeds = 150;

int colorcycle[] = {red, yellow, blue};

DMAMEM int displayMemory[ledsPerStrip*6];
int drawingMemory[ledsPerStrip*6];

const int config = WS2811_GRB | WS2811_800kHz;
const int analogPin = A9;

OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

void setup()  {                
  Serial.begin(38400);

  leds.begin();
  leds.show();
}

void loop()  {
  float sensorValue = 50 * (sin(millis() / 2000.0f) + 1);
  
  for (int i=0; i<=totalLeds; i++)  {
    int position = i / (totalLeds / (sensorValue));
    leds.setPixel(i, colorcycle[(position % 3)]);
  }

  leds.show();

  Serial.println(sensorValue);
}

inline float clamp(float x, float a, float b){
    return x < a ? a : (x > b ? b : x);
}
