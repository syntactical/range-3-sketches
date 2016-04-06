#define BLUE   0x0000FF
#define BLACK  0x000000

#define LENGTH_OF_TAIL 4
#include <OctoWS2811.h>

const int ledsPerStrip = 300;

DMAMEM int displayMemory[ledsPerStrip*6];
int drawingMemory[ledsPerStrip*6];

const int config = WS2811_GRB | WS2811_800kHz;

OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

int analogPin = A9;
int brightestPixel;

unsigned long previousMillis = 0;
unsigned long interval;

float minDelay;
float maxDelay;

int pixelsPerStrip;

void setup()
{                
  Serial.begin(38400);

  minDelay = 30;
  maxDelay = 2000;

  leds.begin();
  leds.show();

  brightestPixel = 0;
}

void loop()  {
  int sensorValue = analogRead(analogPin);
  long delayTime = long(clamp((20000/clamp(sensorValue-50,1,1023)/3), minDelay, maxDelay));

  Serial.print(sensorValue);
  Serial.print("\t");
  Serial.println(clamp(sensorValue-50,1,1023)/3);
  
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= delayTime) {
    previousMillis = currentMillis;
      for (int i=0; i<=LENGTH_OF_TAIL; i++)  {
        float attenuation = i * (256 / LENGTH_OF_TAIL);
        int scale = clamp(int(256 - attenuation), 0, 255);
        int target = (ledsPerStrip + brightestPixel - i) % ledsPerStrip;
        leds.setPixel(target, leds.color(scale, scale, scale));
      }
   
      leds.show();
      brightestPixel = (brightestPixel + 1) % ledsPerStrip;
  }
}

inline float clamp(float x, float a, float b){
    return x < a ? a : (x > b ? b : x);
}
