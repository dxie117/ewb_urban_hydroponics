#include <Arduino.h>
#include <HardwareSerial.h>

class TEMP_Sensor
{
    public:
        TEMP_Sensor(uint8_t port, uint32_t baud);
        float get_temp();
        void set_single_read();
        void setup();

    private:
        char *sensordata = "";
        bool sensor_data_complete = false;
        uint32_t temp_port;
        uint32_t temp_baud;

        uint8_t rx;
        uint8_t tx;

        int read_temp(float *temperature);
};