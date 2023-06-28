/*
 * Utils.c
 *
 * Created: 6/18/2023 2:13:44 AM
 *  Author: mo2a
 */ 

#include "StdTypes.h"

u8 strLen(c8* str){
	u8 i = 0;
	for (i =0;str[i];i++);
	return i;
}
