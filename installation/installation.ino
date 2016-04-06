#define BLUE   0x0000FF
#define BLACK  0x000000

#define LENGTH_OF_TAIL 4
#define LINKS 8
#define LEDS_PER_LINK 150

#include <OctoWS2811.h>

const int ledsPerStrip = LINKS * LEDS_PER_LINK;

DMAMEM int displayMemory[ledsPerStrip*6];
int drawingMemory[ledsPerStrip*6];

const int config = WS2811_GRB | WS2811_800kHz;

OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

int analogPin = A9;
int brightestPixels[LINKS];
float attenuations[LENGTH_OF_TAIL+1];

unsigned long previousMillis = 0;
unsigned long interval;

float minDelay;
float maxDelay;

int pixelsPerStrip;

void setup()
{                
//  Serial.begin(38400);

  minDelay = 30;
  maxDelay = 2000;

  leds.begin();
  leds.show();

  brightestPixels[0] = 0;

  for (int i=0; i<=LENGTH_OF_TAIL; i++)  {
    float attenuation = i * (256 / LENGTH_OF_TAIL);
    int scale = clamp(int(256 - attenuation), 0, 255);
    attenuations[i] = scale;
  }

  for (int link = 1; link < LINKS; link++) {
    brightestPixels[link] = brightestPixels[link-1] + LEDS_PER_LINK;
  }
}

void loop()  {
  int sensorValue = analogRead(analogPin);
  long delayTime = long(clamp((20000/clamp(sensorValue-50,1,1023)/3), minDelay, maxDelay));

//  Serial.print(sensorValue);
//  Serial.print("\t");
//  Serial.println(clamp(sensorValue-50,1,1023)/3);
//  
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= delayTime) {
    previousMillis = currentMillis;

    for (int link = 0; link < LINKS; link++) {
      for (int i=0; i<=LENGTH_OF_TAIL; i++)  {
        int target = (LEDS_PER_LINK + brightestPixels[link] - i) % ledsPerStrip;
        leds.setPixel(target, leds.color(attenuations[i], attenuations[i], attenuations[i]));
      }
   
      leds.show();
      
      brightestPixels[link] = (brightestPixels[link] + 1) % ledsPerStrip;
    }
  }
}

inline float clamp(float x, float a, float b){
    return x < a ? a : (x > b ? b : x);
}
