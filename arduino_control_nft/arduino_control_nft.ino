#include <Time.h>
#include <TimeLib.h>
#include "led.h"

LEDControl controller;
uint16_t prev_time;
uint16_t default_time;

void setup() {
  Serial.begin(9600);
  controller = LEDControl();
  controller.configure_channel(3, 100, 0002, 0003);
  controller.configure_channel(9, 100, 0001, 0005);
  controller.initialize();
  setTime(0, 1, 0, 1, 1, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  uint16_t cur_time = hour()* 100 + minute();
  controller.tick(cur_time);
}
