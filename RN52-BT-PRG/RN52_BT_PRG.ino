//
// RN52-BT-PRG
//
// Code for programming RN52 bluetooth module using ATMEGA328P-PU microcontroller
// Developed with [embedXcode](http://embedXcode.weebly.com)
//
// Author 		Karlis Veilands
//
// Date			3/7/16 3:33 PM
// Version		1.0
//
// Copyright	© Karlis Veilands, 2016
// Licence      Beerware
//

#include "Arduino.h"
#include "RN52.h"

RN52Class RN52;

void setup() {
    RN52.initialize_atmel_pins();
    Serial.begin(BAUDRATE);
    RN52.reset_factory_defaults();
    RN52.connect();
    Serial.println("RN52 programming mode");
}

int rowCount = 0;

void loop() {
    if (Serial.available() > 0) {
        char in_char[2];
        in_char[0] = Serial.read();
        in_char[1] = 0;
        RN52.write(in_char);
    }
    if (RN52.read()) {
        Serial.println(RN52.in_buffer);
    }
}
