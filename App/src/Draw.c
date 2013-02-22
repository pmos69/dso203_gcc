///******************** (C) COPYRIGHT 2009 e-Design Co.,Ltd. *********************
// File Name : Draw.c       
// Version   : DS203_APP Ver 2.3x                                  Author : bure
//*******************************************************************************/
#include "Process.h"
#include "Draw.h"
#include "Menu.h"
#include "BIOS.h"
#include "Function.h"

u16 MAX_X;
uc16 RULE_BASE[8] ={0x020,0x040,0x080,0x040,0x020,0x010,0x008,0x010};
u8 OffsetX;
u8 OffsetY;
u16 SpecRow = 0;
u8 colors[200];
u16 RowMem, RowMemPeak;

uc8  Mark_TAB[5][7] ={{0x00,0x00,0x42,0xFE,0x02,0x00,0x00},    // Mark 1
                      {0x00,0x46,0x8A,0x92,0x92,0x62,0x00},    // Mark 2
                      {0x00,0x44,0x82,0x92,0x92,0x6C,0x00},    // Mark 3
                      {0x00,0x18,0x28,0x4A,0xFE,0x0A,0x00},    // Mark 4
                      {0x00,0x40,0x40,0x7E,0x40,0x40,0x00}};   // Mark T

uc16 CLK_TAB[44] =
  {0x000,0x000,0x000,0x000,0x070,0x3FF,0x070,0x000,0x000,0x000,0x000,
   0x000,0x002,0x004,0x008,0x070,0x070,0x070,0x080,0x100,0x200,0x000,
   0x020,0x020,0x020,0x020,0x070,0x070,0x070,0x020,0x020,0x020,0x020,
   0x000,0x200,0x100,0x080,0x070,0x070,0x070,0x008,0x004,0x002,0x000};

uc16 Char_TAB_8x11[744] = {
  0x000,0x000,0x000,0x780,0x040,0x020,0x020,0x020, // "the upper left corner
//  0x020,0x020,0x040,0x780,0x000,0x000,0x000,0x000, //  # the upper right corner
  0x1FC,0x1FC,0x1FC,0x1FC,0x1FC,0x1FC,0x1FC,0x000, // # box
//  0x000,0x000,0x000,0x00F,0x010,0x020,0x020,0x020, // $ lower left corner
  0x070,0x0F8,0x1FC,0x1FC,0x1FC,0x0F8,0x070,0x000, // $ dot
  0x30C,0x18C,0x0C0,0x060,0x030,0x318,0x30C,0x000, // % 
  0x000,0x180,0x260,0x21C,0x26A,0x284,0x140,0x000, // & 
  0x202,0x202,0x202,0x202,0x202,0x202,0x3FE,0x000, // ' battery Last empty
  0x000,0x000,0x0F8,0x1FC,0x306,0x202,0x000,0x000, // (
  0x000,0x000,0x202,0x306,0x1FC,0x0F8,0x000,0x000, // )
  0x000,0x18C,0x0D8,0x070,0x070,0x0D8,0x18C,0x000, // * x
  0x000,0x020,0x020,0x0F8,0x0F8,0x020,0x020,0x000, // + 
  0x020,0x020,0x010,0x00F,0x000,0x000,0x000,0x000, // , the lower right corner
  0x000,0x020,0x020,0x020,0x020,0x020,0x020,0x000, // -
  0x000,0x000,0x300,0x300,0x000,0x000,0x000,0x000, // .
  0x180,0x0C0,0x060,0x030,0x018,0x00C,0x006,0x000, // / 
  0x1FC,0x3FE,0x242,0x222,0x212,0x3FE,0x1FC,0x000, // 0
  0x000,0x208,0x20C,0x3FE,0x3FE,0x200,0x200,0x000, // 1
  0x304,0x386,0x2C2,0x262,0x232,0x31E,0x30C,0x000, // 2
  0x104,0x306,0x222,0x222,0x222,0x3FE,0x1DC,0x000, // 3
  0x060,0x070,0x058,0x24C,0x3FE,0x3FE,0x240,0x000, // 4
  0x11E,0x21E,0x212,0x212,0x212,0x3F2,0x1E2,0x000, // 5
  0x1F8,0x3FC,0x226,0x222,0x222,0x3E0,0x1C0,0x000, // 6
  0x006,0x006,0x3C2,0x3E2,0x032,0x01E,0x00E,0x000, // 7
  0x1DC,0x3FE,0x222,0x222,0x222,0x3FE,0x1DC,0x000, // 8
  0x01C,0x23E,0x222,0x222,0x322,0x1FE,0x0FC,0x000, // 9
  0x000,0x000,0x000,0x198,0x198,0x000,0x000,0x000, // : 
  0x2FA,0x2FA,0x2FA,0x2FA,0x2FA,0x2FA,0x2FA,0x2FA, // ; battery body
  0x000,0x020,0x070,0x0D8,0x18C,0x306,0x202,0x000, // <
  0x090,0x090,0x090,0x090,0x090,0x090,0x000,0x000, // =  
  0x000,0x202,0x306,0x18C,0x0D8,0x070,0x020,0x000, // >
  //0x018,0x01C,0x004,0x344,0x364,0x03C,0x018,0x000, // ?
  0x07F,0x008,0x0D4,0x322,0x441,0x300,0x0C0,0x000, // ? Kv
  //0x1F8,0x104,0x272,0x272,0x2F2,0x284,0x078,0x000, // @ 
  0x3FE,0x020,0x3FE,0x000,0x320,0x2A0,0x260,0x000, // @		Hz
  0x3F0,0x3F8,0x04C,0x046,0x04C,0x3F8,0x3F0,0x000, // A
  0x202,0x3FE,0x3FE,0x222,0x222,0x3FE,0x1DC,0x000, // B
  0x0F8,0x1FC,0x306,0x202,0x202,0x306,0x18C,0x000, // C
  0x202,0x3FE,0x3FE,0x202,0x306,0x1FC,0x0F8,0x000, // D
  0x202,0x3FE,0x3FE,0x222,0x272,0x306,0x38E,0x000, // E
  0x202,0x3FE,0x3FE,0x222,0x072,0x006,0x00E,0x000, // F
  0x0F8,0x1FC,0x306,0x222,0x222,0x1E6,0x3EC,0x000, // G
  0x3FE,0x3FE,0x020,0x020,0x020,0x3FE,0x3FE,0x000, // H
  0x000,0x000,0x202,0x3FE,0x3FE,0x202,0x000,0x000, // I
  0x1C0,0x3C0,0x200,0x202,0x3FE,0x1FE,0x002,0x000, // J
  0x202,0x3FE,0x3FE,0x030,0x0F8,0x3CE,0x306,0x000, // K
  0x202,0x3FE,0x3FE,0x202,0x200,0x200,0x300,0x000, // L
  0x3FE,0x3FE,0x01C,0x038,0x01C,0x3FE,0x3FE,0x000, // M
  0x3FE,0x3FE,0x01C,0x038,0x070,0x3FE,0x3FE,0x000, // N
  0x0F8,0x1FC,0x306,0x202,0x306,0x1FC,0x0F8,0x000, // O
  0x202,0x3FE,0x3FE,0x222,0x022,0x03E,0x01C,0x000, // P
  0x1FC,0x3FE,0x202,0x282,0x302,0x3FE,0x1FC,0x000, // Q
  0x202,0x3FE,0x3FE,0x022,0x062,0x3FE,0x39C,0x000, // R
  0x10C,0x31E,0x232,0x222,0x262,0x3CE,0x18C,0x000, // S
  0x000,0x00E,0x206,0x3FE,0x3FE,0x206,0x00E,0x000, // T
  0x1FE,0x3FE,0x200,0x200,0x200,0x3FE,0x1FE,0x000, // U
  0x07E,0x0FE,0x180,0x300,0x180,0x0FE,0x07E,0x000, // V
  0x0FE,0x3FE,0x380,0x1E0,0x380,0x3FE,0x0FE,0x000, // W
  0x306,0x3DE,0x0F8,0x020,0x0F8,0x3DE,0x306,0x000, // X
  0x000,0x01E,0x23E,0x3E0,0x3E0,0x23E,0x01E,0x000, // Y
  0x38E,0x3C6,0x262,0x232,0x31E,0x38E,0x000,0x000, // Z
  0x200,0x300,0x2C0,0x220,0x218,0x204,0x3FE,0x000, // [ Triangle       
  0x000,0x022,0x042,0x1FE,0x3FE,0x240,0x220,0x000, // \ falling edge   
  0x020,0x010,0x008,0x006,0x008,0x010,0x020,0x020, // ] Pointer   
  0x000,0x220,0x210,0x3FC,0x3FE,0x012,0x022,0x000, // ^ rising edge
  0x000,0x200,0x200,0x200,0x200,0x200,0x200,0x000, // _ under the dash
  0x202,0x202,0x202,0x202,0x202,0x202,0x202,0x202, // ` battery itself empty
  0x1C0,0x3E8,0x228,0x228,0x1F8,0x3F0,0x200,0x000, // a         
  0x202,0x3FE,0x1FE,0x220,0x220,0x3E0,0x1C0,0x000, // b 
  0x1E0,0x3F0,0x210,0x210,0x210,0x330,0x120,0x000, // c 
  0x000,0x1C0,0x3E0,0x220,0x222,0x3FE,0x3FE,0x000, // d 
  0x1E0,0x3F0,0x250,0x250,0x250,0x370,0x160,0x000, // e 
  0x000,0x220,0x3FC,0x3FE,0x222,0x002,0x004,0x000, // f
  0x130,0x378,0x248,0x248,0x3F8,0x1F8,0x000,0x000, // g
  0x202,0x3FE,0x3FE,0x020,0x010,0x3F0,0x3E0,0x000, // h
  0x000,0x000,0x200,0x3F6,0x3F6,0x200,0x000,0x000, // i 
  0x000,0x100,0x300,0x210,0x3F6,0x1F6,0x000,0x000, // j
  0x202,0x3FE,0x3FE,0x062,0x0F0,0x398,0x308,0x000, // k
  0x000,0x000,0x202,0x3FE,0x3FE,0x200,0x000,0x000, // l
  0x3F8,0x3F8,0x018,0x3F0,0x018,0x3F8,0x3F0,0x000, // m 
  0x008,0x3F8,0x3F0,0x008,0x008,0x3F8,0x3F0,0x000, // n 
  0x000,0x1F0,0x3F8,0x208,0x208,0x3F8,0x1F0,0x000, // o
  0x208,0x3F8,0x3F0,0x248,0x048,0x078,0x030,0x000, // p 
  0x030,0x078,0x048,0x248,0x3F8,0x3F8,0x200,0x000, // q
  0x208,0x3F8,0x3F0,0x218,0x008,0x018,0x030,0x000, // r 
  0x000,0x110,0x338,0x268,0x248,0x3D8,0x190,0x000, // s 
  0x010,0x010,0x1F8,0x3FC,0x210,0x310,0x100,0x000, // t
  0x1F8,0x3F8,0x200,0x200,0x1F8,0x3F8,0x200,0x000, // u 
  0x000,0x0F8,0x1F8,0x300,0x300,0x1F8,0x0F8,0x000, // v
  0x0F8,0x3F8,0x300,0x180,0x300,0x3F8,0x0F8,0x000, // w
  0x208,0x318,0x1B0,0x0E0,0x1B0,0x318,0x208,0x000, // x
  0x038,0x278,0x240,0x240,0x240,0x1F8,0x0F8,0x000, // y
  0x318,0x388,0x2C8,0x268,0x238,0x318,0x000,0x000, // z 
  0x0F8,0x088,0x38E,0x022,0x2FA,0x2FA,0x2FA,0x2FA, // { battery head
  0x000,0x000,0x000,0x3FE,0x3FE,0x000,0x000,0x000, // |
  0x2FA,0x2FA,0x2FA,0x2FA,0x2FA,0x202,0x3FE,0x000, // } battery last
  0x0F8,0x088,0x38E,0x202,0x202,0x202,0x202,0x202};// ~ battery empty head 


   
uc16  Row_FullGray[201] =     // with only the vertical edges of the column the basis of data
{GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY , // 00~10
 GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY , // 10~20
 GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY , // 20~30
 GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY , // 30~40
 GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY , // 40~50
 GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY , // 50~60
 GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY , // 60~70
 GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY , // 70~80
 GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY , // 80~90
 GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY , // 90~00
 GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY , // 00~10
 GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY , // 10~20
 GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY , // 20~30
 GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY , // 30~40
 GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY , // 40~50
 GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY , // 50~60
 GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY , // 60~70
 GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY , // 70~80
 GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY , // 80~90
 GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY ,GRAY , // 90~00
 GRAY };
   
uc16   Row_FullBlack[201] =     // column contains only the level of edge basic data
{GRAY ,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK, // 00~10
 BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK, // 10~20
 BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK, // 20~30
 BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK, // 30~40
 BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK, // 40~50
 BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK, // 50~60
 BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK, // 60~70
 BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK, // 70~80
 BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK, // 80~90
 BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK, // 90~00
 BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK, // 00~10
 BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK, // 10~20
 BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK, // 20~30
 BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK, // 30~40
 BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK, // 40~50
 BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK, // 50~60
 BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK, // 60~70
 BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK, // 70~80
 BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK, // 80~90
 BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK, // 90~00
 GRAY };
   
uc16  Row_HorizDots[201] =     // column base data with the horizontal grid lines and horizontal edges
{GRAY ,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK, // 00~10
 BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK, // 10~20
 BLACK,BLACK,BLACK,BLACK,BLACK,GRAY ,BLACK,BLACK,BLACK,BLACK, // 20~30
 BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK, // 30~40
 BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK, // 40~50
 GRAY ,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK, // 50~60
 BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK, // 60~70
 BLACK,BLACK,BLACK,BLACK,BLACK,GRAY ,BLACK,BLACK,BLACK,BLACK, // 70~80
 BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK, // 80~90
 BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK, // 90~00
 GRAY ,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK, // 00~10
 BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK, // 10~20
 BLACK,BLACK,BLACK,BLACK,BLACK,GRAY ,BLACK,BLACK,BLACK,BLACK, // 20~30
 BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK, // 30~40
 BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK, // 40~50
 GRAY ,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK, // 50~60
 BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK, // 60~70
 BLACK,BLACK,BLACK,BLACK,BLACK,GRAY ,BLACK,BLACK,BLACK,BLACK, // 70~80
 BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK, // 80~90
 BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK, // 90~00
 GRAY };
   
uc16  Row_VertDots[201] =     // with the vertical grid lines and horizontal edges of the column the basis of data
{GRAY ,BLACK,BLACK,BLACK,BLACK,GRAY ,BLACK,BLACK,BLACK,BLACK, // 00~10
 GRAY ,BLACK,BLACK,BLACK,BLACK,GRAY ,BLACK,BLACK,BLACK,BLACK, // 10~20
 GRAY ,BLACK,BLACK,BLACK,BLACK,GRAY ,BLACK,BLACK,BLACK,BLACK, // 20~30
 GRAY ,BLACK,BLACK,BLACK,BLACK,GRAY ,BLACK,BLACK,BLACK,BLACK, // 30~40
 GRAY ,BLACK,BLACK,BLACK,BLACK,GRAY ,BLACK,BLACK,BLACK,BLACK, // 40~50
 GRAY ,BLACK,BLACK,BLACK,BLACK,GRAY ,BLACK,BLACK,BLACK,BLACK, // 50~60
 GRAY ,BLACK,BLACK,BLACK,BLACK,GRAY ,BLACK,BLACK,BLACK,BLACK, // 60~70
 GRAY ,BLACK,BLACK,BLACK,BLACK,GRAY ,BLACK,BLACK,BLACK,BLACK, // 70~80
 GRAY ,BLACK,BLACK,BLACK,BLACK,GRAY ,BLACK,BLACK,BLACK,BLACK, // 80~90
 GRAY ,BLACK,BLACK,BLACK,BLACK,GRAY ,BLACK,BLACK,BLACK,BLACK, // 90~00
 GRAY ,BLACK,BLACK,BLACK,BLACK,GRAY ,BLACK,BLACK,BLACK,BLACK, // 00~10
 GRAY ,BLACK,BLACK,BLACK,BLACK,GRAY ,BLACK,BLACK,BLACK,BLACK, // 10~20
 GRAY ,BLACK,BLACK,BLACK,BLACK,GRAY ,BLACK,BLACK,BLACK,BLACK, // 20~30
 GRAY ,BLACK,BLACK,BLACK,BLACK,GRAY ,BLACK,BLACK,BLACK,BLACK, // 30~40
 GRAY ,BLACK,BLACK,BLACK,BLACK,GRAY ,BLACK,BLACK,BLACK,BLACK, // 40~50
 GRAY ,BLACK,BLACK,BLACK,BLACK,GRAY ,BLACK,BLACK,BLACK,BLACK, // 50~60
 GRAY ,BLACK,BLACK,BLACK,BLACK,GRAY ,BLACK,BLACK,BLACK,BLACK, // 60~70
 GRAY ,BLACK,BLACK,BLACK,BLACK,GRAY ,BLACK,BLACK,BLACK,BLACK, // 70~80
 GRAY ,BLACK,BLACK,BLACK,BLACK,GRAY ,BLACK,BLACK,BLACK,BLACK, // 80~90
 GRAY ,BLACK,BLACK,BLACK,BLACK,GRAY ,BLACK,BLACK,BLACK,BLACK, // 90~00
 GRAY };
   
u16 Color[16] = { CYAN,   // #0  TRACK1 
                  YEL,    // #1  TRACK2
                  PURPL,  // #2  TRACK3 
                  GRN,    // #3  TRACK4 
                  WHT,    // #4  VERNIE   
                  BLACK,  // #5  SCRN
                  ORANGE, // #6  X_POSI 
                  BLUE,   // #7  TRIGG 
                  CYAN,   // #8  VERNIE  
                  GRAY,   // #9  GRID 
                  WHT,    // #10 TEXT 
                  GRN,    // #11 TEXT2
                  BLUE,   // #12 TEXT3
                  GRAY,   // #13 BLOCK
                  YEL,    // #14 SIDE
                  RED };  // #15 NOTE 

trigg V_Trigg[4] = {// Value,  Flag:( HID=0x04, UPDAT=0x02 )
                     {   175,  UPDAT + HID },
                     {   116,  UPDAT       },
                     {    75,  UPDAT + HID },
                     {    35,  UPDAT + HID },
                   };
u16 LCD_Buffers[2][240]; 

/*******************************************************************************
 Get_TAB_8x11
*******************************************************************************/
u16 Get_TAB_8x11(u8 Code, u8 Row)
{
    if ((Code == 0x20)||(Code == 0x21)) {
        return 0x0000;
    } else {
        return Char_TAB_8x11[((Code-0x22)*8)+Row];
    }
}

/*******************************************************************************
 Get_Ref_Wave: 
*******************************************************************************/
/*u8 Get_Ref_Wave(u16 i)
{
  return Ref_Wave[i];
}*/
/*******************************************************************************
 Print_Clk:  progress indicator
*******************************************************************************/
void Print_Clk(u16 x0, u16 y0, u16 Type, u8 Phase)
{
  u16 i, j, b;
  
  __LCD_Set_Block(x0, x0+10, y0, y0+10);
  for(i=0; i<11; ++i){
    b = CLK_TAB[Phase *11 + i];
    for(j=0; j<11; ++j){
      if((b >> j)& 0x001) __LCD_SetPixl(Color[Type >> 0x8]);
      else                __LCD_SetPixl(Color[Type & 0x0F]);
    }
  }
  __LCD_Set_Block(LCD_X1,LCD_X2,LCD_Y1,LCD_Y2);  // restore the full-size window
}
/*******************************************************************************
 Print_Str: 
*******************************************************************************/
void Print_Str(u16 x0, u16 y0, u16 Type, u8 Mode, char *s)
{ 
  signed short i, j, b; 
  __LCD_Set_Block(x0, LCD_X2, y0, y0+10);
  for (j=0; j<11;++j){ 
    if(Mode == 0) __LCD_SetPixl(Color[Type & 0x0F]);   //Normal replace Display
    else          __LCD_SetPixl(Color[Type >> 0x8]);   //Inverse replace Display
  }
  x0++;                            // a string to add a blank row
  while (*s!=0) {
    for(i=0;i<8;++i){
      if((*s==0x20)||(*s==0x21)) b = 0x0000;
      else                       b = Get_TAB_8x11(*s, i);
      if((*s==0x21)&&(i==4)) break;
      for(j=0;j<11;++j){
        if((b << j)& 0x400) {
          if(Mode == 0) __LCD_SetPixl(Color[Type >> 0x8]);
          else          __LCD_SetPixl(Color[Type & 0x0F]);
        } else {
          if(Mode == 0) __LCD_SetPixl(Color[Type & 0x0F]);
          else          __LCD_SetPixl(Color[Type >> 0x8]);
        }
      }
    }
    if(*s==0x21) x0 +=4;           // display position horizontally +4 ("!" character - shift by 1/2 a character)
    else  x0 += 8;                 // display position horizontally +8
    ++s;                           // string pointer +1
  }
  __LCD_Set_Block(LCD_X1,LCD_X2,LCD_Y1,LCD_Y2);  // restore the full-size window
}

/*******************************************************************************
 Print_Str_Row: 
*******************************************************************************/
void Print_Str_Row(u16 Row, u16* LCD_Buffer, u16 x0, u16 y0, u16 Type, u8 Mode, char *s)
{ 
    signed short i, y, b; 
    u16 char_width, w = x0;

    while(*s != 0) {
        if ((*s) == 0x21) {
            char_width = CHAR_WIDTH/2 + 1;
        } else {
            char_width = CHAR_WIDTH + 1;
        }
        if ((Row >= w) && (Row < (w + char_width))) {
            i = Row - w - 1;
            if (i < 0) {
                b = 0x0000;
            } else {
                b = Get_TAB_8x11(*s, i);
            }
            for (y = 0; y < CHAR_HEIGHT; y++) {
                if((b << y)& 0x400) {
                    if(Mode == 0) {
                        LCD_Buffer[y0 + y] = Color[Type >> 0x8];   //Inverse replace Display
                    } else {
                        LCD_Buffer[y0 + y] = Color[Type & 0x0F];   //Normal replace Display
                    }
                } else {
                    if(Mode == 0) {
                        LCD_Buffer[y0 + y] = Color[Type & 0x0F];   //Normal replace Display
                    } else {
                        LCD_Buffer[y0 + y] = Color[Type >> 0x8];   //Inverse replace Display
                    }
                }
            }
            break;
        }
            
        w += char_width;
        s++;
    }
}

/*******************************************************************************
 Draw_Row : to ease the DMA conflict, in two buffers alternately
*******************************************************************************/
void Draw_Row_Oscill(u16 Row, u16 *LCD_Buffer)
{ 
    u8  t, i, y[8], Dot_Hide[8]; 
    s16 Tmp, m, n ;
    int fftx, val= 0;
  
  // waveform display data preprocessing
    m = (Row - MIN_X-1)* 4;
    n = (Row - MIN_X)  * 4;
    for(i = 0; i < 8; i += 2) {
        Dot_Hide[i] = 0;
        y[i]   = TrackBuff[m + i/2];                  // endpoint to extract
        y[i+1] = TrackBuff[n + i/2];

        if(y[i]   >= Y_BASE+Y_SIZE)  y[i]   = Y_BASE+Y_SIZE-1;      // bounds
        else if(y[i]   <= Y_BASE+1)  y[i]   = Y_BASE+1;   
        if(y[i+1] >= Y_BASE+Y_SIZE)  y[i+1] = Y_BASE+Y_SIZE-1;
        else if(y[i+1] <= Y_BASE+1)  y[i+1] = Y_BASE+1;

        if(y[i] == y[i+1]){
            if((y[i] == Y_BASE+1)||(y[i] == Y_SIZE-1)) Dot_Hide[i] = 1;  // super-sector blanking
            else {
                if(y[i] >= Y_BASE+2)           y[i]   -= 1;              // horizontal bold
                if(y[i+1] <= Y_BASE+Y_SIZE-2)  y[i+1] += 1;
            }
        }
        if(y[i] > y[i+1]){                                             // order of
            Tmp = y[i+1]; y[i+1]= y[i]; y[i]= Tmp; 
        }
    }
  
//------------------------- Draw the Curve data --------------------------------
    for (t = TR_1; t <= TR_4; t++) {
        if((Dot_Hide[(t-TR_1)*2] == 0)&&(Title[TRACK1+(t-TR_1)][SOURCE].Value != HIDE)) {
            for(i=y[(t-TR_1)*2]; i<=y[(t-TR_1)*2+1]; ++i) {
				LCD_Buffer[i] |=Color[t];        // normal brightness
            }
        }
    }

    // FFT ///////
    if (ShowFFT) {
        fftx = Row - MIN_X; // starts at 1
        if (fftx < FFTSize/2) {
            //val = arrin[fftx];
            val = fr[fftx];
            if (val >= 200) val = 199;
            for (i=Y_BASE+1; i<val; i++) LCD_Buffer[i] = GRN; //0x0ff0;
            if ((fftx == imax - 4) || (fftx == imax + 4)) {
                for (i=PeakFreq - 1; i<PeakFreq+1; i++) {
                    LCD_Buffer[i] = RED;
                    RowMem=Row; 
                }
            } else if ((fftx == imax - 3) || (fftx == imax + 3)) {
                LCD_Buffer[PeakFreq] = RED;
            } else if ((fftx == imax - 5) || (fftx == imax + 5)) {
                for (i=PeakFreq - 2; i<PeakFreq+2; i++) { 
                    LCD_Buffer[i] = RED;
                }
            }
        } else {
            for (i=Y_BASE+1; i<200; i++) {
                LCD_Buffer[i] |= BLACK;
            }
        }
    }
		
//------------------------- Draw the Trigg Vernie data -------------------------
    for (t = TR_1; t <= TR_4; t++) {
        if(Title[TRIGG][SOURCE].Value == TRACK1+(t-TR_1)) 
            LCD_Buffer[V_Trigg[TRACK1+(t-TR_1)].Value] |= Color[t];
//------------------------- Draw the X Vernie data -----------------------------
      Tmp =(MIN_X + 150)- _X_posi.Value;
      if(Tmp > MIN_X) {
        if((Row == Tmp)&&((_X_posi.Flag & HID)== 0)){
          for(i = 1; i < Y_SIZE; i+=3) LCD_Buffer[i] |= Color[X_POSI];
        }
        if((Row == Tmp-1)||(Row == Tmp+1)){
          LCD_Buffer[Y_SIZE] = Color[X_POSI];
          LCD_Buffer[Y_BASE] = Color[X_POSI];
        }
        if(Row == Tmp){
          LCD_Buffer[Y_SIZE]   = Color[X_POSI];
          LCD_Buffer[Y_SIZE-1] = Color[X_POSI];
          LCD_Buffer[Y_BASE+1] = Color[X_POSI];
          LCD_Buffer[Y_BASE]   = Color[X_POSI];
        }
      }
      Tmp = MIN_X + Title[T_VERNIE][T1].Value;
      if((Row == Tmp)&&((Title[T_VERNIE][T1].Flag & HID)== 0)){
        for(i = 1; i < Y_SIZE; i+=3) LCD_Buffer[i] |= Color[VERNIE];
      }
      if((Row == Tmp-1)||(Row == Tmp+1)){
        LCD_Buffer[Y_SIZE] |= Color[VERNIE];
        LCD_Buffer[Y_BASE] |= Color[VERNIE];
      }
      if(Row == Tmp){
        LCD_Buffer[Y_SIZE]   |= Color[VERNIE];
        LCD_Buffer[Y_SIZE-1] |= Color[VERNIE];
        LCD_Buffer[Y_BASE+1] |= Color[VERNIE];
        LCD_Buffer[Y_BASE]   |= Color[VERNIE];
      }
      Tmp = MIN_X + Title[T_VERNIE][T2].Value;
      if((Row == Tmp)&&((Title[T_VERNIE][T2].Flag & HID)== 0)){
        for(i = 1; i < Y_SIZE; i+=3) LCD_Buffer[i] |= Color[VERNIE];
      }
      if((Row == Tmp-1)||(Row == Tmp+1)){
        LCD_Buffer[Y_SIZE] = Color[VERNIE];
        LCD_Buffer[Y_BASE] = Color[VERNIE];
      }
      if(Row == Tmp){
        LCD_Buffer[Y_SIZE]   = Color[VERNIE];
        LCD_Buffer[Y_SIZE-1] = Color[VERNIE];
        LCD_Buffer[Y_BASE+1] = Color[VERNIE];
      }
    } 
//------------------------- Draw the Y Vernie data -----------------------------
    if((Row==MIN_X+1)||(Row==MAX_X-1)){
        LCD_Buffer[Title[V_VERNIE][V1].Value] |= Color[VERNIE];
        LCD_Buffer[Title[V_VERNIE][V2].Value] |= Color[VERNIE];
    }
      if((Title[V_VERNIE][V1].Flag & HID)== 0) 
        LCD_Buffer[Title[V_VERNIE][V1].Value] |= Color[VERNIE];
      if((Title[V_VERNIE][V2].Flag & HID)== 0) 
        LCD_Buffer[Title[V_VERNIE][V2].Value] |= Color[VERNIE];

    if (ShowFFT) {
        Print_Str_Row(Row, LCD_Buffer, 245, 4, 0xFFFF, INV, NFreqStr);
        if ( PeakFreq>5){
            if  ( RowMemPeak < 35)   
                RowMemPeak = 35;
            if (PeakFreq+14 > 178) {
                Print_Str_Row(Row, LCD_Buffer,  RowMemPeak-20,  178, 0xFFFF, INV, PeakFreqStr);
            } else {
                Print_Str_Row(Row, LCD_Buffer,  RowMemPeak-20,  PeakFreq+14, 0xFFFF, INV, PeakFreqStr);
            }
        }
        Print_Str_Row(Row, LCD_Buffer, 15, 189, 0xFFFF, INV, "Div:" ); 
        Print_Str_Row(Row, LCD_Buffer, 50,189, 0xFFFF, INV, FreqDivStr);
        Print_Str_Row(Row, LCD_Buffer, 200,189, 0xFFFF, INV, " T1:" ); 
        Print_Str_Row(Row, LCD_Buffer, 235,189,0xFFFF, INV, FreqT1Str);
    }
}

/*******************************************************************************
 Draw_Oscill_Grid :  
*******************************************************************************/
void Draw_Oscill_Grid(u16 Row, u16 *LCD_Buffer)
{
    if(Row == MAX_X) {
        __Row_Copy(Row_FullGray, LCD_Buffer);
    } else if(Row == MIN_X) {
        __Row_Copy(Row_FullGray, LCD_Buffer);
    } else if((Row - MIN_X)%30 == 0) {
        __Row_Copy(Row_VertDots, LCD_Buffer);
    } else if((Row - MIN_X)%6  == 0) {
        __Row_Copy(Row_HorizDots, LCD_Buffer);
    } else {
        __Row_Copy(Row_FullBlack, LCD_Buffer);
    }
}
/*******************************************************************************
 Draw_XY_Grid :  
*******************************************************************************/
void Draw_XY_Grid(u16 Row, u16 *LCD_Buffer)
{
    if ((Row == 260)||(Row+1 == MAX_X)) {
        __Row_Copy(Row_FullGray, LCD_Buffer);
    } else if(Row == MIN_X) {
        __Row_Copy(Row_FullGray, LCD_Buffer);
    } else if(((Row+1 - MIN_X)%25 == 0)&& (Row<260)) {
        __Row_Copy(Row_VertDots, LCD_Buffer);
    } else if(((Row+1 - MIN_X)%5  == 0)&& (Row<260)) {
        __Row_Copy(Row_HorizDots, LCD_Buffer);
    } else if (Row != 260) {
        __Row_Copy(Row_FullBlack, LCD_Buffer);
    }
}
/*******************************************************************************
 Draw_Spec_Grid :  
*******************************************************************************/
void Draw_Spec_Grid(u16 Row, u16 *LCD_Buffer)
{
    __Row_Copy(Row_FullBlack, LCD_Buffer);
}
/*******************************************************************************
 Draw_Row :  
*******************************************************************************/
void Draw_Row(u16 Row)
{
    u16* LCD_ActiveBuffer = LCD_Buffers[Row & 1];
    u16* LCD_NextBuffer = LCD_Buffers[((Row & 1) + 1) & 1];

  __Row_DMA_Ready();
  __Point_SCR(Row, MIN_Y);
    
    switch(_Mode) {
        case AUTO:
        case NORH:
		case NORC:
        case SGL:
        case SCAN:
			if ((_4_source!=SPEC_A) && (_4_source!=SPEC_B)){
				Draw_Oscill_Grid(Row, LCD_NextBuffer);
				if ((Row > MIN_X) && (Row < MAX_X)) {
					Draw_Row_Oscill(Row, LCD_ActiveBuffer); //Modo oscilloscopio
				}
			} else {
				Draw_Spec_Grid(Row, LCD_NextBuffer);
				if ((Row > MIN_X) && (Row < MAX_X)) {
					Draw_Row_Spec(Row, LCD_ActiveBuffer); //Spectogram
				}
			}
            break;
        case X_Y:
            Draw_XY_Grid(Row, LCD_NextBuffer);
            if ((Row > MIN_X) && (Row < MAX_X)) {
                Draw_Row_XY(Row, LCD_ActiveBuffer); //Modo X Y
            }
            break;
    }
    __LCD_Copy(LCD_ActiveBuffer, Y_SIZE+1);               // Row Transitive

}

/*******************************************************************************
 Draw_Window :  
*******************************************************************************/
void Draw_Window(void)						
{ 
    u16 Row;
    u16 h;
   
    __Row_Copy(Row_FullBlack, LCD_Buffers[0]);
    __Row_DMA_Ready();

    if (((_Mode==X_Y)) && (Title[TRACK1][SOURCE].Value == HIDE)) {
        for (h = 0; h <= X_SIZE; ++h) {
            TrackBuff[h*4]=105;       //#pmos69 limit by X_SIZE, instead of 400
        }
    }
    if (((_Mode==X_Y)) && (Title[TRACK2][SOURCE].Value == HIDE)) {
        for (h = 0; h <= X_SIZE; ++h) {
            TrackBuff[(h*4)+1]=100;   //#pmos69 limit by X_SIZE, instead of 400
        }
    }

    //if (_Mode == SPEC) {
	if ((_4_source==SPEC_A) || (_4_source==SPEC_B)) {
        if (SpecRow > MAX_X) {
            SpecRow = 0;
        }
        Draw_Row(SpecRow);
        SpecRow++;
    } else {
        for(Row = MIN_X; Row <= MAX_X; ++Row) {
            Draw_Row(Row);
        }
    }

    RowMemPeak = RowMem;
    __LCD_DMA_Ready();
    __Row_DMA_Ready();
}
/*******************************************************************************
 Draw_Mark :  Routine per un marcatore laterale
*******************************************************************************/
void Draw_Mark(u16 m, u16 n)						
{ 
  u16 i, j, Tmp;
  
  if(m < 4)  { if(Title[m][POSI].Value > 200-3) return; }
  else       { if(V_Trigg[n].Value > 200-3)     return; }
  
  for(i = 0; i <= 10; ++i){
    if(i < 7){
      if(m < 4){
        Tmp = Mark_TAB[m][i];
        __Point_SCR(i, Title[m][POSI].Value +(MIN_Y-4));
      } else {
        Tmp = Mark_TAB[4][i];
        __Point_SCR(i,     V_Trigg[n].Value +(MIN_Y-4));
      }
      for(j = 0; j < 8; ++j){
        if(Tmp & 1) __LCD_SetPixl(Color[SCRN]);
        else        __LCD_SetPixl(Color[n]);
        Tmp >>= 1;
      }
      __LCD_SetPixl(Color[n]);
    } else {
      if(m < 4){
        __Point_SCR(i, Title[m][POSI].Value +(MIN_Y-10)+ i);
        for(j=Title[m][POSI].Value+(MIN_Y-10)+i;j<(Title[m][POSI].Value+(MIN_Y+11)-i);++j) 
          __LCD_SetPixl(Color[n]); 
      } else { 
        __Point_SCR(i,     V_Trigg[n].Value +(MIN_Y-10)+ i);   
        for(j=V_Trigg[n].Value+(MIN_Y-10)+i;j<(V_Trigg[n].Value+(MIN_Y+11)-i);++j) 
          __LCD_SetPixl(Color[n]); 
      }
    }
  }
}
/*******************************************************************************
 Update_Mark :  Disegna Marcatori laterali
*******************************************************************************/
void Update_Mark(void)						
{ 
  s16 i, Tmp; 
  u8  j;
  
  __Row_DMA_Ready();
  for(i = 0; i <= 10; ++i){
    __Point_SCR(i, MIN_Y-1);
    for(j = MIN_Y-1; j < MAX_Y+2; ++j)  __LCD_SetPixl(Color[SCRN]);   // Clear row
  }
  if(_1_source  != HIDE)    Draw_Mark(TRACK1, TRACK1); // Display Track1 mark
  if(_2_source  != HIDE)    Draw_Mark(TRACK2, TRACK2); // Display Track1 mark
  if(_3_source  != HIDE)    Draw_Mark(TRACK3, TRACK3); // Display Track1 mark
  if(_4_source  != HIDE)    Draw_Mark(TRACK4, TRACK4); // Display Track1 mark
  if(_Tr_source == TRACK1)  Draw_Mark(TRIGG,  TRACK1); // Display Trigg1 mark
  if(_Tr_source == TRACK2)  Draw_Mark(TRIGG,  TRACK2); // Display Trigg1 mark
  if(_Tr_source == TRACK3)  Draw_Mark(TRIGG,  TRACK3); // Display Trigg1 mark
  if(_Tr_source == TRACK4)  Draw_Mark(TRIGG,  TRACK4); // Display Trigg1 mark
    
  __Point_SCR(MAX_X+1, MIN_Y-1);
  for(j = MIN_Y-1; j < MAX_Y+2; ++j) __LCD_SetPixl(Color[SCRN]); // Clear last row 
  for(i = MIN_X; i <= MAX_X+1; ++i)
  {
    __Point_SCR(i, MIN_Y-1);
    __LCD_SetPixl(Color[SCRN]);                            // Clear first column
    __Point_SCR(i, MAX_Y+1);
    __LCD_SetPixl(Color[SCRN]);                            // Clear last column
  }
  __Point_SCR(MIN_X-1, Title[V_VERNIE][V1].Value +(MIN_Y - 2));
  for(i = 0; i < 5; ++i) __LCD_SetPixl(Color[VERNIE]);
  __Point_SCR(MIN_X-1, Title[V_VERNIE][V2].Value +(MIN_Y - 2));
  for(i = 0; i < 5; ++i) __LCD_SetPixl(Color[VERNIE]);
  __Point_SCR(MAX_X+1, Title[V_VERNIE][V1].Value +(MIN_Y - 2));
  for(i = 0; i < 5; ++i) __LCD_SetPixl(Color[VERNIE]);
  __Point_SCR(MAX_X+1, Title[V_VERNIE][V2].Value +(MIN_Y - 2));
  for(i = 0; i < 5; ++i) __LCD_SetPixl(Color[VERNIE]);
  
  Tmp =(MIN_X + 150)- _X_posi.Value;
  if(Tmp > MIN_X){
    for(i = Tmp - 2; i <= Tmp + 2; ++i){
      __Point_SCR(i, MIN_Y-1);
      __LCD_SetPixl(Color[X_POSI]);
      __Point_SCR(i, MAX_Y+1);
      __LCD_SetPixl(Color[X_POSI]);
    }
  }
  Tmp = MIN_X + Title[T_VERNIE][T1].Value;
  for(i = Tmp - 2; i <= Tmp + 2; ++i){
    __Point_SCR(i, MIN_Y-1);
    __LCD_SetPixl(Color[VERNIE]);
    __Point_SCR(i, MAX_Y+1);
    __LCD_SetPixl(Color[VERNIE]);
  }
  Tmp = MIN_X + Title[T_VERNIE][T2].Value;
  for(i = Tmp - 2; i <= Tmp + 2; ++i){
    __Point_SCR(i, MIN_Y-1);
    __LCD_SetPixl(Color[VERNIE]);
    __Point_SCR(i, MAX_Y+1);
    __LCD_SetPixl(Color[VERNIE]);
  }
}
/*******************************************************************************
 Update_View_Area: Disegna rettangolo con zig zag posizione buffer 
*******************************************************************************/
void Update_View_Area(void)
{  
  s16 i, j, k;
  {
    __Point_SCR(89,  0);
    for(j=0; j<12; ++j) __LCD_SetPixl(ORANGE); 
    __Point_SCR(90,  0);
    for(j=0; j<12; ++j) __LCD_SetPixl(ORANGE); 
    __Point_SCR(308, 0);
    for(j=0; j<12; ++j) __LCD_SetPixl(ORANGE); 
    __Point_SCR(309, 0);
    for(j=0; j<12; ++j) __LCD_SetPixl(ORANGE); 

    for(i=91; i<307; i+=8){
      for(j=0; j<8; ++j){
        __Point_SCR(i+j, 0);
        for(k=0; k<12; ++k){
          if((0x01 << k)& RULE_BASE[j]) __LCD_SetPixl(ORANGE);
          else                          __LCD_SetPixl(BLACK);
        }
      }
    }
    
	// draw rectangle for full buffer mode or small buffer mode
	  __Point_SCR(0  + 91 + (_X_posi.Value * 200)/3720, 0); //3796
      for(j=0;j<9;++j) __LCD_SetPixl(WHT);
	  __Point_SCR((217*300/get_bag_max_buf())+1 + 91 + (_X_posi.Value * 200)/3720, 0);
      for(j=0;j<9;++j) __LCD_SetPixl(WHT);
	  for(i=0;i<(217*300/get_bag_max_buf())+1;++i){
        __Point_SCR(i + 91 +(_X_posi.Value * 200)/3720, 0);
        __LCD_SetPixl(WHT);
        __Point_SCR(i + 91 +(_X_posi.Value * 200)/3720, 9);
        __LCD_SetPixl(WHT);
      }
    _X_View.Flag &= !UPDAT;
  }
}
/*******************************************************************************
Clear_Meter_Area  Pulisce l'area sotto ai meter per quando vengono rivisualizzati
*******************************************************************************/
void Clear_Meter_Area(void)
{  
  s16 i, j;
  { 
    for(j=312;j<400;++j){
      for(i=11;i<216;++i){
        __Point_SCR(j , i);
        __LCD_SetPixl(BLACK);
      }
    }
    _X_View.Flag &= !UPDAT;
  }
}

/*******************************************************************************
Draw_Row_XY   Disegna il grafico X Y
*******************************************************************************/
void Draw_Row_XY(u16 Row, u16 *LCD_Buffer)
{ 
  u8  i, y[8]; 
  s16 Tmp, m, n ;
    m = (Row - MIN_X-1)* 4;
    n = (Row - MIN_X)  * 4;
    for(i = 0; i < 8; i += 2) {
    
      y[i]   = TrackBuff[m + i/2];              
      y[i+1] = TrackBuff[n + i/2];
      
      if(y[i]   >= Y_BASE+Y_SIZE)  y[i]   = Y_BASE+Y_SIZE-1;     
      else if(y[i]   <= Y_BASE+1)  y[i]   = Y_BASE+1;   
      if(y[i+1] >= Y_BASE+Y_SIZE)  y[i+1] = Y_BASE+Y_SIZE-1;
      else if(y[i+1] <= Y_BASE+1)  y[i+1] = Y_BASE+1;
      
      if(y[i] == y[i+1]){
        if((y[i] == Y_BASE+1)||(y[i] == Y_SIZE-1)) ; 
        else {
          if(y[i] >= Y_BASE+2)           y[i]   -= 1;            
          if(y[i+1] <= Y_BASE+Y_SIZE-2)  y[i+1] += 1;
        }
      }
      if(y[i] > y[i+1]){                                             
        Tmp = y[i+1]; y[i+1]= y[i]; y[i]= Tmp; 
      }
    }
  
   u16 h;
  u16 TempY;
   u16 TempX;
   
//------------------------- Draw the Curve data --------------------------------
  for(h = 0; h <= 255; ++h)
  {
     TempX=(TrackBuff[(h*4)])+OffsetX-108;   //*5/4
     TempY=TrackBuff[(h*4)+1]+OffsetY-100;
      
     if ((TempY>0) && (TempY<200) && (TempX>0) && (TempX<250)){
         
         if   (TempX==Row)
         {LCD_Buffer[TempY] |= Color[TR_4];
         
     }    
     }
         
         if (h== OffsetY)
         {
           if (Row>5)
            {
              if ((Row<((OffsetX)+5)) && (Row>((OffsetX)-5))){  
              LCD_Buffer[h] |= Color[TR_3];
            }
           }
         }
         
         
         
         if ((Row==OffsetX))
         {
           if (h>5)
            {
              if ((h<OffsetY+5) && (h>OffsetY-5)){  
              LCD_Buffer[h] |= Color[TR_3];
            }
           }
         }
  
 } 
}

u16 to565( u32 clr )
  {
    u8 r = clr>>16;
    u8 g = clr>>8;
    u8 b = clr;
    r >>= 3; // 8->5 bitov
    g >>= 2;
    b >>= 3;
    u16 ret = r;
    ret |= g<<5;
    ret |= b<<11; // 5+6
    return ret;
  }

u32 interpolate(u32 clra, u32 clrb, u8 ratio)
  {
    u8 ra = clra>>16;
    u8 ga = clra>>8;
    u8 ba = clra;

    u8 rb = clrb>>16;
    u8 gb = clrb>>8;
    u8 bb = clrb;

    u16 r = ra + (((rb-ra)*ratio)>>8);
    u16 g = ga + (((gb-ga)*ratio)>>8);
    u16 b = ba + (((bb-ba)*ratio)>>8);
    r &= 0xff;
    g &= 0xff;
    b &= 0xff;
    return (r<<16) | (g<<8) | b;
  }

  void InitColors()
  {
    static const u32 baseclr[] = {0x000080, 0x0000ff, 0x00ffff, 0xffffff, 0xff0000};
	u8 i;
	
    for (i=0; i<200; i++)
    {
      u8 base = i/50;
      u16 rem = (i%50)*255/50;
      colors[i] = to565( interpolate(baseclr[base], baseclr[base+1], rem) )/256;
    }
  }

/*******************************************************************************
 Draw_Row_Spec : to ease the DMA conflict, in two buffers alternately
*******************************************************************************/
void Draw_Row_Spec(u16 Row, u16 *LCD_Buffer)
{ 
  u8  i;
  int val= 0;
   
  InitColors();
  
//------------------------- Draw the Curve data --------------------------------
    for (i=Y_BASE+1; i<200; i++) {
    	val = fr[i-Y_BASE];
    	if (val >= 200) val = 199;
    	LCD_Buffer[i] = colors[val]*256;
    }
     
}

// void DrawPixel(u16 x, u16 y, u16 clr) {
    // __Point_SCR(x, y);
    // __LCD_SetPixl(clr);
// }
  
// void DrawBar(u16 x1, u16 y1, u16 x2, u16 y2, u16 clr) {
	// u16 c;
	
    // __LCD_Set_Block(x1, x2, y1, y2);
    // c = (x2-x1)*(y2-y1);
    // while (c--) __LCD_SetPixl(clr);
    // __LCD_Set_Block(LCD_X1,LCD_X2,LCD_Y1,LCD_Y2);  // restore the full-size window
// }
/******************************** END OF FILE *********************************/
