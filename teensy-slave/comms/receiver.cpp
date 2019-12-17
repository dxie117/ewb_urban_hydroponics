
#include "receiver.h"

const char* cmds[] = {"GET", "PUT"};
const char* devices[] = {"TEMP", "PH", "LED"};

UART_Receiver::UART_Receiver(uint8_t port, uint16_t baud)
{
    mBaud = baud;
    mUART_port = port;
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

uint8_t UART_Receiver::get_port()
{
    return mUART_port;
}

uint8_t UART_Receiver::is_init()
{
    return mInit;
}

int UART_Receiver::begin()
{
    if (UART_obj == nullptr) {
        return -1;
    }
    UART_obj->begin(mBaud);
    return 0;
}

void UART_Receiver::flush()
{
    UART_obj->flush();
}

void UART_Receiver::end()
{
    UART_obj->end();
}

uint8_t UART_Receiver::available() {
    return UART_obj->available();
}

void UART_Receiver::get_message(message &target)
{
    #ifdef DEBUG
        int got_cmd = 0;
        int got_device = 0;
    #endif
    char buf[MAX_BUF_LEN];
    size_t len = 0;
    do {
        len = UART_obj->readBytesUntil('\n', buf, MAX_BUF_LEN); // receive a message
    } while (buf[0] != '$');
    buf[len - 1] = '\0'; // properly NULL-terminate string
    char *substr = strtok(buf, " ");
    #ifndef DEBUG
        match_cmd(substr, target);
    #else
        got_cmd = !match_cmd(substr, target);
        if (!got_cmd) {
            assert(0);
        }
    #endif
    substr = strtok(NULL, " ");
    
    #ifndef DEBUG
        match_device(substr, target);
    #else
        got_device = !match_device(substr, target);
        if (!got_device) {
            assert(0);
        }
    #endif
    substr = strtok(NULL, " ");
    if (substr != nullptr) { // how to handle \n, it's fine
        target.param = atoi(substr);
    }
    return; 
}

uint8_t UART_Receiver::match_cmd(char *substr, message &target)
{
    for (int i = 0; i < NUM_CMDS; i++) {
        if (strcmp(substr, cmds[i]) == 0) {
            target.cmd = static_cast<cmd_type>(i);
            return 0;
        }
    }
    return 1;
}

uint8_t UART_Receiver::match_device(char *substr, message &target)
{
    for (int i = 0; i < NUM_DEVICES; i++) {
        if (strcmp(substr, devices[i]) == 0) {
            target.device = static_cast<device_type>(i);
            return 0;
        }
    }
    return 1;
}

