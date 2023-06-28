/*
* LCD.c
*
* Created: 6/12/2023 8:50:52 PM
*  Author: momen mohamed
*/

#include "StdTypes.h"
#include "Utils.h"
#include "DIO_Interface.h"
#include "LCD_Interface.h"
#include "LCD_Cfg.h"
#include "LCD_Private.h"
#define F_CPU 8000000
#include "util/delay.h"

static u8 current_linePos;
static u8 current_cellPos;

//static void IncrementCellPosition(){
//current_cellPos++;
//if (current_cellPos > 15 && current_linePos == 0)
//{
//LCD_GoTo(1,0);
//current_cellPos = 0;
//current_linePos = 1;
//
//}else if (current_cellPos > 15 && current_linePos == 1){
//LCD_GoTo(0,0);
//current_cellPos = 0;
//current_linePos = 0;
//}
//}

#if LCD_MODE == _8_BIT
static void WriteIns(u8 ins){
	DIO_WritePin(RS,LOW);
	DIO_WritePort(PA,ins); // 8 bit 16
	DIO_WritePin(EN,HIGH);
	_delay_us(1);
	DIO_WritePin(EN,LOW);
	_delay_ms(1);
}

static void WriteData(u8 data){
	DIO_WritePin(RS,HIGH);
	DIO_WritePort(PA,data); // 8 bit data, 2lines ( 1/16 )
	DIO_WritePin(EN,HIGH);
	_delay_ms(1);
	DIO_WritePin(EN,LOW);
	_delay_ms(1);
}

void LCD_Init(void){
	_delay_ms(50);
	WriteIns(0x38);
	WriteIns(0x0c);
	WriteIns(0x01);
	_delay_ms(1);
	WriteIns(0x06);
}
#else

static void WriteIns(u8 ins){
	DIO_WritePin(RS,LOW);
	DIO_WritePin(D7,READ_BIT(ins,7));
	DIO_WritePin(D6,READ_BIT(ins,6));
	DIO_WritePin(D5,READ_BIT(ins,5));
	DIO_WritePin(D4,READ_BIT(ins,4));

	DIO_WritePin(EN,HIGH);
	_delay_ms(1);
	DIO_WritePin(EN,LOW);
	
	DIO_WritePin(D7,READ_BIT(ins,3));
	DIO_WritePin(D6,READ_BIT(ins,2));
	DIO_WritePin(D5,READ_BIT(ins,1));
	DIO_WritePin(D4,READ_BIT(ins,0));

	DIO_WritePin(EN,HIGH);
	_delay_ms(1);
	DIO_WritePin(EN,LOW);
	_delay_ms(2);
}

static void WriteData(u8 data){
	DIO_WritePin(RS,HIGH);
	DIO_WritePin(D7,READ_BIT(data,7));
	DIO_WritePin(D6,READ_BIT(data,6));
	DIO_WritePin(D5,READ_BIT(data,5));
	DIO_WritePin(D4,READ_BIT(data,4));
	DIO_WritePin(EN,HIGH);
	_delay_ms(1);
	DIO_WritePin(EN,LOW);
	
	DIO_WritePin(D7,READ_BIT(data,3));
	DIO_WritePin(D6,READ_BIT(data,2));
	DIO_WritePin(D5,READ_BIT(data,1));
	DIO_WritePin(D4,READ_BIT(data,0));

	DIO_WritePin(EN,HIGH);
	_delay_us(1);
	DIO_WritePin(EN,LOW);
	_delay_us(1);
}

void LCD_Init(void){
	_delay_ms(50);
	WriteIns(0x02);
	WriteIns(0x28);
	WriteIns(0x0c);
	WriteIns(0x01);
	_delay_ms(1);
	WriteIns(0x06);
}
#endif




void LCD_WriteChar(u8 ch){
	WriteData(ch);
	//IncrementCellPosition();
}

void LCD_WriteString(c8* str){
	u8 i ;
	for (i = 0 ; str[i];i++)
	{
		LCD_WriteChar(str[i]);
	}
}

void LCD_WriteNumber(s32 num){
	u8 str[30];
	u8 rem,i = MIN_U8;
	Sign_type sign = POSITIVE;
	if (num == 0)
	{
		LCD_WriteChar('0');
		return;
	}
	
	if (num < 0)
	{
		num = -num;
		sign = NEGATIVE;
	}
	
	while(num){
		rem = num % (u8)10;
		num /= (u8)10;
		str[i] = rem+(u8)'0';
		i++;
	}
	if (sign)
	{
		str[i] = '-';
		i++;
	}
	for (;i >0;i--)
	{
		LCD_WriteChar(str[i-1]);
	}
}

void LCD_WriteNumber_4D(u16 num){
	LCD_WriteChar(((num%10000)/1000)+'0');
	LCD_WriteChar(((num%1000)/100)+'0');
	LCD_WriteChar(((num%100)/10)+'0');
	LCD_WriteChar(((num%10)/1)+'0');
}

void LCD_WriteBinary(u8 num){
	u8 i ,result,flag = 0;
	for (i = 8 ; i > 0;i--)
	{
		result = READ_BIT(num,i-1);
		if (result)
		{
			flag = 1;
		}
		if (flag)
		{
			LCD_WriteChar(result+(u8)'0');
		}
	}
}


//void LCD_WriteHex(u8 num){
//u8 digit,i,noOfDigits = 2;
//
//for (i = 0 ; i<(u8)noOfDigits; i++)
//{
//if (i < noOfDigits-1)
//{
//digit = num / 16;
//}
//else{
//digit = num;
//}
//if (digit != 0 || i == noOfDigits - 1)
//{
//if (digit > 9)
//{
//digit = digit - (u8) 10 +'A';
//LCD_WriteChar(digit);
//
//}
//else{
//LCD_WriteNumber(digit);
//}
//}
//num = num %16;
//}
//}


void LCD_WriteHex(u8 num){
	u8 digit,i,noOfDigits = 2;
	u8 mask = 0xF0;
	for (i = 0 ; i<(u8)noOfDigits; i++)
	{
		digit = (mask & num) >>(4*(noOfDigits - i-1));
		if (digit != 0 || i == noOfDigits - 1)
		{
			if (digit > 9)
			{
				digit = digit - (u8) 10 +'A';
				LCD_WriteChar(digit);
			}
			else{
				LCD_WriteNumber(digit);
			}
		}
		mask = mask >>4;
	}
}

void LCD_GoTo(u8 line,u8 cell){
	current_linePos = line;
	current_cellPos = cell;
	
	if (line == 0)
	{
		WriteIns(LINE0_ADD+cell);
	}
	else if(line == 1){
		WriteIns(LINE1_ADD+cell);
	}
}

void LCD_WriteStringGoTo(u8 line,u8 cell,c8* str)
{
	LCD_GoTo(line,cell);
	LCD_WriteString(str);
}

void LCD_Clear(){
	WriteIns(0x01);
	_delay_ms(1);
}

void LCD_CLearPosition(u8 line ,u8 cell , u8 NoOfCells){
	u8 i;
	LCD_GoTo(line,cell);
	for (i =0 ;i < NoOfCells;i++)
	{
		LCD_WriteChar(' ');
	}
}

void LCD_AddCustomPattern(u8 address,u8* pattern){
	u8 i;
	WriteIns(CGRAM_ADDRESS+address*8);
	for (i=0;i<8;i++)
	{
		WriteData(pattern[i]);
	}
}


//return to CGROM after writing custom character to CGRAM
void LCD_GOHOME(void){ 
	LCD_GoTo(0,0); 
}



