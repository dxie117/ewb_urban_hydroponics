#include "led.h"


output_channel::output_channel(uint8_t channel, uint8_t duty_cycle,
                                   uint16_t turn_on_time, 
                                   uint16_t turn_off_time)
{
  this->ch_num = channel;
  this->duty_cycle = duty_cycle;
  this->turn_on_time = turn_on_time;
  this->turn_off_time = turn_off_time;
  this->is_night_phase = true;
}

linked_list::linked_list()
{
  this->start = nullptr;
  this->end = nullptr;
}

LEDControl::LEDControl()
{
  this->ch_list = linked_list();
}

bool LEDControl::configure_channel(uint8_t channel, uint8_t duty_cycle,
                                   uint16_t turn_on_time, 
                                   uint16_t turn_off_time)
{
  if (channel > 20 || duty_cycle > 100 || turn_on_time > 2400 || 
      turn_off_time > 2400 || turn_off_time <= turn_on_time) return false;
  list_node *cur_node = new list_node();
  output_channel *cur_ch = new output_channel(channel, duty_cycle, 
                                              turn_on_time, turn_off_time);
  cur_node->next = nullptr;
  cur_node->channel = cur_ch;
  if (this->ch_list.start == nullptr) {
    this->ch_list.start = cur_node;
    this->ch_list.end = cur_node;
  }
  else {
    this->ch_list.end->next = cur_node;
    this->ch_list.end = cur_node;
  }
  return true;
}

void LEDControl::initialize()
{
  for (node_t cur_node = this->ch_list.start; cur_node != nullptr; 
       cur_node = cur_node->next)
  {
    pinMode(cur_node->channel->ch_num, OUTPUT);
  }
}

void LEDControl::tick(uint16_t cur_time)
{
  for (node_t cur_node = this->ch_list.start; cur_node != nullptr; 
       cur_node = cur_node->next)
  {
    output_channel *cur_ch = cur_node->channel;
    if (cur_time < cur_ch->turn_on_time || cur_time >= cur_ch->turn_off_time)
    {
      digitalWrite(cur_ch->ch_num, LOW);
      cur_ch->is_night_phase = true;
      continue;
    }
    else if (cur_ch->is_night_phase) {
      digitalWrite(cur_ch->ch_num, HIGH);
      cur_ch->usec_start = micros();
      cur_ch->usec_next_start = cur_ch->usec_start + USEC_PERIOD;
      cur_ch->is_night_phase = false;
    }
    else if (micros() - cur_ch->usec_start >= 
             cur_ch->usec_next_start - cur_ch->usec_start)
    {
      if (cur_ch->duty_cycle != 0) {
        digitalWrite(cur_ch->ch_num, HIGH);
      }
      cur_ch->usec_start = micros();
      cur_ch->usec_next_start = cur_ch->usec_start + USEC_PERIOD;
    }
    else if (micros() - cur_ch->usec_start >= 
             cur_ch->duty_cycle*(USEC_PERIOD/100)) 
    {
      if (cur_ch->duty_cycle != 100) {
        digitalWrite(cur_ch->ch_num, LOW);
      }
    }
  }
    
}
