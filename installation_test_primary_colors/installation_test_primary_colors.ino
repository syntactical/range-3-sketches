#include <OctoWS2811.h>

const int red = 0x110000;
const int yellow = 0xddddff;
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

  leds.begin();
  leds.show();
}

void loop()  {
  float sensorValue = 50 * (sin(millis() / 2000.0f) + 1);
//  float sensorValue = analogRead(A9)/10.0f;
  
  for (int i=0; i<=totalLeds; i++)  {
    int position = i / (totalLeds / (sensorValue)) - 50;
    leds.setPixel(transpose(i), colorcycle[(position % 3)]);
  }

  leds.show();

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
