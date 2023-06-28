/*
* StdTypes.h
*
* Created: 6/9/2023 8:18:19 PM
*  Author: mo2a
*/


#ifndef STDTYPES_H_
#define STDTYPES_H_

typedef unsigned char u8;
typedef signed   char s8;
typedef char c8;

typedef unsigned short u16;
typedef signed   short s16;

typedef unsigned long u32;
typedef signed   long s32;



typedef enum {
	FLASE = 0,
	TRUE
}Bool_t;

typedef enum {
	OK = 0,
	NOK,
	NULLPTR,
	OUTOFRANGE
}Error_t;

#define NULLPTR ((void*)0)
#define NULL	((char)0)

#define MAX_U8  ((u8)255)
#define MIN_U8	((u8)0)
#define MAX_S8  ((s8)127)
#define MIN_S8	((s8)-128)
#define ZERO_S8 ((s8)0)
#define MAX_S16 ((s16)32767)
#define MIN_S16 ((s16)-32768)
#define ZERO_S16 ((s16)0)
#define MAX_S32 ((s32)2147483647)
#define MIN_S32 ((s32)-2147483648)
#define ZERO_S32 ((s32)0)

#endif /* STDTYPES_H_ */