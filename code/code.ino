#include <Adafruit_CircuitPlayground.h>
#include "pitches.h"

#define       NZEROS    2
#define       NPOLES    2
#define       GAIN      1.804169259

#define       HI        5
#define       LO        -3

double        xv[NZEROS+1];
double        yv[NPOLES+1];

bool          upFound = false;
unsigned char steps = 0;

double        amagPrev = 0;

void setLedNum(unsigned char val) {
  int index = 0;

  CircuitPlayground.clearPixels();

  do
  {
    if ((val >> index) & 0x1)
      CircuitPlayground.setPixelColor(index, 255,   0,   0);
  } while (++index < sizeof(val)*8);

}

float lowPass(float in){
    xv[0] = xv[1]; 
    xv[1] = xv[2];
    xv[2] = in / GAIN;
    
    yv[0] = yv[1]; 
    yv[1] = yv[2];
    yv[2] =  (xv[0] + xv[2]) + 2 * xv[1]
            + ( -0.8008026467 * yv[0]) + (  1.7786317778 * yv[1]);
    return yv[2];
}

void setup() {
  CircuitPlayground.begin();
  Serial.begin(9600);
  Serial.println("Start");
}

void loop() {
  double ax = CircuitPlayground.motionX();
  double ay = CircuitPlayground.motionY();
  double az = CircuitPlayground.motionZ();

  double amag = sqrt(az*az + ax*ax + ay*ay);
  amag = lowPass(amag);

  double dAmag = amag - amagPrev;
  amagPrev = amag;
  
  //Serial.println(dAmag);
  
  if (!upFound) {
      if (dAmag > HI) {
          upFound = true;
      }
  }
  else {
      if (dAmag < LO) {
          steps++;
          upFound = false;
          Serial.print("Step: ");
          Serial.println(steps);
          setLedNum(steps);
      }
  }
  
  delay(50);
}
