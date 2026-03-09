#ifndef STDTYPES_H_
#define STDTYPES_H_
typedef signed char 	s8;
typedef signed short  	s16;
typedef signed long 	s32;
typedef char 	            c8;
typedef unsigned char 		u8;
typedef unsigned short  	u16;
typedef unsigned long 	u32;
#define NULLPTR ((void*)0)
#define  MAX_u8 255
#define  MAX_u16 65535
#define  MAX_u32 4294967295
#define  MAX_s8 127
#define  MAX_s16 32767
#define  MAX_s32 2147483647
#define  MIN_u8 0
#define  MIN_u16 0
#define  MIN_u32 0
#define  MIN_s8 -128
#define  MIN_s16 -32768
#define  MIN_s32 -2147483648
#define SET_BIT(REG,BIT) (REG|=(1<<BIT))
#define CLR_BIT(REG,BIT) (REG&=(~(1<<BIT)))
#define TOG_BIT(REG,BIT) (REG^=(1<<BIT))
#define GET_BIT(REG,BIT) (((REG>>BIT)&1))
typedef enum
{
	FALSE=0X55,
	TRUE=0XAA
}bool_t;
typedef enum
{
	OK,
	NOK,
	NULL_PTR,
	OUT_OF_RANGE,
	IN_PROGRESS
}error_t;
#define  F_CPU 8000000 // processor speed is 8MHZ
#include <util/delay.h> // library for function delay
#endif /* STDTYPES_H_ */