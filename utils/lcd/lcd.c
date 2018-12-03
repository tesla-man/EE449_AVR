/*
 * LCD_Firmware.c
 *
 * Created: 2/22/2015 12:17:40 PM
 *  Author: Contovasilis
 */ 

#include "lcd.h"

char firstColumnPositions[4] = {0, 0x40, 0x14, 0x54};	
unsigned char pattern1[8] = {0x7, 0xf, 0x1f, 0x1f, 0x1f, 0x1f, 0xf, 0x7};
unsigned char pattern2[8] = {0x1c, 0x1e, 0x1f, 0x1f, 0x1f, 0x1f, 0x1e, 0x1c};	
unsigned char pattern3[8] = {0x1, 0xa, 0x14, 0x10, 0xd, 0x2, 0x1, 0x0};
unsigned char pattern4[8] = {0x10, 0x0, 0x7, 0x8, 0x14, 0x3, 0x10, 0xc};
unsigned char pattern5[8] = {0xd, 0x12, 0x0, 0x0, 0x0, 0x1, 0x1a, 0x4};
unsigned char pattern6[8] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x4};
unsigned char pattern7[8] = {0x4, 0xa, 0xa, 0xa, 0x1f, 0x1f, 0x1f, 0x1f};
unsigned char pattern8[8] = {0x01, 0x03, 0x07, 0x0F, 0x07, 0x03, 0x01};
unsigned char pattern9[8] = {0x1F, 0x10, 0x17, 0x17, 0x11, 0x17, 0x17, 0x17};
unsigned char pattern10[8] = {0x1F, 0x11, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x11};
unsigned char pattern11[8] = {0x1F, 0x11, 0x16, 0x16, 0x11, 0x15, 0x16, 0x16};
unsigned char pattern12[8] = {0x1F, 0x11, 0x17, 0x17, 0x11, 0x17, 0x17, 0x11};

void Initialize_LCD(void)
{
	_delay_ms(100);
	ENA_DIR |= 1<<ENA_BIT;
	RW_DIR |= 1<<RW_BIT;
	RS_DIR |= 1<<RS_BIT;
	Send_A_Command(0x30); //wake up
	Send_A_Command(0x38); //function set: 8bit, 2line, 5x8
	clear_Screen();
	Send_A_Command(0x06); //entry mode
	Send_A_Command(0x0C); //display on, cursor off
	return_Home();
	create_char(0, pattern1);
	create_char(1, pattern2);
	create_char(2, pattern3);
	create_char(3, pattern4);
	create_char(4, pattern5);
	create_char(5, pattern6);
	create_char(6, pattern7);
	create_char(7, pattern8);
}

void addPatterns(void)
{
	create_char(0, pattern9);
	create_char(1, pattern10);
	create_char(2, pattern11);
	create_char(3, pattern12);
}
void Check_BF()
{
	DB7_DIR &= ~(1<<DB7_BIT);
	RW_P |= 1<<RW_BIT;
	RS_P &= ~(1<<RS_BIT);
	while (DB7_P & 0x80)
	{
		ENA_P |= 1<<ENA_BIT;
		_delay_ms(2);
		ENA_P &= ~(1<<ENA_BIT);
	}
	DB0_DIR |= 1<<DB0_BIT;
	DB1_DIR |= 1<<DB1_BIT;
	DB2_DIR |= 1<<DB2_BIT;
	DB3_DIR |= 1<<DB3_BIT;
	DB4_DIR |= 1<<DB4_BIT;
	DB5_DIR |= 1<<DB5_BIT;
	DB6_DIR |= 1<<DB6_BIT;
	DB7_DIR |= 1<<DB7_BIT;
}

void Send_A_Command(unsigned char command)
{
	Check_BF();
	if(command & 1<<0) DB0_P |= 1<<DB0_BIT;
	if(command & 1<<1) DB1_P |= 1<<DB1_BIT; 
	if(command & 1<<2) DB2_P |= 1<<DB2_BIT; 
	if(command & 1<<3) DB3_P |= 1<<DB3_BIT; 
	if(command & 1<<4) DB4_P |= 1<<DB4_BIT; 
	if(command & 1<<5) DB5_P |= 1<<DB5_BIT; 
	if(command & 1<<6) DB6_P |= 1<<DB6_BIT; 
	if(command & 1<<7) DB7_P |= 1<<DB7_BIT;  
	RW_P &= ~(1<<RW_BIT);
	RS_P &= ~(1<<RS_BIT);

	ENA_P |= 1<<ENA_BIT;
	_delay_ms(2);
	ENA_P &= ~(1<<ENA_BIT);

	DB0_P &= ~(1<<DB0_BIT);
	DB1_P &= ~(1<<DB1_BIT);
	DB2_P &= ~(1<<DB2_BIT);
	DB3_P &= ~(1<<DB3_BIT);
	DB4_P &= ~(1<<DB4_BIT);
	DB5_P &= ~(1<<DB5_BIT);
	DB6_P &= ~(1<<DB6_BIT);
	DB7_P &= ~(1<<DB7_BIT);
}

void clear_Screen()
{
	Check_BF();
	DB0_P |= 1<<DB0_BIT;
	RW_P &= ~(1<<RW_BIT);
	RS_P &= ~(1<<RS_BIT);
	ENA_P |= 1<<ENA_BIT;
	_delay_ms(2);
	ENA_P &= ~(1<<ENA_BIT);
	DB0_P &= ~(1<<DB0_BIT);
}

void return_Home()
{
	Check_BF();
	DB5_P |= 1<<DB5_BIT;
	RW_P |= 1<<RW_BIT;
	RS_P &= ~(1<<RS_BIT);
	ENA_P |= 1<<ENA_BIT;
	_delay_ms(2);
	ENA_P &= ~(1<<ENA_BIT);
	DB5_P &= ~(1<<DB5_BIT);
	
}

void Send_A_Character(unsigned char character)
{
	Check_BF();
	if(character & 1<<0) DB0_P |= 1<<DB0_BIT;
	if(character & 1<<1) DB1_P |= 1<<DB1_BIT; 
	if(character & 1<<2) DB2_P |= 1<<DB2_BIT; 
	if(character & 1<<3) DB3_P |= 1<<DB3_BIT; 
	if(character & 1<<4) DB4_P |= 1<<DB4_BIT; 
	if(character & 1<<5) DB5_P |= 1<<DB5_BIT; 
	if(character & 1<<6) DB6_P |= 1<<DB6_BIT; 
	if(character & 1<<7) DB7_P |= 1<<DB7_BIT; 
	RW_P &= ~(1<<RW_BIT);
	RS_P |= 1<<RS_BIT;

	ENA_P |= 1<<ENA_BIT;
	_delay_us(38);
	ENA_P &= ~(1<<ENA_BIT);

	DB0_P &= ~(1<<DB0_BIT);
	DB1_P &= ~(1<<DB1_BIT);
	DB2_P &= ~(1<<DB2_BIT);
	DB3_P &= ~(1<<DB3_BIT);
	DB4_P &= ~(1<<DB4_BIT);
	DB5_P &= ~(1<<DB5_BIT);
	DB6_P &= ~(1<<DB6_BIT);
	DB7_P &= ~(1<<DB7_BIT);
}

void Send_A_Character_With_Location(uint8_t x, uint8_t y, unsigned char character)
{
	Goto_Location(x, y);
	Send_A_Character(character);
}

void Send_A_String(char *StringofCharacters)
{
	while(*StringofCharacters > 0)
	{
		Send_A_Character(*StringofCharacters++);
	}
}

void Send_A_String_With_Location(uint8_t x, uint8_t y, char *StringOfCharacters)
{
	Goto_Location(x, y);
	Send_A_String(StringOfCharacters);
}

void Goto_Location(uint8_t x, uint8_t y)
{
	Send_A_Command(0x80 + firstColumnPositions[y-1] + (x-1));
}

void Send_An_Integer(uint8_t x, uint8_t y, uint32_t IntegerToDisplay, char NumberOfDigits)
{
	char StringToDisplay[NumberOfDigits];
	ltoa(IntegerToDisplay, StringToDisplay, 10);
	Send_A_String_With_Location(x, y, StringToDisplay);
	if(strlen(StringToDisplay) < NumberOfDigits)
	{
		Send_A_String(" ");
	}
}

void Send_An_Integer_With_Tail(uint8_t x, uint8_t y, int IntegerToDisplay, char NumberOfDigits, char *tail)
{
	char StringToDisplay[NumberOfDigits];
	itoa(IntegerToDisplay, StringToDisplay, 10);
	Send_A_String_With_Location(x, y, StringToDisplay); Send_A_String(tail);
	if(strlen(StringToDisplay) < NumberOfDigits)
	{
		Send_A_String(" ");
	}
}

void Send_A_Double(uint8_t x, uint8_t y, double DoubleToDisplay, char NumberOfDigits, int precision)
{
	char StringToDisplay[NumberOfDigits];
	dtostrf(DoubleToDisplay, NumberOfDigits, precision, StringToDisplay);
	Send_A_String_With_Location(x, y, StringToDisplay);
}

void Send_A_Double_With_Tail(uint8_t x, uint8_t y, double DoubleToDisplay, char NumberOfDigits, int precision, char *tail)
{
	char StringToDisplay[NumberOfDigits];
	dtostrf(DoubleToDisplay, NumberOfDigits, precision, StringToDisplay);
	Send_A_String_With_Location(x, y, StringToDisplay); Send_A_String(tail); Send_A_String(" ");
}
void create_char(unsigned char location, unsigned char *ptr)
{
	unsigned char i;
	Send_A_Command(0x40 | (location << 3));
	for(i=0;i<8;i++)
	{
		Send_A_Character(ptr[ i ]);
	}
}
