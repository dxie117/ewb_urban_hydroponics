#ifndef RECEIVER_H_
#define RECEIVER_H_
#define MAX_BUF_LEN 25
#define NUM_CMDS 2
#define NUM_DEVICES 3
#define UART_BUF_LEN 32

#include <Arduino.h>
#include <HardwareSerial.h>
#include "queues.h"

#define DEBUG
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
        void listen();
        UART_Receiver(uint8_t port, uint32_t baud);

    private:
        char rx_buf[UART_BUF_LEN];
        uint32_t rx_ind;
        uint8_t mUART_port;
        uint8_t mInit;
        HardwareSerial *UART_obj;
        uint32_t mBaud;
        Queue mQueue;
        uint8_t match_cmd(char *substr, message &target);
        uint8_t match_device(char *substr, message &target); // gonna not handle the case where buffer loops around...
};






#endif