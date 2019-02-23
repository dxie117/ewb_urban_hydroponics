/*
 * David Xie / davidx
 * 
 * duty_cycle.h
 * 
 * This header file defines constants and library functions that 
 * simplify the operation and upkeep of modulating
 * duty cycles for the growth LED strips on the NFT hydroponic system.
 */
 
#ifndef led_h
#define led_h
#include <stdint.h>
#include <Arduino.h>

#define USEC_PERIOD 16000

class output_channel
{
  public:
    bool is_night_phase;
    uint8_t ch_num;
    uint8_t duty_cycle;
    uint16_t turn_on_time;
    uint16_t turn_off_time;
    unsigned long usec_start;
    unsigned long usec_next_start;
    output_channel(uint8_t channel, uint8_t duty_cycle, uint16_t turn_on_time, 
                   uint16_t turn_off_time);
};

class list_node
{
  public:
    list_node *next;
    output_channel *channel;
};

class linked_list
{
  public:
    list_node *start;
    list_node *end;
    linked_list();
};

typedef list_node* node_t;

class LEDControl
{
  public:
    bool configure_channel(uint8_t channel, uint8_t duty_cycle,
                           uint16_t turn_on_time, uint16_t turn_off_time);
    void initialize();
    void tick(uint16_t cur_time);
    LEDControl();
  private:
    linked_list ch_list;
};

#endif
