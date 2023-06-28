/*
* UTILS.h
*
* Created: 6/9/2023 8:37:14 PM
*  Author: momen mohamed
*/


#ifndef UTILS_H_
#define UTILS_H_

#define READ_BIT(reg,bit)  (((reg)>>(bit))&1)
#define SET_BIT(reg,bit)  (reg = (reg) | (1<<(bit)))
#define TOGGLE_BIT(reg,bit)  (reg = (reg) ^ (1<<(bit)))
#define CLEAR_BIT(reg,bit)  (reg = (reg) & (~(1<<(bit))))

typedef enum {
	POSITIVE,
	NEGATIVE
}Sign_type;

u8 strLen(c8* str);

#endif /* UTILS_H_ */