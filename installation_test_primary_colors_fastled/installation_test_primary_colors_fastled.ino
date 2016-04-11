#define USE_OCTOWS2811

#include <bitswap.h>
#include <chipsets.h>
#include <color.h>
#include <colorpalettes.h>
#include <colorutils.h>
#include <controller.h>
#include <dmx.h>
#include <FastLED.h>
#include <fastled_config.h>
#include <fastled_delay.h>
#include <fastled_progmem.h>
#include <fastpin.h>
#include <fastspi.h>
#include <fastspi_bitbang.h>
#include <fastspi_dma.h>
#include <fastspi_nop.h>
#include <fastspi_ref.h>
#include <fastspi_types.h>
#include <hsv2rgb.h>
#include <led_sysdefs.h>
#include <lib8tion.h>
#include <noise.h>
#include <pixeltypes.h>
#include <platforms.h>
#include <power_mgt.h>

#include <OctoWS2811.h>

#define NUM_LEDS_PER_STRIP 900
#define NUM_STRIPS 8
#define NUM_LEDS NUM_STRIPS * NUM_LEDS_PER_STRIP

CRGB leds[NUM_LEDS];

const int red = 0x115533;
const int yellow = 0x111122;
const int blue = 0x000000;

const int stripLength = 150;
const int ledsPerStrip = 900;
const int totalLeds = 3600;

int colorcycle[] = {red, yellow, blue};

DMAMEM int displayMemory[ledsPerStrip*6];
int drawingMemory[ledsPerStrip*6];

const int config = WS2811_GRB | WS2811_800kHz;
const int analogPin = A9;

OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

void setup()  {                
  Serial.begin(38400);

  LEDS.addLeds<OCTOWS2811>(leds, NUM_LEDS_PER_STRIP);
}

void loop()  {
  float sensorValue = 50 * (sin(millis() / 4000.0f) + 1);
//  float sensorValue = analogRead(A9)/10.0f;
  
  for (int i=0; i<=totalLeds; i++)  {
    int position = i / (totalLeds / (sensorValue)) - 50;
    leds.setPixel(transpose(i), colorcycle[(position % 3)]);
//      leds.setPixel(i,red);
  }

  LEDS.show();

  Serial.println(sensorValue);
}

inline float clamp(float x, float a, float b){
    return x < a ? a : (x > b ? b : x);
}

inline int transpose(int position){
  int transposedPosition;
  
  bool reversed = (position / stripLength) % 2 == 1;

  if (reversed) {
    int stripStartPosition = ((position / stripLength) * stripLength);
    transposedPosition = stripStartPosition + ((stripLength - 1) - (position - stripStartPosition));
  } else {
    transposedPosition = position;
  }

  return transposedPosition;
}
