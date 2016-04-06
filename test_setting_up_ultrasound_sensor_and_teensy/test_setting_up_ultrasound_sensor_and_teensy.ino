/* Analog Input Example, Teensyduino Tutorial #4
   http://www.pjrc.com/teensy/tutorial4.html

   After uploading this to your board, use Serial Monitor
   to view the message.  When Serial is selected from the
   Tools > USB Type menu, the correct serial port must be
   selected from the Tools > Serial Port AFTER Teensy is
   running this code.  Teensy only becomes a serial device
   while this code is running!  For non-Serial types,
   the Serial port is emulated, so no port needs to be
   selected.

   This example code is in the public domain.
*/

void setup()
{                
  Serial.begin(38400);
  pinMode(12, OUTPUT);
}

int val1;
int val2;
int val3;

void loop()                     
{
  digitalWrite(12, HIGH);
  delayMicroseconds(100);
  digitalWrite(12, LOW);
  val1 = analogRead(9);
  val2 = analogRead(8);
  val3 = analogRead(7);
  Serial.print("analog 0 is: ");
  Serial.print(val1);
  Serial.print("    ");
  Serial.print("analog 0 is: ");
  Serial.print(val2);
  Serial.print("    ");
  Serial.print("analog 0 is: ");
  Serial.println(val3);
  delay(30);
}

