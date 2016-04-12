#include <OctoWS2811.h>
#define AVERAGE_ARRAY_LENGTH 10

const int red = 0x115533;
const int yellow = 0x220000;
const int blue = 0x000000;

const int stripLength = 150;
const int ledsPerStrip = 900;
const int totalLeds = 3600;

int colorcycle[] = {red, yellow, blue};

float readings[AVERAGE_ARRAY_LENGTH];

DMAMEM int displayMemory[ledsPerStrip*6];
int drawingMemory[ledsPerStrip*6];

const int config = WS2811_GRB | WS2811_800kHz;
const int analogPin = A9;

float minDelay = 30;
float maxDelay = 2000;

unsigned long previousMillis = 0;

OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

void setup()  {                
  Serial.begin(38400);

  leds.begin();
  leds.show();
}

void loop()  {
  float sensorValue = analogRead(A9);

  float sineValue = 50 * (sin(millis() / (4*sensorValue)))+ 50;
  
  long delayTime = long(clamp((20000/clamp(sensorValue-50,1,1023)/3), minDelay, maxDelay));
  
  updateAveragesArray(sensorValue);

  float reading = getAverageReading() / 5.0f;

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= delayTime) {
    previousMillis = currentMillis;

    for (int i=totalLeds; i<=totalLeds*2; i++)  {
      int position = i / (totalLeds*2 / (sineValue));
      leds.setPixel(transpose(i-totalLeds), colorcycle[(position % 3)]);
    }
  }
  
  leds.show();

  Serial.println((int) reading);
//  Serial.print("\t");
//  Serial.println(sensorValue);
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

void updateAveragesArray(float newReading){
  for (int i = 0; i < AVERAGE_ARRAY_LENGTH - 1; i++) {
    readings[i] = readings[i+1];
  }

  readings[AVERAGE_ARRAY_LENGTH-1] = newReading;
}

float getAverageReading(){
  float sum = 0;

  for (int i = 0; i < AVERAGE_ARRAY_LENGTH; i++) {
    sum += readings[i];
  }

  return sum/AVERAGE_ARRAY_LENGTH;
}
