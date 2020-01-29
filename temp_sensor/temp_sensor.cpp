enum inputs {
  GET
}


enum commands {
  single,
  sleep,
  continuous_on,
  continuous_off
}


class temp_sensor
{
  uint32_t serial_port, baud_rate;
  string curr_mode;

  private:
    
  public:
    int reading;
    void get_message(char *msg);

    temp_sensor(uint32_t port, uint32_t rate) {
      serial_port = port;
      baud_rate = rate;
      curr_mode = single;
      send_command(single);
    }

    void execute() {
      switch (inputs) {
      case GET: {
        send_command(single);
        break;
      }
      
      default:
        break;
      }
    }

    void send_command(char *command) {
      return;
    }

};