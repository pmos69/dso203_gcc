/********************* (C) COPYRIGHT 2010 e-Design Co.,Ltd. ********************
 File Name : file.h  
 Version   : DS203_APP Ver 2.5x                                  Author : bure
*******************************************************************************/
#ifndef __FILE_H
#define __FILE_H

#include "stm32f10x_lib.h"

//#define FLASH_WAIT_TIMEOUT      100000 
//#define PAGE_0                  0 
//#define PAGE_1                  1 
//#define PAGE_2                  2 
//#define PAGE_3                  3 

#define OK           0            // operation is completed
//#define SEC_ERR      1            // sectors read and write errors
//#define FAT_ERR      2            // FAT table to read and write errors
#define OVER         3            // operations overflow
#define NEW          4            // new directory entry
#define EMPT         4            // file does not exist
#define VER_ERR      5            // wrong version
#define SUM_ERR      6            // checksum error
#define RD_ERR       8            // sectors read error
#define WR_ERR       9            // sector write error
#define DISK_ERR     10           // disk error

extern u8  FileBuff[1600];

u8 Save_Bmp(u8 FileNum);
u8 Color_Num(u16 Color);
u8 Load_Param(void);
u8 Save_Param(void);
u8 Save_Dat(u8 FileNum);
u8 Load_Dat(u8 FileNum);
u8 Save_Buf(u8 FileNum);
u8 Load_Buf(u8 FileNum);
u8 Save_Csv(u8 FileNum);
u8 Make_Filename(u8 FileNum,char* FileName);
void reset_parameter(void);
#endif
/********************************* END OF FILE ********************************/
