#define LENGTH_OF_TAIL 4
#define LINKS_PER_STRIP 6
#define LEDS_PER_LINK 150

#include <OctoWS2811.h>

const int ledsPerStrip = LINKS_PER_STRIP * LEDS_PER_LINK;
const int totalLinks = LINKS_PER_STRIP * 8;
const int totalLeds = totalLinks * LEDS_PER_LINK;

DMAMEM int displayMemory[ledsPerStrip*6];
int drawingMemory[ledsPerStrip*6];

const int config = WS2811_GRB | WS2811_800kHz;

OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

int analogPin = A9;
int brightestPixels[totalLinks];
float attenuations[LENGTH_OF_TAIL+1];

unsigned long previousMillis = 0;
unsigned long interval;

float minDelay;
float maxDelay;

int pixelsPerStrip;

void setup()  {                
  Serial.begin(38400);

  minDelay = 2;
  maxDelay = 2000;

  leds.begin();
  leds.show();

  brightestPixels[0] = 0;

  for (int i=0; i<=LENGTH_OF_TAIL; i++)  {
    float attenuation = i * (256 / LENGTH_OF_TAIL);
    int scale = clamp(int(256 - attenuation), 0, 255);
    attenuations[i] = scale;
  }

  for (int link = 1; link < totalLinks; link++) {
    brightestPixels[link] = brightestPixels[link-1] + LEDS_PER_LINK;
  }
}

void loop()  {
  int sensorValue = analogRead(analogPin);
  long delayTime = long(clamp((20000/clamp(sensorValue-50,1,1023)/3), minDelay, maxDelay));

  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= delayTime) {
    Serial.println(currentMillis - previousMillis);
    previousMillis = currentMillis;

    for (int link = 0; link < totalLinks; link++) {
      for (int i=0; i<=LENGTH_OF_TAIL; i++)  {
        int target = (LEDS_PER_LINK + brightestPixels[link] - i) % totalLeds;
        leds.setPixel(target, leds.color(attenuations[i], attenuations[i], attenuations[i]));
      }

      brightestPixels[link] = (brightestPixels[link] + 1) % totalLeds;
    }

    leds.show();
  }
}

inline float clamp(float x, float a, float b){
    return x < a ? a : (x > b ? b : x);
}
