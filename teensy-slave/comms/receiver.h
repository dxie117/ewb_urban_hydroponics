#ifndef RECEIVER_H_
#define RECEIVER_H_
#define MAX_BUF_LEN 25
#define NUM_CMDS 2
#define NUM_DEVICES 3

#include <Arduino.h>
#include <HardwareSerial.h>


#ifdef DEBUG
#include <assert.h>
#endif

/// let's base this off the NMEA way of handling messages



enum cmd_type{
    GET,
    PUT
};

enum device_type {
    TEMP,
    PH,
    LED
};

typedef struct {
    cmd_type cmd;
    device_type device;
    uint8_t param;
} message;

class UART_Receiver 
{
    public:
        uint8_t get_message(message &target);
        uint8_t get_port();
        uint8_t is_init();
        void flush();
        int begin();
        void end();
        uint8_t available();
        UART_Receiver(uint8_t port, uint16_t baud);

    private:
        uint8_t mUART_port;
        uint8_t mInit;
        HardwareSerial *UART_obj;
        uint16_t mBaud;
        uint8_t match_cmd(char *substr, message &target);
        uint8_t match_device(char *substr, message &target);
};






#endif