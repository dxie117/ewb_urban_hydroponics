// All lights have frequency of 50 hz, and duty cycles vary from 0 to 100 in 10 percent increments, with no duty cycle of 10
// This code runs the duty cycles for 14 hours and then turns off for 10 hours, to simulate day and night
 
 /* Welcome to DmxSimple. This library allows you to control DMX stage and
** architectural lighting and visual effects easily from Arduino. DmxSimple
** is compatible with the Tinker.it! DMX shield and all known DIY Arduino
** DMX control circuits.
**
** DmxSimple is available from: http://code.google.com/p/tinkerit/
** Help and support: http://groups.google.com/group/dmxsimple       */

/* To use DmxSimple, you will need the following line. Arduino will
** auto-insert it if you select Sketch > Import Library > DmxSimple. */

#include <DmxSimple.h>
#include <util/delay.h>

int stripPins[] = {1,2,3,4,5,6,7,9,8,10};  //the lights are not wired in the right order
const unsigned long fourteen_hours = 1000UL*14*60*60;
const unsigned long ten_hours = 1000UL*10*60*60;
unsigned long elapsed = 0;

void setup() {
  /* The most common pin for DMX output is pin 3, which DmxSimple
  ** uses by default. If you need to change that, do it here. */
  DmxSimple.usePin(3);

  /* DMX devices typically need to receive a complete set of channels
  ** even if you only need to adjust the first channel. You can
  ** easily change the number of channels sent here. If you don't
  ** do this, DmxSimple will set the maximum channel number to the
  ** highest channel you DmxSimple.write() to. */
  DmxSimple.maxChannel(10);
  for(int x = 0; x <= 9; x++){
    DmxSimple.write(stripPins[x], 255); // turn on all other lights, initial state
}
  DmxSimple.write(stripPins[0], 0);  // duty cycle 0 light, always off
}

void loop() {
  unsigned long current_millis = millis();
  
  if(current_millis<elapsed){  // check if the arduino clock rolled over, may cause days to be of incorrect length on the order of once every 50 days
    elapsed = 0;
  }

  if(current_millis-elapsed <= fourteen_hours){ // run duty cycle during "day"
    DmxSimple.write(1,0);  // duty cycle 0 light, make sure its always off
    delay(1); // extra delay to skip duty cycle of 10
    for(int x = 1; x <= 9; x++){
      delay(1); // difference in duty cycle of 10% between one light and the next
      DmxSimple.write(stripPins[x], 0); // turn off light after delay for duty cycle
    }
    for(int x = 9; x >= 1; x--){
      DmxSimple.write(stripPins[x], 255); // turn lights back on in backward order
      delay(1); // difference in duty cycle of 10% between one light and the next
    }
    delay(1); // extra delay to skip duty cycle of 10
  }
  
  else{
    if(current_millis-elapsed <= fourteen_hours+ten_hours){ // all lights off during "night"
      for(int x = 1; x <= 10; x++){
        DmxSimple.write(stripPins[x], 0); // make sure all lights off
      }
    }
    else{
      elapsed = millis();
    }
  }
}
