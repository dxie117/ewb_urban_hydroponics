
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "ph_grav.h"
#include "EEPROM.h"


Gravity_pH::Gravity_pH(uint8_t pin){
	this->pin = pin;
}

bool Gravity_pH::begin(){
	if(EEPROM.read(0) == magic_char){
		EEPROM.get(0,pH);
		return true;
    }
	return false;
}

float Gravity_pH::read_voltage() {
  float voltage_mV = 0;
  for (int i = 0; i < volt_arr_len; ++i) {
    voltage_mV += analogRead(this->pin) / 1024.0 * 5000.0;
  }
  voltage_mV /= volt_arr_len;
  return voltage_mV;
}

float Gravity_pH::read_ph(float voltage_mV) {
  if (voltage_mV > pH.mid_cal) { //high voltage = low ph
    return 7.0 - 3.0 / (this->pH.low_cal - this->pH.mid_cal) * (voltage_mV - this->pH.mid_cal);
  } else {
    return 7.0 - 3.0 / (this->pH.mid_cal - this->pH.high_cal) * (voltage_mV - this->pH.mid_cal);
  }
}

float Gravity_pH::read_ph() {
  return(read_ph(read_voltage()));
}

void Gravity_pH::cal_mid(float voltage_mV) {
  this->pH.mid_cal = voltage_mV;
  EEPROM.put(0,pH);
}
void Gravity_pH::cal_mid() {
  cal_mid(read_voltage());
}

void Gravity_pH::cal_low(float voltage_mV) {
  this->pH.low_cal = voltage_mV;
  EEPROM.put(0,pH);
}
void Gravity_pH::cal_low() {
  cal_low(read_voltage());
}

void Gravity_pH::cal_high(float voltage_mV) {
  this->pH.high_cal = voltage_mV;
  EEPROM.put(0,pH);
}

void Gravity_pH::cal_high() {
  cal_high(read_voltage());
}

void Gravity_pH::cal_clear() {
  this->pH.mid_cal = 1500;
  this->pH.low_cal = 2030;
  this->pH.high_cal = 975;
  EEPROM.put(0,pH);
}
