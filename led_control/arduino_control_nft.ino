#include <Time.h>
#include <TimeLib.h>


#include "led.h"

LEDControl controller;
uint16_t prev_time;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  controller = LEDControl();
  controller.configure_channel(3, 100, 0002, 0003);
  controller.configure_channel(9, 5, 0001, 0002);
  controller.initialize();
  prev_time = hour()*100 + minute() - 1;
}

void loop() {
  // put your main code here, to run repeatedly:
  uint16_t cur_time = hour() << 2 + minute();
  controller.tick(cur_time);
}
