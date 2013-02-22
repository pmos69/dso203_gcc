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

u16 Result_FPS;
u8 Cnt_InCharge;
u8 Cnt_Charged;
u8 Cnt_Batt;
u8 FlagInCharge;

char a_xy[8]="CH!A=!X";		
char b_xy[8]="CH!B=!Y";
char ax10_xy[7]="X10A=X";
char bx10_xy[7]="X10B=Y";
char a[8]="!CH(A)!";		
char b[8]="!CH(B)!";
char ax10[7]="x10(A)";
char bx10[7]="x10(B)";

char T_UNIT[12] ={'u','S', 0 ,'u','S', 0 ,'m','S', 0 ,'S',' ', 0 };
//char V_UNIT[12] ={'m','V', 0 ,'m','V', 0 ,'V',' ', 0 ,'k','V', 0 };
char V_UNIT[12] ={'m','V', 0 ,'m','V', 0 ,'V',' ', 0 ,'?',' ', 0 };     // ? = special"kv"character 
char F_UNIT[12] ={'H','z', 0 ,'K','@', 0 ,'M','@', 0 ,'G','@', 0 };	//Used at high sweep rates so display works right
char FM_UNIT[12] ={'M','@', 0 ,'M','@', 0 ,'G','@', 0 ,'T','@', 0 };
// char N_UNIT[12] ={ 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 };
char S_UNIT[12] ={'/','S','e','c', 0 ,'/','S','e','c', 0 , 0 , 0 };
char P_UNIT[12] ={'%',' ', 0 ,'%',' ', 0 ,'%',' ', 0 , 0 , 0 , 0 };
char F_UNITSUB[12]={'H','z', 0 ,'H','z', 0 ,'K','@', 0 ,'M','@', 0 };   //added to keep original FREQ display working right at low sweep rates 


char STATESTR[3][10] = {"!RUN!", "HOLD", "HOLD"};                  // Running state str

uc16 S_Inv[3]       = {(SCRN<<8)+TEXT2, (SCRN<<8)+X_POSI, (SCRN<<8)+NOTE1};         // Running state Color

char BATT_STR[5][10] = {"~``'", "~``}", "~`;}", "~;;}", "{;;}"};   // Battery Status Str
uc16 B_COLOR[5]     = {(NOTE1<<8)+SCRN, (SIDE <<8)+SCRN,
                       (TEXT2<<8)+SCRN, (TEXT2<<8)+SCRN,
                       (TEXT2<<8)+SCRN};                          // Battery Status Color

char CH_A_STR[4][10]  = {" !OFF! ", "!CH(A)!", "x10(A)"};
char CH_B_STR[4][10]  = {" !OFF! ", "!CH(B)!", "x10(B)"};
char CH_C_STR[2][10]  = {" !OFF! ", "!CH(C)!"};
char CH_D_STR[14][10] = {" !OFF! ", "!CH(D)!", "!(A+B)!", "!(A-B)!",  
                     "!(C&D)!", "!(C|D)!", "!REC_A!",  "!REC_B!",
						"!REC_C!",  "!REC_D!", "!FFT_A!", "!FFT_B!", "SPEC_A", "SPEC_B"};                      // Track source Str
char NO_RANGE[5]      = "";//" -- ";                       
char NO_DEF[5]        = "";//"--";                       

char  YCOUPLE[3][10]  = {"DC", "AC", "!-!"};                        // Track Couple Str                    
char  Vertical[15][10];                                             // Track Range Str
char  YPOSISTR[5]    = {"YPOS"};                                    // Track Position Str 
uc16 Y_INV[5]       = {(SCRN<<8)+TR_1, (SCRN<<8)+TR_2,
                       (SCRN<<8)+TR_3, (SCRN<<8)+TR_4,
                       (SCRN<<8)+VERNIE};                          // Track Color 1
uc16 Y_COLOR[5]     = {(TR_1<<8)+SCRN, (TR_2<<8)+SCRN,
                       (TR_3<<8)+SCRN, (TR_4<<8)+SCRN,
                       (VERNIE<<8)+SCRN};                          // Track Color 2

char MODESTR[10][10]  = {"TrOFF","!AUTO!", "NORML","NorCL","SINGL"," X_Y "};        // Sync Mode Str
char BaseStr[30][10];                                              // Time Base b Str
char  XPOSISTR[5]    = {"XPOS"};
uc16 XCOLOR[2]      = {(SCRN<<8)+X_POSI, (X_POSI<<8)+SCRN};        // Time Base Color

char FO_TYPE[6][10]  = {"!SINUS!",  "TRIANG",  "! SAW !",  "SQUARE", "! PWM !", " NONE "}; // Output Kind Str  "Kd=  %"

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
char  VT_STR[4]  =  "LEV";                                // Trigg Voltage Str
                          
char  VERNIE1[5]     = {"!V1!"};          // V1 Vernie Str
char  VERNIE2[5]     = {"!V2!"};          // V2 Vernie Str
char  VERNIE3[5]     = {"!T1!"};          // T1 Vernie Str
char  VERNIE4[5]     = {"!T2!"};          // T2 Vernie Str
uc16 V_INV[1]       = {(SCRN<<8)+VERNIE};
uc16 V_COLOR[1]     = {(VERNIE<<8)+SCRN};                         // Y Vernie Color
uc16 T_INV[1]       = {(SCRN<<8)+VERNIE};
uc16 T_COLOR[1]        = {(VERNIE<<8)+SCRN};                         // X Vernie Color
char  F_FUNC[2][10]  = {"Save File", "Load File"};                 // File Function Str

char  F_EXT[8][10]   = {".BMP ", ".DAT ", ".BUF ",".CSV ", " OK! ",
                         " ERR!",".HEX", ".BIN",};                  // File Ext Name Str
uc16 F_INV[1]       = {(SCRN<<8)+TEXT1};                          //  File Color
char  DELTA_V[2][10] = {"[V:", "[V:"};
char  DELTA_T[4]     = "[T:";
char  METER[12][5]   = {"Vbt", "FPS", "Vpp", "Vdc", "RMS",  "Max", 
                       "Min", "Frq", "Per", "Dut", "!TH!", "!TL!"};

u8   Detail[14];
char NumStr[12];
u8   Current = 0, Update = 1;

char BL_Str[5]="B.L", Vol_Str[5]="Vol";

char  x10[8][6]   = {"0.5V", " 1V ", " 2V ", " 5V ", "!10V!", "!20V!", "!50V!", "100V"};

menu Title[13][4]=   
{
  //     Item_String,   m_Color,   Limit, Mark, xPos, yPos, Value, Flag 
  {//============================ Title Track1 Group ===========================
    {(char*)CH_A_STR,(u16*)Y_INV,     3-1, CIRC,   35,  228,     1, UPDAT}, //  Track source   2
    {(char*)YCOUPLE, (u16*)Y_COLOR,   2-1, CIRC,   35,  216,     0, UPDAT}, //  Track Couple        
    {(char*)Vertical,(u16*)Y_COLOR,   9-1,    0,   51,  216,     5, UPDAT}, //  Track Range 
    {(char*)YPOSISTR,(u16*)Y_INV,     200,  FIX,    0,    0,   150, UPDAT}, //  Adj. Track Position 160
  },
  {//============================ Title Track2 Group ===========================
    {(char*)CH_B_STR,(u16*)Y_INV+1,   3-1, CIRC,   86,  228,     1, UPDAT}, //  Track source   2
    {(char*)YCOUPLE, (u16*)Y_COLOR+1, 2-1, CIRC,   86,  216,     1, UPDAT}, //  Track Couple        
    {(char*)Vertical,(u16*)Y_COLOR+1, 9-1,    0,  102,  216,     1, UPDAT}, //  Track Range
    {(char*)YPOSISTR,(u16*)Y_INV+1,   200,  FIX,    0,    0,   100, UPDAT}, //  Adj. Track Position
  },
  {//============================ Title Track3 Group ===========================
    {(char*)CH_C_STR,(u16*)Y_INV+2,   2-1, CIRC,  137,  228,     1, UPDAT}, //  Track source
    {(char*)NO_DEF , (u16*)Y_COLOR+2, 1-1, CIRC,  137,  216,     0, HID}, //  Track Couple        
    {(char*)NO_RANGE,(u16*)Y_COLOR+2, 1-1,    0,  153,  216,     0, HID}, //  Track Range
    {(char*)YPOSISTR,(u16*)Y_INV+2, 200-1,  FIX,    0,    0,    60, UPDAT}, //  Adj. Track Position
  },
  {//============================ Title Track4 Group ===========================
    {(char*)CH_D_STR,(u16*)Y_INV+3,  14-1, CIRC,  188,  228,     1, UPDAT}, //  Track source
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
    {(char*)FO_TYPE, (u16*)O_COLOR,   6-1, CIRC,  239,  228,     3, UPDAT}, //  Output Wave Kind    282 228       
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
    {(char*)VT_STR,  (u16*)Y_COLOR,     0,  NOT,  357,  216,     0,   HID}, //  Number position     
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
    {(char*)NumStr,  (u16*)V_COLOR,  10-1, NUM2,  342,  152,     5, UPDAT}, //  Class     was 11-1, causing display error
    {(char*)NumStr,  (u16*)V_INV,       0,  NOT,    0,    0,     0,   HID},    
    {(char*)NumStr,  (u16*)V_INV,       0,  NOT,    0,    0,     0,   HID},    
  },
};
      //.str
meter Meter[9] = //.track   .item                         .flag
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

void Display_Meter(void)                  // refresh measurements display
{
  u8  i;
  
  for(i=0; i<9; ++i){        
    if(Meter[i].Flag & UPDAT){            //-----the name of the measurements show the need to refresh
      Meter[i].Flag &= (!UPDAT & !BLINK);       // Clr Update & Blink flag
      Print_Str
        (Meter[i].XPOS1, Meter[i].YPOS,        // coordinates to be displayed
         Y_INV[Meter[i].Track],                // need to display color (belongs channel)
         PRN,                                  // print (no flash)
         Meter[i].Str +(Meter[i].Item * 5));   // need to display the project name
    } 
  }
  if((Current >= METER_0)&&(Current <= METER_8)){
    if(Blink){                            //----- the name of the display required flashing measurements
      i = Current - METER_0;
      Blink = 0;                               // Clr Blink Ctrl flag 
      Print_Str(
        Meter[i].XPOS1, Meter[i].YPOS,         // flashing coordinates
        Y_INV[Meter[i].Track],                 // flashing colors (belongs channel)
        Twink,                                 // flashing
        Meter[i].Str +(Meter[i].Item *5));     // flashing the project name
    }
  }
}

void Display_Value(u8 i)
{
  s32 Tmp = 0;
  s32 Tmp2= 0;
  u16 Kp;
  u32 k, n, m;
  u16 bag_max_buf = 4096;   // store sample buffer size

  Kp = _Kp1; // independent sampling mode
  
  k = _T_Range; m = 1;  n = 1;
  if(k < 9)  m = Power(10, (11-k)/3); //9 //11
  else       n = Power(10, (k- 9)/3);  //9
  k = X_Attr[(k%3)+9].SCALE;
  
  bag_max_buf = get_bag_max_buf();
		
  switch (Meter[i].Item){  
  case VBT://--------------- calculation and display of battery voltage ---------------
    Int2Str(NumStr, __Get(V_BATTERY)*1000, V_UNIT, 3, SIGN);
    break;
  case FPS://--------------- calculates and displays the frame count ---------------
    Int2Str(NumStr, Result_FPS & 0x7F, S_UNIT, 2, STD);
    break;  
  case VPP:
    if(Meter[i].Track == TRACK1){
		if  (_1_source == HIDE) Tmp=0;
		else {
			Tmp=a_Max+Ka1[_A_Range]-_1_posi;				
	            Tmp = ((Ka2[_A_Range]*Tmp)+512)/1024;
			Tmp2=a_Min+Ka1[_A_Range]-_1_posi;
	            Tmp2 = ((Ka2[_A_Range]*Tmp2)+512)/1024; 
			Tmp=Tmp-Tmp2;
			if(Tmp <= 2) Tmp = 0;						// round of precision error - was 4
			Tmp *= Y_Attr[_A_Range].SCALE;
			if  (_1_source == CH_X10) Tmp=Tmp*10;
		}
    }
    if(Meter[i].Track == TRACK2){
		if  (_2_source == HIDE) Tmp=0;
		else {
			Tmp=b_Max+Kb1[_B_Range]-_2_posi;
	            Tmp = ((Kb2[_B_Range]*Tmp)+512)/1024; 
			Tmp2=b_Min+Kb1[_B_Range]-_2_posi;
	            Tmp2 = ((Kb2[_B_Range]*Tmp2)+512)/1024; 
			Tmp=Tmp-Tmp2;


			if(Tmp <= 2) Tmp = 0;						// round of precision error - was 4
			Tmp *= Y_Attr[_B_Range].SCALE;
			if  (_2_source == CH_X10) Tmp=Tmp*10;
		}
    }
	
    Int2Str(NumStr, Tmp, V_UNIT, 3, SIGN);
    break;
  case VDC:
    if(Meter[i].Track == TRACK1){
		if  (_1_source == HIDE) Tmp=0;
		else {
			Tmp=((a_Avg/bag_max_buf)+Ka1[_A_Range]-_1_posi);
	            Tmp = ((Ka2[_A_Range]*Tmp)+512)/1024;   // Add signal level correction factor based on signal zero level rather than bottom of screen, prevents offsets from corupting value 
			if((Tmp >= -2)&&(Tmp <= 2)) Tmp = 0;		// round of precision error
			Tmp *= Y_Attr[_A_Range].SCALE;
			if  (_1_source == CH_X10) Tmp=Tmp*10;
		}
    }
    if(Meter[i].Track == TRACK2){
		if  (_2_source == HIDE) Tmp=0;
		else {
			Tmp=((b_Avg/bag_max_buf)+Kb1[_B_Range]-_2_posi);
	            Tmp = ((Kb2[_B_Range]*Tmp)+512)/1024; // use bag_max_buf as average divider
			if((Tmp >= -2)&&(Tmp <= 2)) Tmp = 0;		// round of precision error
			Tmp *= Y_Attr[_B_Range].SCALE;
			if  (_2_source == CH_X10) Tmp=Tmp*10;
		}
    }
    Int2Str(NumStr, Tmp, V_UNIT, 3, SIGN);
    break;
  case RMS:
    if(Meter[i].Track == TRACK1){
		if  (_1_source == HIDE) Tmp=0;
		else {
			Tmp = (Ka2[_A_Range]*Int_sqrt(a_Ssq/bag_max_buf)+ 512)/1024;     // adding Ka1 here causes error in calc, already factored in in Process...
			if(Tmp <= 2) Tmp = 0;						// round of precision error				
			Tmp *= Y_Attr[_A_Range].SCALE;
			if  (_1_source == CH_X10) Tmp=Tmp*10;
		}
    }
    if(Meter[i].Track == TRACK2){
		if  (_2_source == HIDE) Tmp=0;
		else {
			Tmp = (Kb2[_B_Range]*Int_sqrt(b_Ssq/bag_max_buf)+ 512)/1024; 			
			if(Tmp <= 2) Tmp = 0;						// round of precision error				
			Tmp *= Y_Attr[_B_Range].SCALE;
			if  (_2_source == CH_X10) Tmp=Tmp*10;
		}
    }
    Int2Str(NumStr, Tmp, V_UNIT, 3, SIGN); //unsign
    break;
  case MAX:
    if(Meter[i].Track == TRACK1){
		if  (_1_source == HIDE) Tmp=0;
		else {
			Tmp=a_Max+Ka1[_A_Range]-_1_posi;
	            Tmp = ((Ka2[_A_Range]*Tmp)+512)/1024; // use bag_max_buf as average divider
			if((Tmp >= -2)&&(Tmp <= 2)) Tmp = 0;
			Tmp *= Y_Attr[_A_Range].SCALE;
			if  (_1_source == CH_X10) Tmp=Tmp*10;
		}
	}
    if(Meter[i].Track == TRACK2){
		if  (_2_source == HIDE) Tmp=0;
		else {
			Tmp=b_Max+Kb1[_B_Range]-_2_posi;
	            Tmp = ((Kb2[_B_Range]*Tmp)+512)/1024; // use bag_max_buf as average divider
			if((Tmp >= -2)&&(Tmp <= 2)) Tmp = 0;
			Tmp *= Y_Attr[_B_Range].SCALE;
			if  (_2_source == CH_X10) Tmp=Tmp*10;
		}
    }
	
    Int2Str(NumStr, Tmp, V_UNIT, 3, SIGN);
    break;
  case MIN:
    if(Meter[i].Track == TRACK1){
		if  (_1_source == HIDE) Tmp=0;
		else {
			Tmp=a_Min+Ka1[_A_Range]-_1_posi;
	            Tmp = ((Ka2[_A_Range]*Tmp)+512)/1024; // use bag_max_buf as average divider
			if((Tmp >= -2)&&(Tmp <= 2)) Tmp = 0;
			Tmp *= Y_Attr[_A_Range].SCALE;
			if  (_1_source == CH_X10) Tmp=Tmp*10;
		}
    }
    if(Meter[i].Track == TRACK2){
		if  (_2_source == HIDE) Tmp=0;
		else {
			Tmp=b_Min+Kb1[_B_Range]-_2_posi;
	            Tmp = ((Kb2[_B_Range]*Tmp)+512)/1024; // use bag_max_buf as average divider
			if((Tmp >= -2)&&(Tmp <= 2)) Tmp = 0;
			Tmp *= Y_Attr[_B_Range].SCALE;
			if  (_2_source == CH_X10) Tmp=Tmp*10;
		}
    }
    Int2Str(NumStr, Tmp, V_UNIT, 3, SIGN);
    break;
  case FRQ:
    if((Meter[i].Track == TRACK1)&&(_1_source != HIDE))
      Tmp = 2000*((500000 * TaN)/TaS);
    if((Meter[i].Track == TRACK2)&&(_2_source != HIDE))
      Tmp = 2000*((500000 * TbN)/TbS);
    if((Meter[i].Track == TRACK3)&&(_3_source == CH_C))
      Tmp = 2000*((500000 * TcN)/TcS);
    if((Meter[i].Track == TRACK4)&&((_4_source == CH_D)||(_4_source == C_and_D)||(_4_source == C_or_D)))
      Tmp = 2000*((500000 * TdN)/TdS);
    if(n < 10)  Int2Str(NumStr, ((250*(Tmp/(k/60)))/m)/15, F_UNITSUB , 4, UNSIGN);     //added display string so works correctly
    else        Int2Str(NumStr, Kp*(n*(Tmp/(k/60))/240)/256, F_UNIT, 4, UNSIGN);       //returned to original code, modified did not work > ~2Mhz
    break;  
  case CIR:
    if((Meter[i].Track == TRACK1)&&(_1_source != HIDE))
      Tmp = (k *TaS)/TaN;
    if((Meter[i].Track == TRACK2)&&(_2_source != HIDE))
      Tmp = (k *TbS)/TbN;
    if((Meter[i].Track == TRACK3)&&(_3_source == CH_C))
      Tmp = (k *TcS)/TcN;
    if((Meter[i].Track == TRACK4)&&((_4_source == CH_D)||(_4_source == C_and_D)||(_4_source == C_or_D)))
      Tmp = (k *TdS)/TdN;
    if(Tmp <= 0x7FFFFFFF/m) Tmp = 1024*((m*Tmp)/Kp)/n;
    else                    Tmp = 0x80000000;
    Int2Str(NumStr, Tmp, T_UNIT, 4, UNSIGN);
    break;
  case DUT:
    if((Meter[i].Track == TRACK1)&&(_1_source != HIDE))
      Tmp = (100000*PaS)/TaS;
    if((Meter[i].Track == TRACK2)&&(_2_source != HIDE))
      Tmp = (100000*PbS)/TbS;
    if((Meter[i].Track == TRACK3)&&(_3_source == CH_C))
      Tmp = (100000*PcS)/TcS;
    if((Meter[i].Track == TRACK4)&&((_4_source == CH_D)||(_4_source == C_and_D)||(_4_source == C_or_D)))
      Tmp = (100000*PdS)/TdS;
    Int2Str(NumStr, Tmp, P_UNIT, 4, UNSIGN);
    break;
  case TL:
    if((Meter[i].Track == TRACK1)&&(_1_source != HIDE))
      Tmp = (k*TaS)/TaN - (k*PaS)/TaN;
    if((Meter[i].Track == TRACK2)&&(_2_source != HIDE))
      Tmp = (k*TbS)/TbN - (k*PbS)/TbN;
    if((Meter[i].Track == TRACK3)&&(_3_source == CH_C))
      Tmp = (k*TcS)/TcN - (k*PcS)/TcN;
    if((Meter[i].Track == TRACK4)&&((_4_source == CH_D)||(_4_source == C_and_D)||(_4_source == C_or_D)))
      Tmp = (k*TdS)/TdN - (k*PdS)/TdN;
    if(Tmp <= 0x7FFFFFFF/m) Tmp = 1024*((m*Tmp)/Kp)/n;
    else                    Tmp = 0x80000000;
    Int2Str(NumStr, Tmp, T_UNIT, 4, UNSIGN);
    break;
  case TH:
    if((Meter[i].Track == TRACK1)&&(_1_source != HIDE))
      Tmp = (k*PaS)/TaN;
    if((Meter[i].Track == TRACK2)&&(_2_source != HIDE))
      Tmp = (k*PbS)/TbN;
    if((Meter[i].Track == TRACK3)&&(_3_source == CH_C))
      Tmp = (k*PcS)/TcN;
    if((Meter[i].Track == TRACK4)&&((_4_source == CH_D)||(_4_source == C_and_D)||(_4_source == C_or_D)))
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
    NumStr);                              // display the measured values
}

void Display_Title(void)			
{
  u16  i, j;
  s32  k, n, m;
  u8 z;
  char *p;

if (_Mode==X_Y){					//replace menu title for chA and chB to show X_Y mode
  p=(char *)CH_A_STR;
  p+=10;
  for (i=0;i<7;i++)*p++=a_xy[i];
  p+=3;
  for (i=0;i<6;i++)*p++=ax10_xy[i];
  p=(char *)CH_B_STR;
  p+=10;
  for (i=0;i<7;i++)*p++=b_xy[i];
  p+=3;
  for (i=0;i<6;i++)*p++=bx10_xy[i];
 }else{						//show regular title
  p=(char *)CH_A_STR;
  p+=10;
  for (i=0;i<7;i++)*p++=a[i];
  p+=3;
  for (i=0;i<6;i++)*p++=ax10[i];
  p=(char *)CH_B_STR;
  p+=10;
  for (i=0;i<7;i++)*p++=b[i];
  p+=3;
  for (i=0;i<6;i++)*p++=bx10[i];
}

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
        Int2Str(NumStr, (Title[OUTPUT][OUTATT].Value*28000), V_UNIT, 2, UNSIGN);     //26000 
        Print_Str(Title[OUTPUT][OUTATT].XPOS, Title[OUTPUT][OUTATT].YPOS,
                  (Title[OUTPUT][OUTATT].Color[0]),z,NumStr);
      }

      if (CalFlag==0){
        Print_Str(137,216,0x0405,PRN,"Un");
      }else{
        Print_Str(137,216,0x0405,PRN,"!C!");
      }
	  
  for(i = TRACK1; i <= VOLUME; ++i){
    for(j = 0; j < 4; ++j){
      if(Title[i][j].Flag & UPDAT){   // need to refresh the Item
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
			  // battery and volume meter titles - not flashing
             );
          } else if(Title[i][j].MARK & NUM3){          // ---- Under NUM3 mode
            if(i == V_VERNIE){
              if  (Title[_Meas_V_Track][SOURCE].Value ==2) 
                  {Int2Str(NumStr,(_V1_Vernie-_V2_Vernie)*_Meas_V_Scale*10, V_UNIT, 3, SIGN);}
                else
                  {Int2Str(NumStr,(_V1_Vernie-_V2_Vernie)*_Meas_V_Scale, V_UNIT, 3, SIGN);
                }
              Print_Str(
                Title[i][j].XPOS, Title[i][j].YPOS,
                Title[i][j].Color[_Meas_V_Track],      // Color fixed  
                PRN, 
                NumStr                                // String for numerical
				//Delta V value
              );
            }
            if(i == T_VERNIE){				
              m = 1;  n = 1;
              k = _T_Range;
              if(k < 9)  m = Power(10, (11-k)/3);
              else       n = Power(10, (k- 9)/3);
              k = X_Attr[(k%3)+9].SCALE *(_T2 - _T1);
		  if (_T_Range<3)k/=1000;				//special case for very long interval so won't overload
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
            if(i==FILE){
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
            if(i == BK_LIGHT){                        // backlight percentage
              if(Title[i][j].Value == 9){
                Int2Str(NumStr, 100, P_UNIT, 3, STD);
                } else Int2Str(NumStr, 10*(Title[i][j].Value+1), P_UNIT, 2, STD);
            } else {                                  // volume percentage
              if(Title[i][j].Value == 9){							//was 10, changed so volume ind works properly
                Int2Str(NumStr, 100, P_UNIT, 3, STD);
              } else Int2Str(NumStr, 10*(Title[i][j].Value+1), P_UNIT, 2, STD);
            }
            Print_Str(
              Title[i][j].XPOS, Title[i][j].YPOS,
              Title[i][j].Color[0],                  // Color fixed  
              PRN, 
              NumStr                        // String for numerical
			  //Battery and volume values
            );
          } else if(Title[i][j].MARK != NOT){
          
            if(i == V_VERNIE){
              Print_Str(
                Title[i][j].XPOS, Title[i][j].YPOS,
                Title[i][j].Color[_Meas_V_Track],         // Color variable 
                PRN, 
                Title[i][j].Str +(Title[i][j].Value * 10) // String variable
				//Delta V meter title
              ); 
            } else {
				if (i<=1 && j==2 && Title[i][0].Value==2){
				
				Print_Str(
					Title[i][j].XPOS, Title[i][j].YPOS,
					Title[i][j].Color[0],                    // Color fixed 
					PRN, 
					x10[Title[i][j].Value]// String variable
					//ChA or ChB x10 value - not flasing
				  );
				  
				} else {
				  Print_Str(
					Title[i][j].XPOS, Title[i][j].YPOS,
					Title[i][j].Color[0],                    // Color fixed 
					PRN, 
					Title[i][j].Str +(Title[i][j].Value *10)// String variable
					//Upper menu titles and values - not flashing
				  );

				}
            }
          } else if(i == T_VERNIE){
            Print_Str(
              Title[i][j].XPOS, Title[i][j].YPOS,
              Title[i][j].Color[0],                    // Color fixed 
              PRN, 
              Title[i][j].Str                          // String variable
            ); 
          }
        }
      } else if((Current == i)&&(Detail[i] == j)&&(Blink)){ // current cursor position Item
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
        if(i == V_VERNIE)
            Print_Str(
                Title[i][j].XPOS, Title[i][j].YPOS,
                Title[i][j].Color[_Meas_V_Track],        // Color variable
                Twink, 
                Title[i][j].Str +(Title[i][j].Value * 10)// String variable
				// Delta V meter title - flashing
            ); 
        else {
			if (i<=1 && j==2 && Title[i][0].Value==2)
				Print_Str(
					Title[i][j].XPOS, Title[i][j].YPOS,
					Title[i][j].Color[0],                    // Color fixed 
					Twink, 
					x10[Title[i][j].Value]// String variable
					//ChA or ChB x10 value - flashing
				); 
			else
				Print_Str(
					Title[i][j].XPOS, Title[i][j].YPOS,
					Title[i][j].Color[0],                    // Color fixed 
					Twink, 
					Title[i][j].Str +(Title[i][j].Value * 10)// String variable
					//Upper menu titles and values - flashing
				); 
			}
                  }
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
 Load_Attr:   load the hardware properties
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
 Update_Battery:  refresh the battery indicator
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
    Level =0;	//3000,3500,3900,4100 (or)    3200,3600,3900,4100 (mod)     3400,3600,3800,3990 >Batt seemed to die off very fast after going from 1/2 to 1/4...
    //if(Vb > 3000 ) Level +=1;
    if(Vb > 3400 ) Level +=1;
    //if(Vb > 3500 ) Level +=1;
    if(Vb > 3600 ) Level +=1;
    if(Vb > 3800 ) Level +=1;
    if(Vb > 3990 ) Level +=1; 
     
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
