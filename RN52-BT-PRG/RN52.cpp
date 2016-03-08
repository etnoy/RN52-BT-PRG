
#include "Arduino.h"
#include "RN52.h"
#include "SoftwareSerial.h"

/**
 * Atmel 328 pin definitions:
 */

const int BT_STATUS_PIN = 3;    // RN52 GPIO2 pin for reading current status of the module
const int BT_CMD_PIN = 4;       // RN52 GPIO9 pin for enabling command mode
const int UART_TX_PIN = 5;      // UART Tx
const int UART_RX_PIN = 6;      // UART Rx

SoftwareSerial bt_serial =  SoftwareSerial(UART_RX_PIN, UART_TX_PIN);

void RN52Class::initialize_atmel_pins() {
    pinMode(BT_CMD_PIN, OUTPUT);
    pinMode(A0,OUTPUT);         //  RN52 factory reset pin; needs to be pulled down so it is not left floating
    digitalWrite(BT_CMD_PIN, HIGH);
    digitalWrite(A0,LOW);
}

void RN52Class::connect() {
    bt_serial.begin(BAUDRATE);
    digitalWrite(BT_CMD_PIN,LOW);
}

void RN52Class::disconnect() {
    digitalWrite(BT_CMD_PIN,HIGH);
}

void RN52Class::write(const char * in_message) {
    bt_serial.print(in_message);
}

bool RN52Class::read() {
    while (bt_serial.available() > 0) {
        char in_char = bt_serial.read();
//        Serial.print(int(in_char));
//        Serial.print(in_char);
        if (in_char == '\r') continue;
        if (in_char == '\n') {
            end_of_line_received = true;
            in_buffer[serial_index] = 0;
            serial_index = 0;
            return true;
        }
        if (serial_index < SERIAL_RX_BUFFER_SIZE - 1) {
            in_buffer[serial_index] = in_char;
            serial_index++;            
        }
    }
    return false;
}
