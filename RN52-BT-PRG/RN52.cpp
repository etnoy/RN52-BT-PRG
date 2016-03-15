
#include "Arduino.h"
#include "RN52.h"
#include "SoftwareSerial.h"

/**
 * Atmel 328 pin definitions:
 */

const int BT_STATUS_PIN = 3;    // RN52 GPIO2 pin for reading current status of the module
const int BT_CMD_PIN = 4;       // RN52 GPIO9 pin for enabling command mode
const int BT_FACT_RST_PIN = A0; // RN52 factory reset pin GPIO4;
const int UART_TX_PIN = 5;      // UART Tx
const int UART_RX_PIN = 6;      // UART Rx
const int BT_WAKEUP_PIN = 7;    // Power enable pin on RN52
const int POWER_SWITCH_PIN = 8; // Pin used as a "switch" for testing of "wakeup" function

SoftwareSerial bt_serial =  SoftwareSerial(UART_RX_PIN, UART_TX_PIN);

void RN52Class::initialize_atmel_pins() {
    pinMode(BT_CMD_PIN, OUTPUT);
    pinMode(BT_FACT_RST_PIN,OUTPUT);
    pinMode(BT_WAKEUP_PIN,OUTPUT);
    pinMode(POWER_SWITCH_PIN,OUTPUT);
    digitalWrite(BT_CMD_PIN, HIGH);
    digitalWrite(BT_WAKEUP_PIN,LOW);
    digitalWrite(POWER_SWITCH_PIN,LOW);
}

void RN52Class::connect() {
    bt_serial.begin(BAUDRATE);
    digitalWrite(BT_CMD_PIN,LOW);
}

void RN52Class::disconnect() {
    digitalWrite(BT_CMD_PIN,HIGH);
}

void RN52Class::reset_factory_defaults() {
    digitalWrite(BT_FACT_RST_PIN,HIGH);
    delay(1000);
    digitalWrite(BT_FACT_RST_PIN,LOW);
    delay(1000);
    digitalWrite(BT_FACT_RST_PIN,HIGH);
    delay(1000);
    digitalWrite(BT_FACT_RST_PIN,LOW);
    delay(1000);
    digitalWrite(BT_FACT_RST_PIN,HIGH);
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

void RN52Class::wakeup() {
    Serial.println("Waking up RN52");
    digitalWrite(BT_WAKEUP_PIN,HIGH);
}

void RN52Class::check_power_enable() {
    int power_switch_state = 0;
    power_switch_state = digitalRead(POWER_SWITCH_PIN);
    if ((power_switch_state == 1) && (bt_up == false)) {
        wakeup();
        bt_up = true;
    }
}