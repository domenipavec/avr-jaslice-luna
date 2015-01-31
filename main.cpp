/* File: main.cpp
 * Contains base main function and usually all the other stuff that avr does...
 */
/* Copyright (c) 2012-2013 Domen Ipavec (domen.ipavec@z-v.si)
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

//#include <util/delay.h>

#include <avr/io.h>
#include <avr/interrupt.h>
//#include <avr/pgmspace.h>
//#include <avr/eeprom.h> 

#include <stdint.h>

#include "bitop.h"

static const uint8_t I2C_ADDRESS = 0x52;

int main() {
    // init
    /**
     * PA0 ... part 5
     * PA1 ... part 4
     * PA2 ... part 3
     * PA3 ... part 2
     * PB1 ... part 1
     * PB0 ... supply on
     */
    DDRA = 0b00001111;
    DDRB = 0b00000011;

    // init i2c
    TWSCRA = 0b00111000;
    TWSA = I2C_ADDRESS<<1;

    // enable interrupts
    sei();

    for (;;);
}

ISR(TWI_SLAVE_vect) {
    static uint8_t state;
    if (BITSET(TWSSRA, TWASIF)) {
        // received address/stop
        if (BITSET(TWSSRA, TWAS)) {
            // received address
            if (BITSET(TWSSRA, TWDIR)) {
                // read operation
                TWSCRB = 0b00000110;
            } else {
                // write operation
                state = 0;
                TWSCRB = 0b00000011;
            }
        } else {
            // received stop
            TWSCRB = 0b00000010;
        }
    } else if (BITSET(TWSSRA, TWDIF)) {
        // received data
        if (state == 0) {
            state++;
            uint8_t v = TWSD;
            switch (v) {
                case 1:
                    PORTA = 0;
                    PORTB = 0b011;
                    break;
                case 2:
                    PORTA = 0b01000;
                    PORTB = 0b011;
                    break;
                case 3:
                    PORTA = 0b01100;
                    PORTB = 0b011;
                    break;
                case 4:
                    PORTA = 0b01110;
                    PORTB = 0b011;
                    break;
                case 5:
                    PORTA = 0b01111;
                    PORTB = 0b011;
                    break;
                case 6:
                    PORTA = 0b01111;
                    PORTB = 0b001;
                    break;
                case 7:
                    PORTA = 0b00111;
                    PORTB = 0b001;
                    break;
                case 8:
                    PORTA = 0b00011;
                    PORTB = 0b001;
                    break;
                case 9:
                    PORTA = 0B00001;
                    PORTB = 0b001;
                    break;
                default:
                    PORTA = 0;
                    PORTB = 0;
                    break;
            }
        }
        
        TWSCRB = 0b00000011;
    } else {
        TWSCRB = 0b00000111;
    }
}