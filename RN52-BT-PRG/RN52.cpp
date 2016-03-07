
#include "Arduino.h"
#include "RN52.h"
#include "SoftwareSerial.h"

const int uart_tx_pin = 5; // UART Tx
const int uart_rx_pin = 6; // UART Rx
//const int status_pin = 5;
const int program_pin = 4;
SoftwareSerial bt_serial =  SoftwareSerial(uart_rx_pin, uart_tx_pin);

void RN52Class::initialize_atmel_pins() {
    pinMode(program_pin, OUTPUT);
    digitalWrite(program_pin, HIGH);

}

void RN52Class::connect() {
    bt_serial.begin(BAUDRATE);
    digitalWrite(program_pin,LOW);
}

void RN52Class::disconnect() {
    digitalWrite(program_pin,HIGH);
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
