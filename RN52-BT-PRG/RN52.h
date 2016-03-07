
#ifndef _RN52_
#define _RN52_
#define SERIAL_BUFFER_SIZE      512     //Buffer size for comms with RN52
#define BAUDRATE                9600    //RN52 is happier with 9600bps instead of default 115200 when controlled by ATMEGA-328

enum action {PlayPause,Previous,Next,Reboot};

//----------------------------------------------------------------------------
// CLASS
//----------------------------------------------------------------------------

class RN52Class
{
    int serial_index;
    bool end_of_line_received;
public:
    char in_buffer[SERIAL_BUFFER_SIZE];
    void connect();
    void disconnect();
    bool read();
    void write(const char * in_message);
    void initialize_atmel_pins();
    RN52Class() {
        serial_index = 0;
        end_of_line_received = false;
    }
};

//----------------------------------------------------------------------------
// VARIABLES
//----------------------------------------------------------------------------

extern RN52Class RN52;

#endif /* defined(__SAAB_Sandbox__RN52__) */
