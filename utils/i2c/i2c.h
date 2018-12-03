#ifndef __I2C_H__
#define __12C_H__

#define F_SCL 384000UL
#define F_SCL_PRESCALAR 1 
#define BIT_RATE ((F_CPU / (F_SCL * F_SCL_PRESCALAR)) / 2) - 8

#define VDAC_ADDRESS 0b1100001
#define FANDAC_ADDRESS 0b1100000
#define ADC_ADDRESS 0b0110011
#define CONTROLLER_ADDRESS 0b1110000

#define BUFFER_SIZE 4

void i2cInit(void);
void i2cStart(void);
void i2cStop(void);
void i2cWrite(uint8_t u8data);
void i2cInitSlave(void);
uint8_t i2cReadACK(void);
uint8_t i2cReadNACK(void);
uint8_t i2cGetStatus(void);
void voltageDACInit(void);
void setVoltage(uint16_t data);
void fanDACInit(void);
void adcInit(void);
uint32_t readADC(void);
void setFanSpeed(uint16_t data);
void controllerInit(void);
void controllerWrite(uint32_t data);

#endif
