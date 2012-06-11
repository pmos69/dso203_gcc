/********************* (C) COPYRIGHT 2010 e-Design Co.,Ltd. ********************
 File Name : Draw.h  
 Version   : DS203_APP Ver 2.3x                                  Author : bure
*******************************************************************************/
#ifndef __DRAW_H
#define __DRAW_H

#include "stm32f10x_lib.h"

typedef struct 
{
  s16  Value;
  u8   Flag;       // HID=0x04, UPDAT=0x02, 
} trigg; 

//------------------- ��Ļ��ʾ��ɫ��Ŷ��� ----------------------
#define TR_1       0
#define TR_2       1
#define TR_3       2
#define TR_4       3
#define VERNIE     4
#define SCRN       5
#define GRID       9
#define NOTE2      7
#define NOTE3      8
#define X_POSI     6
#define TEXT1      10
#define TEXT2      11
#define TEXT3      12
#define BLOCK      13
#define SIDE       14
#define NOTE1      15


#define SINE       0
#define TRIANG     1
#define SAW        2
#define DIGI       3
#define PWM        4
#define NOOUT	   5

#define PRN        0   // No inverse Display
#define INV        1   // Inverse Display


#define WHT        0xFFFF
#define CYAN       0xFFE0
#define CYAN_      0xBDE0
#define YEL        0x07FF
#define YEL_       0x05F7
#define PURPL      0xF81F
#define PURPL_     0xB817
#define GRN        0x07E0
#define GRN_       0x05E0
#define GRAY       0x7BEF   
#define BLUE       0xF800
#define ORANGE     0x051F
#define RED        0x001F
#define BLACK      0x0000


#define LCD_X1      0
#define LCD_X2      399
#define LCD_Y1      0
#define LCD_Y2      239
#define X_SIZE      392   //300  400
#define X_BASE      0
#define Y_SIZE      200
#define Y_BASE      0
#define MIN_X       11
#define MIN_Y       13
#define TRACK_OFFSET  2
//#define MAX_X       (X_SIZE + MIN_X)
#define MAX_Y       (Y_SIZE + MIN_Y)

#define CHAR_WIDTH  8
#define CHAR_HEIGHT 11


extern u8 OffsetX;
extern u8 OffsetY;
extern u16 MAX_X;
extern u8 FlagFrameMode;
extern u8 FlagMeter;
extern u8 TrgAuto;
extern u16 Color[16];

extern trigg V_Trigg[4];
u16  Get_TAB_8x11(u8 Code, u8 Row);
u8   Get_Ref_Wave(u16 i);
void Print_Clk(u16 x0, u16 y0, u16 Type, u8 Phase);
void Print_Str(u16 x0, u16 y0, u16 Color, u8 Mode, char *s);
void Print_Str_Row(u16 Row, u16* LCD_Buffer, u16 x0, u16 y0, u16 Color, u8 Mode, char *s);
void Draw_Row_Oscill(u16 Row, u16 *LCD_Buffer);
void Draw_Row_XY(u16 Row, u16 *LCD_Buffer);
void Draw_Row_Spec(u16 Row, u16 *LCD_Buffer);
void Draw_Window(void);						
void Draw_Mark(u16 m, u16 n);						
void Update_Mark(void);						
void Update_View_Area(void);
void Clear_Meter_Area(void);

// void DrawBar(u16 x1, u16 y1, u16 x2, u16 y2, u16 clr);
// void DrawPixel(u16 x, u16 y, u16 clr);

#endif

/********************************* END OF FILE ********************************/
