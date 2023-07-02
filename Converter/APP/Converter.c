#define  CONVERTER_PRG 1
#include "StdTypes.h"
#include "Converter_Interface.h"
#include "DIO_Interface.h"
#include "LCD_Interface.h"
#include "KeyPad_Interface.h"
#define F_CPU 8000000
#include "util/delay.h"
#include "Converter_Private.h"



static void (*conversionArr[3])(u8 keyPressed) = {convertFromDec,convertFromBin,convertFromHex};
static u8 hexValuesArr[2];
static u8 hexDigitIndex ;


static CURRENT_STATUS_type status;
static CONVERSION_Type convert_from;
static CONVERSION_Type convert_to;
static u8 result;
static u8 num ;
static Bool_t allowDisplay = TRUE;	//this flag is raised to allow display on LCD.
static Bool_t requestResult = FLASE; //this flag raised when result is requested after pressing '=' key
static Bool_t initialEntery = TRUE; //this flag raised when function is executed for first time
static Bool_t resultDisplayed = FLASE; //this flag raised when result is displayed on LCD
static u8 cursorPosition ; //determine the position of cursor on the LCD.


//initialize converter with required values
void converter_Init(void){
	cursorPosition = 2;
	status = SELECT_FROM;
	printEnteryMessage();
	_delay_ms(3000);
	LCD_Clear();
}


//converter runnable function
void converter_Runnable(void){
	u8 keyPressed;
	keyPressed = KEYPAD_GetKey();

	if (status == SELECT_FROM)
	{
		getFirstSelection(keyPressed);
	}
	else if(status == SELECT_TO){
		getSecondSelection(keyPressed);
	}else if (status == CONVERT)
	{
		conversionArr[convert_from](keyPressed);
		if (keyPressed == '=')
		{
			requestResult = TRUE;
		}
	}
	if (keyPressed == 'C')
	{
		resetConverter();
	}
	
}


//handle the conversion from DEC to BIN and HEX bases
static void convertFromDec(u8 keyPressed){
	u16 concatenationResult = 0;
	if (initialEntery)
	{
		convert_to == BIN? showLabel("D:","B:"):showLabel("D:","H:");
		initialEntery = FLASE;
	}
	
	if (keyPressed != 'T')
	{
		if (keyPressed >='0' && keyPressed <= '9')
		{
			if (resultDisplayed)
			{
				resetMode();
				convert_to == BIN? showLabel("D:","B:"):showLabel("D:","H:");
				resultDisplayed = FLASE;
			}
			concatenationResult  = (result * 10) + (keyPressed - '0')  ;
			if (concatenationResult <= 255)
			{
				result = concatenationResult;
				allowDisplay = TRUE;
			}
			else{
				playBuzz();
				allowDisplay = FLASE;
			}
			if (allowDisplay)
			{
				LCD_WriteChar(keyPressed);
			}
		}else if (keyPressed != 'C' && keyPressed != '=')
		{
			playBuzz();
		}
	}
	
	if (requestResult)
	{
		LCD_GoTo(1,2);
		convert_to == BIN ? LCD_WriteBinary(result):LCD_WriteHex(result);
		requestResult = FLASE;
		resultDisplayed = TRUE;
	}
}


//handle the conversion from BIN to DEC and HEX bases
static void convertFromBin(u8 keyPressed){
	u16 concatenationResult = 0;
	if (initialEntery)
	{
		convert_to == DEC? showLabel("B:","D:"):showLabel("B:","H:");
		initialEntery = FLASE;
	}
	
	if (keyPressed != 'T')
	{
		if (keyPressed >='0' && keyPressed <= '1')
		{
			if (resultDisplayed)
			{
				resetMode();
				convert_to == DEC? showLabel("B:","D:"):showLabel("B:","H:");
				resultDisplayed = FLASE;
			}
			concatenationResult  = ((result << 1) | (keyPressed - '0'))   ;
			if (concatenationResult <= 255)
			{
				result = concatenationResult;
				allowDisplay = TRUE;
			}
			else{
				playBuzz();
				allowDisplay = FLASE;
			}
			if (allowDisplay)
			{
				LCD_WriteChar(keyPressed);
			}
		}else if (keyPressed != 'C' && keyPressed != '=')
		{
			playBuzz();
		}
	}
	
	if (requestResult)
	{
		LCD_GoTo(1,2);
		convert_to == DEC ? LCD_WriteNumber(result):LCD_WriteHex(result);
		requestResult = FLASE;
		resultDisplayed = TRUE;
	}
}


//handle the conversion from HEX to DEC and BIN bases
static void convertFromHex(u8 keyPressed){
	
	if (initialEntery)
	{
		convert_to == DEC? showLabel("H:","D:"):showLabel("H:","B:");
		LCD_WriteNumber(0);
		initialEntery = FLASE;
	}
	
	if (keyPressed != 'T')
	{
		if (resultDisplayed)
		{
			resetMode();
			convert_to == DEC? showLabel("H:","D:"):showLabel("H:","B:");
			LCD_WriteNumber(0);
			resultDisplayed = FLASE;
		}
		
		if (keyPressed == '+')
		{
			num++;
			allowDisplay = TRUE;
		}else if (keyPressed == '-')
		{
			num --;
			allowDisplay = TRUE;
		}else if (keyPressed == 'N' && hexDigitIndex <1)
		{
			cursorPosition ++;
			hexDigitIndex++;
			num = 0;
		}
		else if (keyPressed != 'C' && keyPressed != '=')
		{
			playBuzz();
			allowDisplay = FLASE;
		}
		
		if (num > 15 || num < 0)
		{
			num = 0;
		}
		
		if (allowDisplay)
		{
			LCD_GoTo(0,cursorPosition);
			LCD_WriteHex(num);
		}
		hexValuesArr[hexDigitIndex] = num;
	}
	
	if (requestResult)
	{
		for (u8 i = 0 ; i < (cursorPosition - 1) ; i++)
		{
			result = (result *16) + hexValuesArr[i];
		}
		LCD_GoTo(1,2);
		convert_to == DEC ? LCD_WriteNumber(result):LCD_WriteBinary(result);
		requestResult = FLASE;
		resultDisplayed = TRUE;
	}
	
}




static void getFirstSelection(u8 keyPressed){
	
	printFirstSelectionMessage();
	
	if (keyPressed != 'T')
	{
		if (keyPressed >='1' && keyPressed <='3')
		{
			convert_from = keyPressed - '0' - 1;
			status = SELECT_TO;
			LCD_Clear();
		}
		else if(keyPressed != 'C'){
			handleWrongEntery();
		}
		
	}
}

static void getSecondSelection(u8 keyPressed){

	printSecondSelectionMessage(convert_from);
	if (keyPressed != 'T')
	{
		if (keyPressed >= '1' && keyPressed <= '2'){
			switch(convert_from){
				case DEC:
				convert_to = keyPressed == '1' ? BIN : HEX;
				break;
				case BIN:
				convert_to = keyPressed == '1' ? DEC :HEX;
				break;
				case HEX:
				convert_to = keyPressed == '1' ? DEC:BIN;
				break;
			}
			LCD_Clear();
			status = CONVERT;
		}
		else if(keyPressed != 'C') {
			handleWrongEntery();
		}
	}
}

static void handleWrongEntery(){
	LCD_Clear();
	LCD_WriteStringGoTo(0,2,"WRONG ENTERY");
	_delay_ms(1000);
	LCD_Clear();
}

static inline void printEnteryMessage(void){
	LCD_WriteStringGoTo(0,5,"Hello");
	LCD_WriteStringGoTo(1,1,"conv. by momen");
}


//print message to select the base to convert from .
static inline void printFirstSelectionMessage(void){
	LCD_WriteStringGoTo(0,2,"Convert from");
	LCD_WriteStringGoTo(1,1,"1:D  2:B  3:H");
}

//print message to select the base to convert to .
static void printSecondSelectionMessage(CONVERSION_Type convertFrom){
	LCD_WriteStringGoTo(0,3,"Convert to");
	switch(convertFrom){
		case DEC:
		LCD_WriteStringGoTo(1,4,"1:B  2:H");
		break;
		case BIN:
		LCD_WriteStringGoTo(1,4,"1:D  2:H");
		break;
		case HEX:
		LCD_WriteStringGoTo(1,4,"1:D  2:B");
		break;
	}

}



//this function trigger the buzzer
static void playBuzz(){
	DIO_WritePin(PINC5,HIGH);
	_delay_ms(40);
	DIO_WritePin(PINC5,LOW);
}

//function to show labels based on conversion from one base to another.
static void showLabel(char* from ,char* to){
	LCD_WriteStringGoTo(0,0,from);
	LCD_WriteStringGoTo(1,0,to);
	LCD_GoTo(0,2);
}

//reset the current conversion mode to enter new value
static void resetMode(){
	cursorPosition = 2;
	hexDigitIndex = 0;
	LCD_Clear();
	result = 0;
	num = 0;
}

//reset the entire converter in order to enter new conversion choice
static void resetConverter(){
	result = 0;
	num = 0;
	hexDigitIndex = 0;
	cursorPosition = 2;
	status = SELECT_FROM;
	convert_from = 0;
	convert_to = 0;
	allowDisplay = TRUE;
	requestResult = FLASE;
	initialEntery = TRUE;
	resultDisplayed = FLASE;
	LCD_Clear();
}