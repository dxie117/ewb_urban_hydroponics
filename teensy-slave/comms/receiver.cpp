
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
    mInit = 1;
    #ifdef DEBUG
        UART_obj->println("RECEIVER INITIALIZED");
    #endif
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

uint8_t UART_Receiver::get_message(message &target)
{
    #ifdef DEBUG
        int got_cmd = 0;
        int got_device = 0;
    #endif
    char buf[MAX_BUF_LEN];
    buf[0] = '\0';
    size_t len = 0;
    do {
        len = UART_obj->readBytesUntil('\n', buf, MAX_BUF_LEN); // receive a message
    } while (buf[0] != '$');
    buf[len] = '\0'; // properly NULL-terminate string
    #ifdef DEBUG
        UART_obj->print("FULL MSG: ");
        UART_obj->println(buf);
    #endif
    char *substr = strtok(buf, " ");
    #ifndef DEBUG
        if (!match_cmd(substr + 1, target)) {
            return 0; // need to omit leading $
        }        
    #else
        got_cmd = match_cmd(substr + 1, target);
        UART_obj->print("CMD: ");
        UART_obj->println(substr);
        if (!got_cmd) {
            UART_obj->println("UNABLE TO MATCH COMMAND");
            delay(100);
            assert(0);
        }

    #endif
    substr = strtok(NULL, " ");
    
    #ifndef DEBUG
        if (!match_device(substr, target)) {
            return 0;
        };
    #else
        got_device = match_device(substr, target);
        UART_obj->print("DEVICE: ");
        UART_obj->println(substr);
        if (!got_device) {
            UART_obj->println("UNABLE TO MATCH DEVICE");
            delay(100);
            assert(0);
        }

    #endif
    substr = strtok(NULL, " ");
    if (substr != nullptr) { // how to handle \n, it's fine
        #ifdef DEBUG
            UART_obj->print("PARAMETER: ");
            UART_obj->println(substr);
        #endif
        target.param = atoi(substr);
    }
    return 1; 
}

uint8_t UART_Receiver::match_cmd(char *substr, message &target)
{
    for (int i = 0; i < NUM_CMDS; i++) {
        if (strcmp(substr, cmds[i]) == 0) {
            target.cmd = static_cast<cmd_type>(i);
            return 1;
        }
    }
    return 0;
}

uint8_t UART_Receiver::match_device(char *substr, message &target)
{
    for (int i = 0; i < NUM_DEVICES; i++) {
        if (strcmp(substr, devices[i]) == 0) {
            target.device = static_cast<device_type>(i);
            return 1;
        }
    }
    return 0;
}

