/*
* LCD_Interface.h
*
* Created: 6/12/2023 8:50:07 PM
*  Author: mo2a
*/


#ifndef LCD_INTERFACE_H_
#define LCD_INTERFACE_H_



/**line 0:1	cell:0:15**/
void LCD_GoTo(u8 line,u8 cell);
void LCD_Init(void);
void LCD_WriteString(c8* str);
void LCD_WriteChar(u8 ch);
void LCD_WriteNumber(s32 num);
void LCD_WriteNumber_4D(u16 num);
void LCD_WriteHex(u8 num);
void LCD_WriteBinary(u8 num);
void LCD_Clear(void);
void LCD_CLearPosition(u8 line ,u8 cell , u8 NoOfCells);
void LCD_WriteStringGoTo(u8 line,u8 cell,c8* str);
void LCD_AddCustomPattern(u8 address,u8* pattern);
void LCD_GOHOME(void);

/********************* move on lcd **************************/

#endif /* LCD_INTERFACE_H_ */