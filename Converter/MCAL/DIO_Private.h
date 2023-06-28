/*
* DIO_Private.h
*
* Created: 6/9/2023 10:52:17 PM
*  Author: mo2a
*/


#ifndef DIO_PRIVATE_H_
#define DIO_PRIVATE_H_


typedef struct{
	volatile unsigned char* port;
	volatile unsigned char* ddr;
	volatile unsigned char* pin;
	
}DIO_PortMap_type;

static void DIO_InitPin(DIO_Pin_type pin,DIO_PinStatus_type mode);
extern const DIO_PinStatus_type DIO_PinsStatusArr[TOTAL_PINS];




#endif /* DIO_PRIVATE_H_ */