/********************* (C) COPYRIGHT 2010 e-Design Co.,Ltd. ********************
 File Name : Menu.c  
 Version   : DS203_APP Ver 2.5x                                  Author : bure
*******************************************************************************/
#include <string.h>
#include "Interrupt.h"
#include "Function.h"
#include "Process.h"
#include "Menu.h"
#include "Draw.h"
#include "BIOS.h"

//I32STR_RES Num;

u16 Result_FPS;
u8 x10;
u8 Cnt_InCharge;
u8 Cnt_Charged;
u8 Cnt_Batt;
u8 FlagInCharge;

//  u8 N[20];
//uc8 T_UNIT[12] ={'A','B', 0 ,'C','D', 0 ,'E','F', 0 ,'G','H', 0 };
char T_UNIT[12] ={'u','S', 0 ,'u','S', 0 ,'m','S', 0 ,'S',' ', 0 };
char V_UNIT[12] ={'m','V', 0 ,'m','V', 0 ,'V',' ', 0 ,'k','V', 0 };
char F_UNIT[12] ={'H','z', 0 ,'H','z', 0 ,'K','C', 0 ,'M','C', 0 };
char N_UNIT[12] ={ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 };
char S_UNIT[12] ={'/','S','e','c', 0 ,'/','S','e','c', 0 , 0 , 0 };
char P_UNIT[12] ={'%',' ', 0 ,'%',' ', 0 ,'%',' ', 0 , 0 , 0 , 0 };
//u8 V_Unit[4][3]={"uV","mV","V ","kV"};
//u8 T_Unit[5][3]={"nS","uS","mS","S ","  "};
//u8 F_Unit[5][4]={"Hz","Hz","KC","MC","  "};

char STATESTR[3][10] = {"!RUN!", "HOLD", "HOLD"};                  // Running state str

uc16 S_Inv[3]       = {(SCRN<<8)+TEXT2, (SCRN<<8)+X_POSI, (SCRN<<8)+NOTE1};         // Running state Color

char BATT_STR[5][10] = {"~``'", "~``}", "~`;}", "~;;}", "{;;}"};   // Battery Status Str
uc16 B_COLOR[5]     = {(NOTE1<<8)+SCRN, (SIDE <<8)+SCRN,
                       (TEXT2<<8)+SCRN, (TEXT2<<8)+SCRN,
                       (TEXT2<<8)+SCRN};                          // Battery Status Color

char CH_A_STR[4][10]  = {" -NO- ", "!CH(A)!", "x10(A)"};
char CH_B_STR[4][10]  = {" -NO- ", "!CH(B)!", "x10(B)"};
char CH_C_STR[2][10]  = {" -NO- ", "!CH(C)!"};
char CH_D_STR[10][10] = {" -NO- ", "!CH(D)!", "!(A+B)!", "!(A-B)!",  
                     "!(C&D)!", "!(C|D)!", "!REC_A!",  "!REC_B!",
                        "!REC_C!",  "!REC_D!"};                      // Track source Str
char NO_RANGE[5]      = "";//" -- ";                       
char NO_DEF[5]        = "";//"--";                       

char  YCOUPLE[3][10]  = {"DC", "AC", "!-!"};                        // Track Couple Str                    
char   Vertical[15][10];                                             // Track Range Str
char  YPOSISTR[5]    = {"YPOS"};                                    // Track Position Str 
uc16 Y_INV[5]       = {(SCRN<<8)+TR_1, (SCRN<<8)+TR_2,
                       (SCRN<<8)+TR_3, (SCRN<<8)+TR_4,
                       (SCRN<<8)+VERNIE};                          // Track Color 1
uc16 Y_COLOR[5]     = {(TR_1<<8)+SCRN, (TR_2<<8)+SCRN,
                       (TR_3<<8)+SCRN, (TR_4<<8)+SCRN,
                       (VERNIE<<8)+SCRN};                          // Track Color 2

char MODESTR[10][10]  = {"!AUTO!", "!NORM!","SINGL","!SCAN!","X_Y S","X_Y A"};  //,"!NONE!"                                  // Sync Mode Str
char   BaseStr[30][10];                                              // Time Base b Str
char  XPOSISTR[5]    = {"XPOS"};
uc16 XCOLOR[2]      = {(SCRN<<8)+X_POSI, (X_POSI<<8)+SCRN};        // Time Base Color

char FO_TYPE[5][10]  = {"!SINUS!",  "TRIANG",  "! SAW !",  "SQUARE", "! PWM !"}; // Output Kind Str  "Kd=  %"

char FO_STR[23][10]  = {"! 1Hz !",  "! 2Hz !",  "! 5Hz !",
                        " 10Hz ",  " 20Hz ",  " 50Hz ",   "!100Hz!",
                       "!200Hz!", "!500Hz!", " 1KHz ",   " 2KHz ",
                       " 5KHz ",  "!10KHz!", "!20KHz!",  "!50KHz!",
                       "100KHz",  "200KHz",  "500KHz",   " 1MHz ",
                       " 2MHz ",  " 4MHz ",  " 6MHz ",   " 8MHz "}; // Output Frequency Str

uc16 O_COLOR[2]     = {(SCRN<<8)+TEXT3, (TEXT3<<8)+SCRN};         // Output Frequency Color

char  TRIGSTR[8][10] = {"TRG A", "TRG B", "TRG C", "TRG D"};      // Trigger source Str
                          
char  TR_TYPE[8][10] = {":!\\!", ":!^!", "<Vt", ">Vt",            // Trigger Type Str
                       "<TL",   ">TL",  "<TH", ">TH"};

char  VT_STR[3]  =  "LEV";                                // Trigg Voltage Str

char  VERNIE1[5]     = {"!V1!"};          // V1 Vernie Str
char  VERNIE2[5]     = {"!V2!"};          // V2 Vernie Str
char  VERNIE3[5]     = {"!T1!"};          // T1 Vernie Str
char  VERNIE4[5]     = {"!T2!"};          // T2 Vernie Str
uc16 V_INV[1]       = {(SCRN<<8)+VERNIE};
uc16 V_COLOR[1]     = {(VERNIE<<8)+SCRN};                         // Y Vernie Color
uc16 T_INV[1]       = {(SCRN<<8)+VERNIE};
uc16 T_COLOR[1]        = {(VERNIE<<8)+SCRN};                         // X Vernie Color
 

char F_FUNC[2][10]  = {"Save File", "Load File"};                 // File Function Str

char  F_EXT[8][10]   = {".BMP?", ".DAT?", ".BUF?",".CSV?", " OK! ",
                         " ERR!",".HEX", ".BIN",};                  // File Ext Name Str
uc16 F_INV[1]       = {(SCRN<<8)+TEXT1};                          //  File Color
char  DELTA_V[2][10] = {"[V:", "[V:"};
char  DELTA_T[4]     = "[T:";
char  METER[12][5]   = {"Vbt", "FPS", "Vpp", "Vdc", "RMS",  "Max", 
                       "Min", "Frq", "Per", "Dut", "!TH!", "!TL!"};

u8   Detail[14];
char  NumStr[12];
u8   Current = 0, TypeA = 0, Update = 1;
char   BL_Str[5]="B.L", Vol_Str[5]="Vol";

menu Title[13][4]=   
{
  //     Item_String,   m_Color,   Limit, Mark, xPos, yPos, Value, Flag 
  {//============================ Title Track1 Group ===========================
    {(char*)CH_A_STR,(u16*)Y_INV,     3-1, CIRC,   35,  228,     1, UPDAT}, //  Track source   2
    {(char*)YCOUPLE, (u16*)Y_COLOR,   2-1, CIRC,   35,  216,     0, UPDAT}, //  Track Couple        
    {(char*)Vertical,(u16*)Y_COLOR,   9-1,    0,   51,  216,     5, UPDAT}, //  Track Range 9
    {(char*)YPOSISTR,(u16*)Y_INV,     200,  FIX,    0,    0,   150, UPDAT}, //  Adj. Track Position 160
  },
  {//============================ Title Track2 Group ===========================
    {(char*)CH_B_STR,(u16*)Y_INV+1,   3-1, CIRC,   86,  228,     1, UPDAT}, //  Track source   2
    {(char*)YCOUPLE, (u16*)Y_COLOR+1, 2-1, CIRC,   86,  216,     1, UPDAT}, //  Track Couple        
    {(char*)Vertical,(u16*)Y_COLOR+1, 9-1,    0,  102,  216,     1, UPDAT}, //  Track Range 9
    {(char*)YPOSISTR,(u16*)Y_INV+1,   200,  FIX,    0,    0,   100, UPDAT}, //  Adj. Track Position
  },
  {//============================ Title Track3 Group ===========================
    {(char*)CH_C_STR,(u16*)Y_INV+2,   2-1, CIRC,  137,  228,     1, UPDAT}, //  Track source
    {(char*)NO_DEF , (u16*)Y_COLOR+2, 1-1, CIRC,  137,  216,     0, HID}, //  Track Couple        
    {(char*)NO_RANGE,(u16*)Y_COLOR+2, 1-1,    0,  153,  216,     0, HID}, //  Track Range
    {(char*)YPOSISTR,(u16*)Y_INV+2, 200-1,  FIX,    0,    0,    60, UPDAT}, //  Adj. Track Position
  },
  {//============================ Title Track4 Group ===========================
    {(char*)CH_D_STR,(u16*)Y_INV+3,  10-1, CIRC,  188,  228,     1, UPDAT}, //  Track source
    {(char*)NO_DEF , (u16*)Y_COLOR+3, 1-1, CIRC,  188,  216,     0, UPDAT}, //  Track Couple        
    {(char*)NO_RANGE,(u16*)Y_COLOR+3, 1-1,    0,  204,  216,     0, UPDAT}, //  Track Range
    {(char*)YPOSISTR,(u16*)Y_INV+3, 200-1,  FIX,    0,    0,    20, UPDAT}, //  Adj. Track Position
  },
  {//================== Running State & Battery Status Group ===================
    {(char*)STATESTR,(u16*)S_Inv,     3-1,    0,    0,  228,     0, UPDAT}, //  Running state
    {(char*)BATT_STR,(u16*)B_COLOR,   5-1,    0,    0,  216,     0, UPDAT}, //  Battery status
    {(char*)BATT_STR,(u16*)B_COLOR,   5-1,  NOT,    0,    0,     0,   HID}, 
    {(char*)BATT_STR,(u16*)B_COLOR,   5-1,  NOT,    0,    0,     0,   HID},
  },
         
  {//======================= Title Output Signal Group =========================
    {(char*)FO_TYPE, (u16*)O_COLOR,   5-1, CIRC,  239,  228,     3, UPDAT}, //  Output Wave Kind    282 228     
    {(char*)FO_STR,  (u16*)O_COLOR+1,23-1,    0,  239,  216,    13, UPDAT}, //  Output Frequency    282 216
    {(char*)NumStr,  (u16*)O_COLOR,   100, NUM3,  212,  216,    50, UPDAT}, //  Duty value      282,202   
    {(char*)NumStr,  (u16*)O_COLOR,   100, NUM3,  196,  216,    50, UPDAT}, //  Attenuazione    251,202   
  },
   {//========================= Title Time Base Group ===========================
    {(char*)MODESTR, (u16*)XCOLOR,    6-1, CIRC,  290,  228,     0, UPDAT}, //  Sync Mode    5 numero modi  239  228
    {(char*)BaseStr, (u16*)XCOLOR+1, 27-1,    0,  290,  216,    17, UPDAT}, //  Time Base Range             239 216
    {(char*)XPOSISTR,(u16*)XCOLOR,   3695,  FIX,  366,    0,     0, UPDAT}, //  Adj. X position  3795
    {(char*)XPOSISTR,(u16*)XCOLOR,   3695,  NOT,   80,    0,     0, UPDAT}, //  View window rule  3795
  },   
  {//=====================;===== Title Trigger Group ====;======================
    {(char*)TRIGSTR, (u16*)Y_INV,     4-1, CIRC,  333,  228,     1, UPDAT}, //  Trigger source        
    {(char*)TR_TYPE, (u16*)Y_INV,     8-1, CIRC,  373,  228,     0, UPDAT}, //  Trigger Kine         
    {(char*)VT_STR,  (u16*)Y_COLOR,   200,  FIX,  333,  216,     0, UPDAT}, //  Adj. Trigger threshold     
    {(char*)VT_STR,  (u16*)Y_COLOR,     0,  NOT,  357,  216,     0,   HID}, //  Number position    0,  NOT,  357,  216,     0,   HID
  },
  {//============================ Title Y Vernie Group =========================
    {(char*)VERNIE1, (u16*)V_INV,   200-1,  FIX,   35,    0,   180, UPDAT}, //  V1 Vernie         
    {(char*)VERNIE2, (u16*)V_INV,   200-1,  FIX,   62,    0,    40, UPDAT}, //  V2 Vernie     
    {(char*)DELTA_V, (u16*)Y_INV,     2-1, CIRC,  314,  197,     0, UPDAT}, //  Measure Track    
    {(char*)NumStr,  (u16*)Y_COLOR,   240, NUM3,  342,  197,    70, UPDAT}, //  Delta V value    
  },
  {//============================= Title File Group ============================
    {(char*)F_FUNC,  (u16*)F_INV,     2-1, CIRC,  134,    0,     0,     0}, //  File function    
    {(char*)NumStr,  (u16*)F_INV,   256-1, NUM3,  206,    0,     0,     1}, //  File number     
    {(char*)F_EXT,   (u16*)F_INV,     4-1, CIRC,  230,    0,     0,     0}, //  Ext. name     
    {(char*)F_EXT,   (u16*)F_INV,       0,  NOT,    0,    0,     0,   HID},  
  },
  {//============================ Title X Vernie Group =========================
    {(char*)VERNIE3, (u16*)T_INV,     395,  FIX,  312,    0,    80, UPDAT}, //  T1 Vernie     300    
    {(char*)VERNIE4, (u16*)T_INV,     395,  FIX,  339,    0,   280, UPDAT}, //  T2 Vernie     300
    {(char*)DELTA_T, (u16*)T_INV,       0,  NOT,  314,  182,     0, UPDAT}, //  Delta T Str     
    {(char*)NumStr,  (u16*)T_COLOR,   400, NUM3,  342,  182,    80, UPDAT}, //  Delta T value 300   
  },
  {//=========================== Title BackLight Group =========================
    {(char*)BL_Str,  (u16*)V_INV,     1-1,  FIX,  314,  167,     0, UPDAT}, //  BackLight    
    {(char*)NumStr,  (u16*)V_COLOR,  10-1, NUM2,  342,  167,     5, UPDAT}, //  Class     
    {(char*)NumStr,  (u16*)V_INV,       0,  NOT,    0,    0,     0,   HID},     
    {(char*)NumStr,  (u16*)V_INV,       0,  NOT,    0,    0,     0,   HID},  
  },
  {//============================ Title Volume Group ===========================
    {(char*)Vol_Str, (u16*)V_INV,     1-1,  FIX,  314,  152,     0, UPDAT}, //  Volume        
    {(char*)NumStr,  (u16*)V_COLOR,  11-1, NUM2,  342,  152,     5, UPDAT}, //  Class     
    {(char*)NumStr,  (u16*)V_INV,       0,  NOT,    0,    0,     0,   HID},    
    {(char*)NumStr,  (u16*)V_INV,       0,  NOT,    0,    0,     0,   HID},    
  },
};

meter Meter[9] =
{ {(char*)METER,      4,    VBT,     314,    342,   137,  UPDAT}, //  Meter #0
  {(char*)METER,      4,    FPS,     314,    342,   122,  UPDAT}, //  Meter #1
  {(char*)METER, TRACK4,    FRQ,     314,    342,   107,  UPDAT}, //  Meter #2
  {(char*)METER, TRACK3,    CIR,     314,    342,    92,  UPDAT}, //  Meter #3
  {(char*)METER, TRACK3,    DUT,     314,    342,    77,  UPDAT}, //  Meter #4
  {(char*)METER, TRACK2,    TH,      314,    342,    62,  UPDAT}, //  Meter #5
  {(char*)METER, TRACK2,    TL,      314,    342,    47,  UPDAT}, //  Meter #6
  {(char*)METER, TRACK1,    VPP,     314,    342,    32,  UPDAT}, //  Meter #7
  {(char*)METER, TRACK1,    VDC,     314,    342,    17,  UPDAT}, //  Meter #8
};         



void Display_Meter(void)                  // 每次刷新显示一个测量项
{
  u8  i;
  
  for(i=0; i<9; ++i){        
    if(Meter[i].Flag & UPDAT){            //-----显示需刷新的测量项目名称
      Meter[i].Flag &= (!UPDAT & !BLINK);       // Clr Update & Blink flag
      Print_Str
        (Meter[i].XPOS1, Meter[i].YPOS,        // 需要显示的坐标
         Y_INV[Meter[i].Track],                // 需要显示的颜色(所属通道)
         PRN,                                  // 需要显示的方式
         Meter[i].Str +(Meter[i].Item * 5) );   // 需要显示的项目名称
    } 
   
  }
  if((Current >= METER_0)&&(Current <= METER_8)){
    if(Blink){                            //-----显示需闪烁的测量项目名称
      i = Current - METER_0;
      Blink = 0;                               // Clr Blink Ctrl flag 
      Print_Str(
        Meter[i].XPOS1, Meter[i].YPOS,         // 需要闪烁的坐标 
        Y_INV[Meter[i].Track],                 // 需要闪烁的颜色(所属通道)
        Twink,                                 // 闪烁方式
        Meter[i].Str +(Meter[i].Item *5));     // 需要闪烁的项目名称
    }
  }
}

void Display_Value(u8 i)
{
  s32 Tmp = 0;
  u16 Kp;
  u32 k, n, m ;
  u16 bag_max_buf = 4096;


// #pmos69 - Determine bag_max_buf - as in process.c 
if (FrameMode>0)      //_Mode == SCAN
  	{
    
    if (_Mode==SCAN)
      {
        if (FlagMeter==0) (bag_max_buf = (390*FrameMode));
        if (FlagMeter==1) (bag_max_buf = (305*FrameMode));
        
      } 
    else
      {
        bag_max_buf = (300*FrameMode);  //X_SIZE
      }
  
    }
  else
     {   
  //	bag_max_buf = X_SIZE - 10;
  	bag_max_buf = 4096;
        
       
}
  
  if(Interlace == 0) Kp = _Kp1; // 独立采样模式
  else               Kp = _Kp2; // 交替采样模式  
  
  k = _T_Range; m = 1;  n = 1;
  if(k < 9)  m = Power(10, (11-k)/3); //9 //11
  else       n = Power(10, (k- 9)/3);  //9
  k = X_Attr[(k%3)+9].SCALE; //9
  
  switch (Meter[i].Item){  
  case VBT://--------------- 计算和显示电池电压 ---------------
    Int2Str(NumStr, __Get(V_BATTERY)*1000, V_UNIT, 3, SIGN);
   
      break;
  case FPS://--------------- 计算和显示帧计数 ---------------
   Int2Str(NumStr, Result_FPS & 0x7F, S_UNIT, 2, STD);
  
    break;  
  case VPP:
    if(Meter[i].Track == TRACK1){
      Tmp = (Ka2[_A_Range]*(a_Max - a_Min)+ 512)/1024;
      if(Tmp <= 4) Tmp = 0;
      Tmp *= Y_Attr[_A_Range].SCALE;

      if  (_1_source ==2) Tmp=Tmp*10;
      if  (_1_source == HIDE) Tmp=0;
     }
    if((Meter[i].Track == TRACK2)&&(_2_source == CH_B)){
      Tmp = (Kb2[_B_Range]*(b_Max - b_Min)+ 512)/1024;
      if(Tmp <= 4) Tmp = 0;
      Tmp *= Y_Attr[_B_Range].SCALE;
      if  (_2_source ==2) Tmp=Tmp*10;
      if  (_2_source == HIDE) Tmp=0;
    }
   
    Int2Str(NumStr, Tmp, V_UNIT, 3, SIGN);
    break;
  case VDC:
    if(Meter[i].Track == TRACK1){
	Tmp = Ka1[_A_Range]+(Ka2[_A_Range]*(a_Avg/bag_max_buf)+ 512)/1024 - _1_posi;	// #pmos69 - use bag_max_buf as average divider
      if((Tmp >= -2)&&(Tmp <= 2)) Tmp = 0;
      Tmp *= Y_Attr[_A_Range].SCALE;
      if  (_1_source ==2) Tmp=Tmp*10;
      if  (_1_source == HIDE) Tmp=0;
   }
    if(Meter[i].Track == TRACK2){
      Tmp = Kb1[_B_Range]+(Kb2[_B_Range]*(b_Avg/bag_max_buf)+ 512)/1024 - _2_posi;	// #pmos69 - use bag_max_buf as average divider
      if((Tmp >= -2)&&(Tmp <= 2)) Tmp = 0;
      Tmp *= Y_Attr[_B_Range].SCALE;
      if  (_2_source ==2) Tmp=Tmp*10;
      if  (_2_source == HIDE) Tmp=0;
    }
    
    Int2Str(NumStr, Tmp, V_UNIT, 3, SIGN);
    break;
  case RMS:
    if(Meter[i].Track == TRACK1){
      Tmp = Ka1[_A_Range] +(Ka2[_A_Range]*Int_sqrt(a_Ssq/bag_max_buf)+ 512)/1024;	// #pmos69 - use bag_max_buf as average divider
      if(Tmp <= 2) Tmp = 0;
      Tmp *= Y_Attr[_A_Range].SCALE;
      
      if  (_1_source ==2) Tmp=Tmp*10;
      if  (_1_source == HIDE) Tmp=0;
    }
    if(Meter[i].Track == TRACK2){
      Tmp = Kb1[_B_Range] +(Kb2[_B_Range]*Int_sqrt(b_Ssq/bag_max_buf)+ 512)/1024;	// #pmos69 - use bag_max_buf as average divider
      if(Tmp <= 2) Tmp = 0;
      Tmp *= Y_Attr[_B_Range].SCALE;
      if  (_2_source ==2) Tmp=Tmp*10;
      if  (_2_source == HIDE) Tmp=0;
    }
  
    Int2Str(NumStr, Tmp, V_UNIT, 3, SIGN); //unsign
    break;
  case MAX:
    if(Meter[i].Track == TRACK1){
      Tmp = (Ka1[_A_Range] +(Ka2[_A_Range]*a_Max + 512)/1024 - _1_posi)* Y_Attr[_A_Range].SCALE;
      if  (_1_source ==2) Tmp=Tmp*10;
      if  (_1_source == HIDE) Tmp=0;
    }
    if(Meter[i].Track == TRACK2){
      Tmp = (Kb1[_B_Range] +(Kb2[_B_Range]*b_Max + 512)/1024 - _2_posi)* Y_Attr[_B_Range].SCALE;
      if  (_2_source ==2) Tmp=Tmp*10;
      if  (_2_source == HIDE) Tmp=0;
    }
    
    Int2Str(NumStr, Tmp, V_UNIT, 3, SIGN);
    break;
  case MIN:
    if(Meter[i].Track == TRACK1){
      Tmp = (Ka1[_A_Range] +(Ka2[_A_Range]*a_Min + 512)/1024 - _1_posi)* Y_Attr[_A_Range].SCALE;
      if  (_1_source ==2) Tmp=Tmp*10;
      if  (_1_source == HIDE) Tmp=0;
    }
    if(Meter[i].Track == TRACK2){
      Tmp = (Kb1[_B_Range] +(Kb2[_B_Range]*b_Min + 512)/1024 - _2_posi)* Y_Attr[_B_Range].SCALE;
      if  (_2_source ==2) Tmp=Tmp*10;
      if  (_2_source == HIDE) Tmp=0;
    }
    Int2Str(NumStr, Tmp, V_UNIT, 3, SIGN);
    break;
  case FRQ:
    if((Meter[i].Track == TRACK1)&&(_1_source == CH_A)){ 
      Tmp = 2000*((5000000 * TaN)/TaS);
      
    }
    if((Meter[i].Track == TRACK2)&&(_2_source == CH_B))
      Tmp = 2000*((5000000 * TbN)/TbS);
    if((Meter[i].Track == TRACK3)&&(_3_source == CH_C))
      Tmp = 2000*((5000000 * TcN)/TcS);
    if((Meter[i].Track == TRACK4)&&(_4_source == CH_D))
      Tmp = 2000*((5000000 * TdN)/TdS);
    
                                                                                        
     if(n < 10)  Int2Str(NumStr,((((250*(Tmp/(k/6)))/m)/15)),   F_UNIT, 4, UNSIGN);
     else  Int2Str(NumStr, ((Kp*(n*(Tmp/(k/6))/240)/256)*1000), F_UNIT, 4, UNSIGN); //aggiunto *1000   4
  
    
    break;  
  case CIR:
    if((Meter[i].Track == TRACK1)&&(_1_source == CH_A))
      Tmp = (k *TaS)/TaN;
    if((Meter[i].Track == TRACK2)&&(_2_source == CH_B))
      Tmp = (k *TbS)/TbN;
    if((Meter[i].Track == TRACK3)&&(_3_source == CH_C))
      Tmp = (k *TcS)/TcN;
    if((Meter[i].Track == TRACK4)&&(_4_source == CH_D))
      Tmp = (k *TdS)/TdN;
    if(Tmp <= 0x7FFFFFFF/m) Tmp = 1024*((m*Tmp)/Kp)/n;
    else                    Tmp = 0x80000000;
    Int2Str(NumStr, Tmp, T_UNIT, 4, UNSIGN);
    break;
  case DUT:
    if((Meter[i].Track == TRACK1)&&(_1_source == CH_A))
      Tmp = (100000*PaS)/TaS;
    if((Meter[i].Track == TRACK2)&&(_2_source == CH_B))
      Tmp = (100000*PbS)/TbS;
    if((Meter[i].Track == TRACK3)&&(_3_source == CH_C))
      Tmp = (100000*PcS)/TcS;
    if((Meter[i].Track == TRACK4)&&(_4_source == CH_D))
      Tmp = (100000*PdS)/TdS;
    Int2Str(NumStr, Tmp, P_UNIT, 4, UNSIGN);
    break;
  case TH:
    if((Meter[i].Track == TRACK1)&&(_1_source == CH_A))
      Tmp = (k*TaS)/TaN - (k*PaS)/TaN;
    if((Meter[i].Track == TRACK2)&&(_2_source == CH_B))
      Tmp = (100000*PbS)/TbS;
    if((Meter[i].Track == TRACK3)&&(_3_source == CH_C))
      Tmp = (100000*PcS)/TcS;
    if((Meter[i].Track == TRACK4)&&(_4_source == CH_D))
      Tmp = (100000*PdS)/TdS;
    if(Tmp <= 0x7FFFFFFF/m) Tmp = 1024*((m*Tmp)/Kp)/n;
    else                    Tmp = 0x80000000;
    Int2Str(NumStr, Tmp, T_UNIT, 4, UNSIGN);
    break;
  case TL:
    if((Meter[i].Track == TRACK1)&&(_1_source == CH_A))
      Tmp = (k*PaS)/TaN;
    if((Meter[i].Track == TRACK2)&&(_2_source == CH_B))
      Tmp = (k*PbS)/TbN;
    if((Meter[i].Track == TRACK3)&&(_3_source == CH_C))
      Tmp = (k*PcS)/TcN;
    if((Meter[i].Track == TRACK4)&&(_4_source == CH_D))
      Tmp = (k*PdS)/TdN;
    if(Tmp <= 0x7FFFFFFF/m) Tmp = 1024*((m*Tmp)/Kp)/n;
    else                    Tmp = 0x80000000;
    Int2Str(NumStr, Tmp, T_UNIT, 4, UNSIGN);
    break;
    }
  Print_Str(
    Meter[i].XPOS2, Meter[i].YPOS,
    Y_COLOR[Meter[i].Track], 
    PRN,
    NumStr);                              // 显示测量数值
}

void Display_Title(void)
{
  u16  i, j;
  s32  k, n, m;
  u8 z;
  
  
      if ((Current == OUTPUT)&&((_Det==DUTYPWM) || (_Det==OUTATT))){ z=Twink; } else { z=INV;}
     
      if  (Title[OUTPUT][KIND].Value == PWM)
      {
        u8ToDec3(Title[OUTPUT][DUTYPWM].Str, Title[OUTPUT][DUTYPWM].Value);
        Print_Str(156,  216, (Title[OUTPUT][DUTYPWM].Color[0]), z, "Duty % "); //225,202[Title[OUTPUT][PWM].Value]
        Print_Str(Title[OUTPUT][DUTYPWM].XPOS, Title[OUTPUT][DUTYPWM].YPOS,
                  (Title[OUTPUT][DUTYPWM].Color[0]),z,Title[OUTPUT][DUTYPWM].Str);
      }  
      else
      {
        Print_Str(156,  216, (Title[OUTPUT][DUTYPWM].Color[0]), z, "!!Out ");//230,202
        Int2Str(NumStr, (Title[OUTPUT][OUTATT].Value*26000), V_UNIT, 2, UNSIGN);
        Print_Str(Title[OUTPUT][OUTATT].XPOS, Title[OUTPUT][OUTATT].YPOS,
                  (Title[OUTPUT][OUTATT].Color[0]),z,NumStr);
      }
   
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  for(i = TRACK1; i <= VOLUME; ++i){
    
    
    
    
    
    
    
    
    
    
    for(j = 0; j < 4; ++j){
     if(Title[i][j].Flag & UPDAT){   // 需要刷新的Item
        Title[i][j].Flag &= ~UPDAT;   // Clr Update flag 
        
        if((i == BATTERY)||(i == TRIGG)){
          if(Title[i][j].MARK & FIX){                  // ---- Under fix mode
            Print_Str( 
              Title[i][j].XPOS, Title[i][j].YPOS,
              Title[i][j].Color[Title[i][0].Value],    // Color variable 
              PRN, 
              Title[i][j].Str                          // String fixed
            );                    
          } else if(!(Title[i][j].MARK & NOT)){        // ---- Under other mode
            Print_Str(
              Title[i][j].XPOS, Title[i][j].YPOS,
              Title[i][j].Color[Title[i][0].Value],    // Color variable 
              PRN, 
              Title[i][j].Str +(Title[i][j].Value * 10)// String variable
            ); 
          }
        } else {
          if(Title[i][j].MARK & FIX){                  // ---- Under fix mode
            Print_Str(
              Title[i][j].XPOS, Title[i][j].YPOS,
              Title[i][j].Color[0],                    // Color fixed 
              PRN, 
              Title[i][j].Str                          // String fixed
             );
          } else if(Title[i][j].MARK & NUM3){          // ---- Under NUM3 mode
            
              if(i == V_VERNIE){
             
                if  (Title[_Meas_V_Track][SOURCE].Value ==2) 
                  {Int2Str(NumStr,(_V1_Vernie-_V2_Vernie)*_Meas_V_Scale*10, V_UNIT, 3, SIGN);}
                else
                  {Int2Str(NumStr,(_V1_Vernie-_V2_Vernie)*_Meas_V_Scale, V_UNIT, 3, SIGN);
                }
               
              //  if  (_1_source ==2) Int2Str(NumStr,(_V1_Vernie-_V2_Vernie)*_Meas_V_Scale*10, V_UNIT, 3, SIGN);
              //  if  (_1_source !=2) Int2Str(NumStr,(_V1_Vernie-_V2_Vernie)*_Meas_V_Scale, V_UNIT, 3, SIGN);
                
              Print_Str(
                Title[i][j].XPOS, Title[i][j].YPOS,
                Title[i][j].Color[_Meas_V_Track],      // Color fixed  
                PRN, 
                NumStr                                // String for numerical
              );
            }
            if(i == T_VERNIE){
              m = 1;  n = 1;
              k = _T_Range;
              if(k < 9)  m = Power(10, (11-k)/3);
              else       n = Power(10, (k- 9)/3);
              k = X_Attr[(k%3)+9].SCALE *(_T2 - _T1);
              if(k > 0){
                if(k <=  0x7FFFFFFF/m)  k = (m * k)/n;
                else                    k = 0x80000000;
              } else {
                if((-k)> 0x7FFFFFFF/m)  k = (m * k)/n;
                else                    k = 0x80000000;
              }
              Int2Str(NumStr, k, T_UNIT, 3, SIGN);
              Print_Str(
                Title[i][j].XPOS, Title[i][j].YPOS,
                Title[i][j].Color[0],                  // Color fixed  
                PRN, 
                NumStr                                // String for numerical
              );
            }
            if(i==FILE){//??????????????????????????????
               u8ToDec3(Title[FILE][1].Str, Title[FILE][1].Value);
               Print_Str(
                Title[FILE][1].XPOS, Title[FILE][1].YPOS,
                Title[FILE][1].Color[0],                  // Color fixed  
                PRN, 
                Title[FILE][1].Str                        // String for numerical
              );
            }
            
            
            
          } else if(Title[i][j].MARK == NUM2){ 
                NumStr[0]=' ';
            if(i == BK_LIGHT){                        // 背光百分比显示处理
              if(Title[i][j].Value == 9){
                Int2Str(NumStr, 100, P_UNIT, 3, STD);
                } else Int2Str(NumStr, 10*(Title[i][j].Value+1), P_UNIT, 2, STD);
            } else {                                  // 音量百分比显示处理
              if(Title[i][j].Value == 10){
                Int2Str(NumStr, 100, P_UNIT, 3, STD);
              } else Int2Str(NumStr, 10*(Title[i][j].Value+1), P_UNIT, 2, STD);
            }
            Print_Str(
              Title[i][j].XPOS, Title[i][j].YPOS,
              Title[i][j].Color[0],                  // Color fixed  
              PRN, 
              NumStr                        // String for numerical
            );
          } else if(Title[i][j].MARK != NOT){
          
            if(i == V_VERNIE){
              Print_Str(
                Title[i][j].XPOS, Title[i][j].YPOS,
                Title[i][j].Color[_Meas_V_Track],         // Color variable 
                PRN, 
                Title[i][j].Str +(Title[i][j].Value * 10)// String variable
              ); 
            } else {
              Print_Str(
                Title[i][j].XPOS, Title[i][j].YPOS,
                Title[i][j].Color[0],                    // Color fixed 
                PRN, 
                Title[i][j].Str +(Title[i][j].Value * 10)// String variable
              ); 
            }
          } else if(i == T_VERNIE){
            Print_Str(
              Title[i][j].XPOS, Title[i][j].YPOS,
              Title[i][j].Color[0],                    // Color fixed 
              PRN, 
              Title[i][j].Str                          // String variable
            ); 
          }
        }//
          
        
        
      } else if((Current == i)&&(Detail[i] == j)&&(Blink)){ // 当前光标位置的Item
        Blink = 0;
        if((i == BATTERY)||(i == TRIGG)){
          if((Title[i][j].MARK & FIX)){      // ---- Under fix mode
            Print_Str( 
              Title[i][j].XPOS, Title[i][j].YPOS,
              Title[i][j].Color[Title[i][0].Value],    // Color variable 
              Twink, 
              Title[i][j].Str                          // String fixed
            );                    
          } else {                                // ---- Under other mode
            Print_Str(
              Title[i][j].XPOS, Title[i][j].YPOS,
              Title[i][j].Color[Title[i][0].Value],    // Color variable 
              Twink, 
              Title[i][j].Str +(Title[i][j].Value * 10)// String variable
            ); 
          }
        } else {
          if(Title[i][j].MARK & FIX){             // ---- Under fix mode
            Print_Str(
              Title[i][j].XPOS, Title[i][j].YPOS,
              Title[i][j].Color[0],                    // Color fixed 
              Twink, 
              Title[i][j].Str                          // String fixed
            );
     } else if(Title[i][j].MARK != NUM3){
                 if(i == V_VERNIE){
              Print_Str(
                Title[i][j].XPOS, Title[i][j].YPOS,
                Title[i][j].Color[_Meas_V_Track],        // Color variable=
                Twink, 
                Title[i][j].Str +(Title[i][j].Value * 10)// String variable
              ); 
            } else {
              Print_Str(
                Title[i][j].XPOS, Title[i][j].YPOS,
                Title[i][j].Color[0],                    // Color fixed 
                Twink, 
                Title[i][j].Str +(Title[i][j].Value * 10)// String variable
              ); 
      }
                  }
          
         
//       if ((_Det==DUTYPWM) || (_Det==OUTATT)){ z=Twink; } else { z=PRN;}
          
//          if((i == OUTPUT)&&(Title[i][j].MARK & NUM3)){   ///////////////////////////////////////
//          if (j==  OUTATT)
//          {  
//             
//              Int2Str(Title[i][j].Str, (Title[i][j].Value*26000), V_UNIT, 2, UNSIGN);
//              Print_Str(
//                Title[i][j].XPOS, Title[i][j].YPOS,
//                Title[i][j].Color[0],                  // Color fixed  
//                Twink, 
//                Title[i][j].Str                        // String for numerical
//              );
//           } 
//            
//          if (j==  DUTYPWM)
//          {  
//              
//              u8ToDec3(Title[i][j].Str, Title[i][j].Value);
//              Print_Str(
//                Title[i][j].XPOS, Title[i][j].YPOS,
//                Title[i][j].Color[0],                  // Color fixed  
//                Twink, 
//                Title[i][j].Str                        // String for numerical
//              );
//            }
//          }
            if((i == FILE)&&(Title[i][j].MARK & NUM3)){
              u8ToDec3(Title[i][1].Str, Title[i][1].Value);
              Print_Str(
                Title[i][1].XPOS, Title[i][1].YPOS,
                Title[i][1].Color[0],                  // Color fixed  
                Twink, 
                Title[i][1].Str                        // String for numerical
              );
            }
        }
      }
    }
  }
}
/*******************************************************************************
 Load_Attr:  加载硬件属性
*******************************************************************************/
void Load_Attr(void)
{
  u16 i;
  
  
  for(i=0; i<G_Attr[0].Yp_Max+1; ++i) 
    strcpy(&Vertical[i][0], Y_Attr[i].STR);
  Title[TRACK1][RANGE].Limit = G_Attr[0].Yp_Max;
  Title[TRACK2][RANGE].Limit = G_Attr[0].Yp_Max;
  Title[TRACK3][RANGE].Limit = 0;
  Title[TRACK4][RANGE].Limit = 0;
  
  for(i=0; i<G_Attr[0].Xp_Max+6; ++i) 
    strcpy(&BaseStr[i][0], X_Attr[i].STR);
  Title[T_BASE][BASE].Limit = G_Attr[0].Xp_Max;
  
}
/*******************************************************************************
 Update_Battery:  刷新电池电量指示
*******************************************************************************/
void Update_Battery(void)
{
  u16 Vb, Level;
  
  Vb = __Get(V_BATTERY);
  if (Cnt_InCharge>0) Cnt_InCharge--;
  if (Cnt_Charged>0) Cnt_Charged--;
  Cnt_Batt++;
  if (Cnt_Batt>4) Cnt_Batt=1;
  
  if (__Get(CHARGE)==1){
    Cnt_Charged=4;
     if (Cnt_InCharge==0)FlagInCharge=1;
 }
    else
  {  
    Cnt_InCharge=4;  
    if (Cnt_Charged==0)FlagInCharge=0;
   
  }
  
  
  if (FlagInCharge==1){        //batteria in carica
    Level = Cnt_Batt ;
  }
  else
  {
    Level =0;
    if(Vb > 3000 ) Level +=1;
    if(Vb > 3500 ) Level +=1;
    if(Vb > 3900 ) Level +=1;
    if(Vb > 4100 ) Level +=1; 
     
  }  
  if (Level>4) Level=4;
    Print_Str(
    Title[4][1].XPOS, Title[4][1].YPOS,
    Title[4][1].Color[Level], 
    PRN,
    (Title[4][1].Str + Level*10) 
  );    
}
/******************************** END OF FILE *********************************/
