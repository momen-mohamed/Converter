#include "StdTypes.h"
#include "Utils.h"
#include "DIO_Interface.h"
#include "LCD_Interface.h"
#include "KeyPad_Interface.h"
#include "Converter_Interface.h"


int main(void)
{
	DIO_init();
	LCD_Init();
	KEYPAD_Init();
	converter_Init();
    while (1) 
    {
		converter_Runnable();
    }
}

