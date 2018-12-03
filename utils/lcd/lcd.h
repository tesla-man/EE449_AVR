
#ifndef __LCD_FIRMWARE_H__
#define __LCD_FIRMWARE_H__

#include <util/delay.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>

//HARDWARE CONFIGURATION----------------------------------------
#define DB0_P PORTD
#define DB1_P PORTD
#define DB2_P PORTB
#define DB3_P PORTB
#define DB4_P PORTD
#define DB5_P PORTD
#define DB6_P PORTD
#define DB7_P PORTD

#define DB0_BIT 0
#define DB1_BIT 1
#define DB2_BIT 0
#define DB3_BIT 1
#define DB4_BIT 4
#define DB5_BIT 5
#define DB6_BIT 6
#define DB7_BIT 7

#define DB0_DIR DDRD
#define DB1_DIR DDRD
#define DB2_DIR DDRB
#define DB3_DIR DDRB
#define DB4_DIR DDRD
#define DB5_DIR DDRD
#define DB6_DIR DDRD
#define DB7_DIR DDRD

#define ENA_P PORTC
#define RW_P PORTC
#define RS_P PORTC

#define ENA_BIT 2
#define RW_BIT 1
#define RS_BIT 0

#define ENA_DIR DDRC
#define RW_DIR DDRC
#define RS_DIR DDRC
//--------------------------------------------------------------


void Initialize_LCD(void);
void Check_BF(void);
void Goto_Location(uint8_t x, uint8_t y);
void Send_A_Command(unsigned char command);
void clear_Screen(void);
void return_Home(void);
void Send_A_Character(unsigned char character);
void Send_A_String(char *StringofCharacters);
void Send_A_Character_With_Location(uint8_t x, uint8_t y, unsigned char character);
void Send_A_String_With_Location(uint8_t x, uint8_t y, char *StringOfCharacters);
void Send_An_Integer(uint8_t x, uint8_t y, uint32_t IntegerToDisplay, char NumberOfDigits);
void Send_An_Integer_With_Tail(uint8_t x, uint8_t y, int IntegerToDisplay, char NumberOfDigits, char *tail);
void Send_A_Double(uint8_t x, uint8_t y, double DoubleToDisplay, char NumberOfDigits, int precision);
void Send_A_Double_With_Tail(uint8_t x, uint8_t y, double DoubleToDisplay, char NumberOfDigits, int precision, char *tail);
void addPatterns(void);
void create_char(unsigned char location, unsigned char *ptr);

#endif

