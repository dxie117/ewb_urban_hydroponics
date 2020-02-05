
#include "temp_sensor.h"


TEMP_Sensor::TEMP_Sensor(uint8_t port, uint32_t baud) {
    temp_port = port;
    temp_baud = baud;

    switch (port) {
        case 0:
            UART_obj = &Serial;
            break;
        #ifdef HAVE_HWSERIAL1
        case 1:
            UART_obj = &Serial1;
            break;
        #endif
        #ifdef HAVE_HWSERIAL2
        case 2:
            UART_obj = &Serial2;
            break;
        #endif
        #ifdef HAVE_HWSERIAL3
        case 3:
            UART_obj = &Serial3;
            break;
        #endif
        default:
            UART_obj = nullptr;
            break;
    }
}

void TEMP_Sensor::setup() {
    UART_obj.begin(9600);
    //reserve bytes for receiving data
    sensordata.reserve(30);
}

void TEMP_Sensor::set_single_read() {
    UART_obj.print("C,0\r");
}

int TEMP_Sensor::read_temp(float *temp) {
    UART_obj.print("R\r");
    
    //if sensor has sent data
    if (UART_obj.available() > 0) {
        char inchar = (char)UART_obj.read();
        sensordata += inchar;
        if (inchar == '\r') {
            sensor_data_complete = true;
        }
    }

    if (sensor_data_complete == true) {
        if (isdigit(sensordata[0])) {
            *temp = sensordata.toFloat();
        }
        sensordata = "";
        sensor_data_complete = false;
        return 1;
    }
    return -1;
}


float TEMP_Sensor::get_temp() {
    float *temp = malloc(sizeof(float));
    if (temp = NULL) {
        return -1;
    }
    if (read_temp(*temp) == 1) {
        return *temp;
    }
    return -1;
}