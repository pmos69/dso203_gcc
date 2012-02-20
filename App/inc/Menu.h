/********************* (C) COPYRIGHT 2010 e-Design Co.,Ltd. ********************
 File Name : Menu.h  
 Version   : DS203_APP Ver 2.4x                                  Author : bure
*******************************************************************************/
#ifndef __MENU_H
#define __MENU_H

#include "stm32f10x_lib.h"

//----------- The Detail menu constants MARK defined -----------
#define NUM2    0x01   // display the Value of two values ??in the specified location
#define NUM3    0x02   // in the specified location to display the floating point value of the Value of 3
#define CIRC    0x10   // the Detail Value in the regulation of head circulation
#define FIX     0x20   // the Detail Value change the identity of the corresponding string unchanged
#define NOT     0x80   // skip the Detail item choice
//#define NONE    0x00   // Other

//----------- Flag of the definition of variables in the Detail menu -----------
#define HID     0x04   // display the Detail
#define UPDAT   0x02   // Detail entry display refresh
#define BLINK   0x01   // the Detail display flashes

#define BATTERY    4


//=============== MENU Main Menu item is number defined ===============
#define TRACK1     0
#define TRACK2     1
#define TRACK3     2
#define TRACK4     3

#define RUNNING    4
#define OUTPUT     5

#define T_BASE     6
#define TRIGG      7    // 
#define V_VERNIE   8
#define FILE       9
#define T_VERNIE   10
#define BK_LIGHT   11
#define VOLUME     12

//------------- TRACKn submenu item number defined --------------  Dettagli TRACK
#define SOURCE     0      //Sorgente
#define COUPLE     1
#define RANGE      2
#define POSI       3

//............ TRACKn SOURCE item number defined ........... 
#define HIDE       0
/**/
#define CH_A       1
#define CH_B       1
#define CH_C       1
#define CH_D       1
#define A_add_B    2
#define A_sub_B    3
#define C_and_D    4
#define C_or_D     5
#define REC_1      6    //FILE1,2..4
#define REC_2      7
#define REC_3      8
#define REC_4      9

//------------- RUNNING sub-menu item number defined -------------
#define STATE      0
#define STATUS     1

//.............. STATE submenu items ID defined  ..............
#define RUN        0
#define HOLD       1

//-------------- T_BASE submenu item number defined -------------
#define MODE       0
#define BASE       1
#define XPOSI      2
#define VIEW       3

//............. T_BASE in MODE number defined ............
#define AUTO       0
#define NORM       1
#define SGL        2
#define SCAN       3
#define X_Y        4
#define X_Y_A      5
//-------------- OUTPUT submenu item number defined -------------
#define KIND       0
#define FRQN       1
#define DUTYPWM    2
#define OUTATT     3
//#define CLASS      1

//--------------- TRIGG sub menu item, the number defined -------------
#define SOURCE     0
#define TR_KIND    1
#define THRESHOLD  2

//------------- T_VERNIE submenu item number defined ------------
#define T1         0
#define T2         1

//------------- V_VERNIE submenu item number defined ------------
#define V1         0
#define V2         1

//---------------  FILE submenu item number defined --------------
#define SAVE       0
#define LOAD       1
#define BMP        0
#define DAT        1
#define BUF     2
#define CSV     3

//--------- BK_LIGHT the VOLUME sub-menu item number defined --------
#define CLASS      1

//============== METER main menu items defined number ============== 
#define METER_0    13
#define METER_1    14
#define METER_2    15
#define METER_3    16
#define METER_4    17
#define METER_5    18
#define METER_6    19
#define METER_7    20
#define METER_8    21

//------------- The METER in the Item number defined -------------
#define VBT        0
#define FPS        1
#define VPP        2
#define VDC        3
#define RMS        4
#define MAX        5
#define MIN        6
#define FRQ        7
#define CIR        8 
#define DUT        9
#define TH        10
#define TL        11

#define _Meas_V_Track   (Title[V_VERNIE][2].Value)             //Sorgente Delta V
#define _Meas_V_Range   (Title[_Meas_V_Track][RANGE].Value)        
#define _Meas_V_Scale   (Y_Attr[_Meas_V_Range].SCALE)
#define _V1_Vernie      (Title[V_VERNIE][0].Value)
#define _V2_Vernie      (Title[V_VERNIE][1].Value)
#define _D_V_Source     Title[V_VERNIE][2]
#define _Delta_V        Title[V_VERNIE][3]
#define _Delta_T        Title[T_VERNIE][3]
#define _T_Range        (Title[T_BASE][1].Value)
#define _T_Scale        (X_Attr[_T_Range].SCALE)

typedef struct 
{
  char   *Str;
  u8   Track;
  u8   Item;
  uc16 XPOS1;
  uc16 XPOS2;
  uc8  YPOS;
  u8   Flag;       // HID=0x04, UPD=0x02, BLINK=0x01, 
} meter; 

typedef struct 
{
  char   *Str;
  u16  *Color;
  s16  Limit;      // Max Value 
  uc8  MARK;       // NUM=0x04, FIX=0x02, CIR=0x01,
  uc16 XPOS;
  uc8  YPOS;
  s16  Value;
  u8   Flag;       // HID=0x04, UPD=0x02, BLINK=0x01, 
} menu; 

extern menu Title[13][4];
extern meter Meter[9];
extern u8 Current, TypeA, Update;
extern u8 Detail[14];
extern char NumStr[12];
extern u16 Result_FPS;
extern char T_UNIT[12], S_UNIT[12], P_UNIT[12], V_UNIT[12];
extern u8 x10;

extern uc16 Y_COLOR[5];                     // Track Color 2
void EnableMeter(void);
void DisableMeter(void);
void Display_Value(u8 i);
void Display_Title(void);
void Display_Meter(void);
void Load_Attr(void);
void Update_Battery(void);

#endif

/********************************* END OF FILE ********************************/
