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
  private:
    
  public:
    int reading;
    void get_message(char *msg);

    temp_sensor(uint32_t port, uint32_t rate);


    void execute();

    void send_command(char *command);

}