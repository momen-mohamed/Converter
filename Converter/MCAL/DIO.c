
#include "StdTypes.h"
#include "MemMap.h"
#include "Utils.h"

#include "DIO_Interface.h"
#include "DIO_Private.h"
#include "DIO_Cfg.h"

static const DIO_PortMap_type portMapArr[4] = {
	{.port  = PORTA,.ddr = DDRA,.pin = PINA},
	{.port  = PORTB,.ddr = DDRB,.pin = PINB},
	{.port  = PORTC,.ddr = DDRC,.pin = PINC},
	{.port  = PORTD,.ddr = DDRD,.pin = PIND},
};


void DIO_init(){
	DIO_Pin_type i;
	for (i = PINA0 ; i < TOTAL_PINS ; i++)
	{
		DIO_InitPin(i,DIO_PinsStatusArr[i]);
	}
}

static void DIO_InitPin(DIO_Pin_type pin,DIO_PinStatus_type status){
	unsigned char port = pin / 8;
	unsigned char pin_num = pin % 8;
	
	if (pin >= PINA0 && pin <= PIND7)
	{
		switch(status){
			case OUTPUT:
			SET_BIT(*(portMapArr[port].ddr),pin_num);
			CLEAR_BIT(*(portMapArr[port].port),pin_num);
			break;
			
			case INFREE:
			CLEAR_BIT(*(portMapArr[port].ddr),pin_num);
			CLEAR_BIT(*(portMapArr[port].port),pin_num);
			break;
			
			case INPULL:
			CLEAR_BIT(*(portMapArr[port].ddr),pin_num);
			SET_BIT(*(portMapArr[port].port),pin_num);
			break;
		}
	}

}


void DIO_WritePin(DIO_Pin_type pin,DIO_PinVoltage_type status){
	u8 port = pin / 8;
	u8 pin_num = pin % 8;
	
	if (pin >= PINA0 && pin <= PIND7)
	{
		switch(status){
			case HIGH:
			SET_BIT(*(portMapArr[port].port),pin_num);
			break;

			case LOW:
			CLEAR_BIT(*(portMapArr[port].port),pin_num);
			break;
		}
	}
}

DIO_PinVoltage_type DIO_ReadPin(DIO_Pin_type pin){
	u8 port = pin / 8;
	u8 pin_num = pin % 8;
	DIO_PinVoltage_type volt = LOW;
	
	if (pin >= PINA0 && pin <= PIND7)
	{
		volt = READ_BIT(*portMapArr[port].pin,pin_num);
	}
	return volt ;
}

void DIO_TogglePin(DIO_Pin_type pin){
	u8 port = pin / 8;
	u8 pin_num = pin % 8;
	if (pin >= PINA0 && pin <= PIND7)
	{
		TOGGLE_BIT(*(portMapArr[port].port),pin_num);
	}
}

void DIO_WritePort(DIO_Port_type port,u8 data){
	if (port >= PA && port <= PD)
	{
		*(portMapArr[port].port) = data;
	}
}

u8 DIO_ReadPort(DIO_Port_type port){
	u8 port_value = MIN_U8;
	
	if (port >= PA && port <= PD)
	{
		port_value = *(portMapArr[port].pin) ;
	}
	return port_value;
}




void DIO_InitPin2(DIO_Pin_type pin,DIO_PinStatus_type status){
	unsigned char port = pin / 8;
	unsigned char pin_num = pin % 8;
	
	switch(status){
		case OUTPUT:
		switch(port){
			case PA:
			SET_BIT(*DDRA,pin_num);
			CLEAR_BIT(*PORTA,0);
			break;
			case PB:
			SET_BIT(*DDRB,pin_num);
			CLEAR_BIT(*PORTB,0);
			break;
			case PC:
			SET_BIT(*DDRC,pin_num);
			CLEAR_BIT(*PORTC,0);
			break;
			case PD:
			SET_BIT(*DDRD,pin_num);
			CLEAR_BIT(*PORTD,0);
			break;
		}
		break;
		case INFREE:
		switch(port){
			case PA:
			CLEAR_BIT(*DDRA,pin_num);
			CLEAR_BIT(*PORTA,0);
			break;
			case PB:
			CLEAR_BIT(*DDRB,pin_num);
			CLEAR_BIT(*PORTB,0);
			break;
			case PC:
			CLEAR_BIT(*DDRC,pin_num);
			CLEAR_BIT(*PORTC,0);
			break;
			case PD:
			CLEAR_BIT(*DDRD,pin_num);
			CLEAR_BIT(*PORTD,0);
			break;
		}
		break;
		case INPULL:
		switch(port){
			case PA:
			CLEAR_BIT(*DDRA,pin_num);
			SET_BIT(*PORTA,0);
			break;
			case PB:
			CLEAR_BIT(*DDRB,pin_num);
			SET_BIT(*PORTB,0);
			break;
			case PC:
			CLEAR_BIT(*DDRC,pin_num);
			SET_BIT(*PORTC,0);
			break;
			case PD:
			CLEAR_BIT(*DDRD,pin_num);
			SET_BIT(*PORTD,0);
			break;
		}
		break;
	}
}










