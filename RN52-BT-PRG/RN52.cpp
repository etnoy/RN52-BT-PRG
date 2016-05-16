
#include "Arduino.h"
#include "RN52.h"
#include "SoftwareSerial.h"

/**
 * Atmel 328 pin definitions:
 */

const int BT_STATUS_PIN = 3;    // RN52 GPIO2 pin for reading current status of the module
const int BT_CMD_PIN = 4;       // RN52 GPIO9 pin for enabling command mode
const int BT_FACT_RST_PIN = A0; // RN52 factory reset pin GPIO4;
const int BT_PWREN_PIN = 9;     // RN52 Power enable pin
const int UART_TX_PIN = 5;      // UART Tx
const int UART_RX_PIN = 6;      // UART Rx

SoftwareSerial bt_serial =  SoftwareSerial(UART_RX_PIN, UART_TX_PIN);

void RN52Class::initialize_atmel_pins() {
    pinMode(BT_CMD_PIN,OUTPUT);
    pinMode(BT_FACT_RST_PIN,INPUT); // Some REALLY crazy stuff is going on if this pin is set as output and pulled low. Leave it alone! Trust me...
    pinMode(BT_PWREN_PIN,OUTPUT);
    digitalWrite(BT_PWREN_PIN,HIGH);
    digitalWrite(BT_CMD_PIN,HIGH);
}

void RN52Class::wakeup() {
    int pin_status;
    digitalWrite(BT_PWREN_PIN,HIGH);
    pin_status = digitalRead(BT_PWREN_PIN);
    Serial.print("DEBUG: Current PWREN pin status: ");
    Serial.println(pin_status);
    delay(2000);
    digitalWrite(BT_PWREN_PIN,LOW);
    pin_status = digitalRead(BT_PWREN_PIN);
    Serial.print("DEBUG: Current PWREN pin status: ");
    Serial.println(pin_status);
}

void RN52Class::connect() {
//    wakeup();
    bt_serial.begin(BAUDRATE);
    digitalWrite(BT_CMD_PIN,LOW);
}

void RN52Class::disconnect() {
    digitalWrite(BT_CMD_PIN,HIGH);
}

void RN52Class::reset_factory_defaults() {
    Serial.println("Resetting RN52 to factory defaults");
    digitalWrite(BT_FACT_RST_PIN,HIGH);
    delay(1000);
    digitalWrite(BT_FACT_RST_PIN,LOW);
    delay(1000);
    digitalWrite(BT_FACT_RST_PIN,HIGH);
    delay(1000);
    digitalWrite(BT_FACT_RST_PIN,LOW);
    delay(1000);
    digitalWrite(BT_FACT_RST_PIN,HIGH);
    write("R,1\n");
}

void RN52Class::write(const char * in_message) {
    bt_serial.print(in_message);
}

bool RN52Class::read() {
    while (bt_serial.available() > 0) {
        char in_char = bt_serial.read();
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
