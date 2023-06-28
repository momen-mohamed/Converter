/*
 * LCD_Cfg.h
 *
 * Created: 6/12/2023 8:52:17 PM
 *  Author: mo2a
 */ 


#ifndef LCD_CFG_H_
#define LCD_CFG_H_

#define _4_BIT 0
#define _8_BIT 1


#define LCD_MODE _4_BIT

#define RS PINA1
#define EN PINA2

/****************** 4Bit Mode ***********************/
#define D4 PINA3
#define D5 PINA4
#define D6 PINA5
#define D7 PINA6

/****************** 8Bit Mode ***********************/

#define LCD_PORT PA






#endif /* LCD_CFG_H_ */