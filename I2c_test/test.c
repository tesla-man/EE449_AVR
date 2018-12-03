
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "test.h"
#include "i2c.h"


int main() {
    
    DDRB  |= 1<<PB5; //PWM SD
    PORTB &= ~(1<<PB5);

    i2cInit();
    _delay_ms(5);
    controllerInit();
	_delay_ms(10);
 
    while(1) {
	controllerWrite(0x009BA4C8);
	_delay_ms(1000);
    }
	
    return 0;
}
