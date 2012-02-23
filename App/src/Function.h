/********************* (C) COPYRIGHT 2010 e-Design Co.,Ltd. ********************
 File Name : Function.h  
 Version   : DS203_APP Ver 2.3x                                  Author : bure
*******************************************************************************/
#ifndef __Function_H__
#define __Function_H__

#include "stm32f10x_lib.h"

#define NO_KEY       0
#define KEY1         1
#define KEY2         2
#define KEY3         3
#define KEY4         4
#define K_INDEX_DEC  5
#define K_INDEX_INC  6
#define K_INDEX_S    7
#define K_ITEM_INC   8
#define K_ITEM_DEC   9
#define K_ITEM_S     10

#define SIGN         0
#define UNSIGN       1
#define STD          2

/*
typedef struct 
{
    u8 decPos;
    u8 len;
    u8 str[9];
} I32STR_RES;
*/
extern u32 TestCnt;

u32  Power(u8 x, u8 y); 
void Delayms(u16 mS); 
void Int2Str(char *p, s32 n, char *pUnit, u8 e, u8 Mode);
//void Int32String_sign(I32STR_RES *r, s32 n);
//void Int32String(I32STR_RES *r, u32 n, u32 e);
u8   Str2Byte(char x,char y);
void u16ToDec5(char *p, u16 n);
void u8ToDec3(char *p, u8 n);
void u8ToDec2(char *p, u8 n);
void Char2Hex(char *p, u8 n);
void Shor2Hex(char *p, u16 n);
void Word2Hex(char *p, u32 n);
u32  Int_sqrt(u32 n);
u8   Read_Keys(void);
void s8ToPercen(char *p, s8 n);

#endif
/********************************* END OF FILE ********************************/
