
#include <avr/io.h>
#include <stdlib.h>
#include "i2c.h"
#include "lcd.h"

void i2cInit(void)
{
    if(F_SCL_PRESCALAR == 4)
    {
        TWSR |= 1<<TWPS0;
        TWSR &= ~(1<<TWPS1);
    }
    else if(F_SCL_PRESCALAR == 16)
    {
        TWSR &= ~(1<<TWPS0);
        TWSR |= 1<<TWPS1;
    }
    else if(F_SCL_PRESCALAR == 64)
    {
        TWSR |= (1<<TWPS0) | (1<<TWPS1);
    }
    else
    {
       TWSR &= ~(1<<TWPS0);
       TWSR &= ~(1<<TWPS1);
    }

    TWBR = BIT_RATE;
    TWCR = (1<<TWEN);
}

void i2cInitSlave(void)
{
    TWAR = (CONTROLLER_ADDRESS << 1);
    TWCR = (1<<TWEN)|(1<<TWEA)|(1<<TWIE);
}

void i2cStart(void)
{
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while ((TWCR & (1<<TWINT)) == 0);
}

void i2cStop(void)
{
    TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

void i2cWrite(uint8_t u8data)
{
    TWDR = u8data;
    TWCR = (1<<TWINT)|(1<<TWEN);
    while ((TWCR & (1<<TWINT)) == 0);
}

uint8_t i2cReadACK(void)
{
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
    while ((TWCR & (1<<TWINT)) == 0);
    return TWDR;
}

uint8_t i2cReadNACK(void)
{
    TWCR = (1<<TWINT)|(1<<TWEN);
    while ((TWCR & (1<<TWINT)) == 0);
    return TWDR;
}

uint8_t i2cGetStatus(void)
{
    uint8_t status;
    status = TWSR & 0xF8;
    return status;
}

void voltageDACInit(void)
{
    i2cStart();
    if(i2cGetStatus() != 0x08)
    {
        PORTD |= 1<<PD2;
    }
    i2cWrite(VDAC_ADDRESS << 1);
    if(i2cGetStatus() != 0x18)
    {
        PORTD |= 1<<PD2;
    }
    i2cWrite(0x60);
    if(i2cGetStatus() != 0x28)
    {
        PORTD |= 1<<PD2;
    }
    i2cWrite(0x00);
    if(i2cGetStatus() != 0x28)
    {
        PORTD |= 1<<PD2;
    }
    i2cWrite(0x00);
    if(i2cGetStatus() != 0x28)
    {
        PORTD |= 1<<PD2;
    }
    i2cStop();
}

void adcInit(void)
{
    i2cStart();
    if(i2cGetStatus() != 0x08)
    {
        PORTD |= 1<<PD2;
    }
    i2cWrite(ADC_ADDRESS << 1);
    if(i2cGetStatus() != 0x18)
    {
        PORTD |= 1<<PD2;
    }
    i2cWrite(0b11110010);
    if(i2cGetStatus() != 0x28)
    {
        PORTD |= 1<<PD2;
    }
    i2cWrite(0b01100001);
    if(i2cGetStatus() != 0x28)
    {
        PORTD |= 1<<PD2;
    }
    i2cStop();
}

uint32_t readADC(void)
{
    uint8_t msb, lsb; 
    uint32_t result;

    i2cStart();
    if(i2cGetStatus() != 0x08)
    {
        PORTD |= 1<<PD2;
    }
    i2cWrite((ADC_ADDRESS << 1) | 1);
    if(i2cGetStatus() != 0x40)
    {
        PORTD |= 1<<PD2;
    }
    msb = i2cReadACK();
    if(i2cGetStatus() != 0x50)
    {
        PORTD |= 1<<PD2;
    }
    lsb = i2cReadACK();
    if(i2cGetStatus() != 0x50)
    {
        PORTD |= 1<<PD2;
    }
    i2cStop();
    msb = msb & 0b00001111;
    result = msb << 8;
    result = result | lsb;
    return result;
}

void setVoltage(uint16_t data)
{
    i2cStart();
    if(i2cGetStatus() != 0x08)
    {
        PORTD |= 1<<PD2;
    }
    i2cWrite(VDAC_ADDRESS << 1);
    if(i2cGetStatus() != 0x18)
    {
        PORTD |= 1<<PD2;
    }
    i2cWrite(data >> 8);
    if(i2cGetStatus() != 0x28)
    {
        PORTD |= 1<<PD2;
    }
    i2cWrite(data & 0xFF);
    if(i2cGetStatus() != 0x28)
    {
        PORTD |= 1<<PD2;
    }
    i2cStop();
}

void fanDACInit(void)
{
    i2cStart();
    if(i2cGetStatus() != 0x08)
    {
        PORTD |= 1<<PD2;
    }
    i2cWrite(FANDAC_ADDRESS << 1);
    if(i2cGetStatus() != 0x18)
    {
        PORTD |= 1<<PD2;
    }
    i2cWrite(0x60);
    if(i2cGetStatus() != 0x28)
    {
        PORTD |= 1<<PD2;
    }
    i2cWrite(0x00);
    if(i2cGetStatus() != 0x28)
    {
        PORTD |= 1<<PD2;
    }
    i2cWrite(0x00);
    if(i2cGetStatus() != 0x28)
    {
        PORTD |= 1<<PD2;
    }
    i2cStop();
}

void setFanSpeed(uint16_t data)
{
    i2cStart();
    if(i2cGetStatus() != 0x08)
    {
        PORTD |= 1<<PD2;
    }
    i2cWrite(FANDAC_ADDRESS << 1);
    if(i2cGetStatus() != 0x18)
    {
        PORTD |= 1<<PD2;
    }
    i2cWrite(data >> 8);
    if(i2cGetStatus() != 0x28)
    {
        PORTD |= 1<<PD2;
    }
    i2cWrite(data & 0xFF);
    if(i2cGetStatus() != 0x28)
    {
        PORTD |= 1<<PD2;
    }
    i2cStop();
}

void controllerInit(void)
{
    i2cStart();
    if(i2cGetStatus() != 0x08)
    {
        PORTB |= 1<<PB5;
    }
    i2cWrite(CONTROLLER_ADDRESS << 1);
    if(i2cGetStatus() != 0x18)
    {
        PORTB |= 1<<PB5;
    }

    i2cStop();
}

void controllerWrite(uint32_t data)
{
    i2cStart();
    if(i2cGetStatus() != 0x08)
    {
        PORTB |= 1<<PB5;
    }
    i2cWrite(CONTROLLER_ADDRESS << 1);
    if(i2cGetStatus() != 0x18)
    {
        PORTB |= 1<<PB5;
    }
    i2cWrite(data & 0xFF);
    if(i2cGetStatus() != 0x28)
    {
       PORTB |= 1<<PB5;
    }
    i2cWrite((data >> 8) & 0xFF);
    if(i2cGetStatus() != 0x28)
    {
       PORTB |= 1<<PB5;
    }
    i2cWrite((data >> 16) & 0xFF);
    if(i2cGetStatus() != 0x28)
    {
       PORTB |= 1<<PB5;
    }
    i2cWrite((data >> 24) & 0xFF);
    if(i2cGetStatus() != 0x28)
    {
       PORTB |= 1<<PB5;
    }
    i2cStop();
}
