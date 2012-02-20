/********************* (C) COPYRIGHT 2010 e-Design Co.,Ltd. ********************
 File Name : Main.c  
 Version   : DS203_APP Ver 2.5x                                  Author : bure
*******************************************************************************/
#include "Interrupt.h"
#include "Function.h"
#include "Calibrat.h"
#include "Process.h"
#include "Ident.h"
#include "Draw.h"
#include "BIOS.h"
#include "Menu.h"
#include "File.h"

/*******************************************************************************
°æ±¾ÐÞ¸ÄËµÃ÷
APP V2.30: ´Ó¸Ã°æ±¾Æð²»ÔÙ¼æÈÝPCB_V2.6ÒÔÏÂ°æ±¾µÄÖ÷°å
           ´Ó¸Ã°æ±¾Æð²»ÔÙ¼æÈÝSYS_V1.31ÒÔÏÂ°æ±¾
           ÐÝÃß¶¨Ê±ÐÞ¸ÄÎª600Ãë(Main.c)
           ÐÞ¸Ä¼°Ìí¼ÓÁËÐÂµÄSYS¿âº¯Êý(BIOS.s)
           ÐÞ¸ÄÁË¿ª»úÐÅÏ¢ÏÔÊ¾³ÌÐò(Main.c)
APP V2.31: Ôö¼ÓÁË¿ª»úÊ¶±ðFPGA¼ÓÔØÅäÖÃÍê³ÉÓë·ñÅÐ±ð(Main.c)
           Ôö¼ÓÁËLicenceÈ¨ÏÞ¹ÜÀí¹¦ÄÜµÄDemo³ÌÐò·¶Àý(Ident.c,Main.c)
           ÐÞ¸ÄÁËÄ£ÄâÍ¨µÀÐ£Õý¹¦ÄÜµÄ½øÈëºÍÍË³öÏà¹Ø²Ù×÷(calibrat.c)
           Ôö¼ÓÁË144MHz½»Ìæ²ÉÑùÄ£Ê½ÏÂµÄÏà¹Ø¹¦ÄÜ(Process.c)
APP V2.32  ´Ó¸Ã°æ±¾Æð¿É²¢ÐÐÊ¹ÓÃIAR 4.42Óë5.0°æ±¾
           Ô´³ÌÐòÃ»¸Ä¶¯£¬Ôö¼ÓÁËÎÄ¼þ¼Ð¡¡IAR_V5_Prpject
APP V2.33  ÐÞ¸ÄÁËÉ¨ÃèÊ±»ù<1uSÊ±£¬ÏÔÊ¾Ë¢ÐÂµÄBUG(Process.c)
           ÐÞ¸ÄÁËÔÚÐ£×¼×´Ì¬ÏÂ£¬²Ù×÷ÌáÊ¾ÐÅÏ¢µÄBUG(Calibrat.c)
APP V2.34  ¸ÄÎª°´Í¨µÀµ¥¶ÀÐ£×¼(Calibrat.c & Main.c)
           ÐÞ¸ÄÁËÐ£×¼ÏîÑ¡ÔñµÄ²Ù×÷·½Ê½(Calibrat.c)
APP V2.35  ÐÞ¸ÄÁËÐ£×¼¹ý³ÌÖÐµÄBUG(Calibrat.c)
           ÐÞ¸ÄÁËÉ¨ÃèÊ±»ù<5uSÊ±£¬ÔÝÍ£²»ÁËµÄBUG(Process.c)
           ÓÅ»¯ÁËÏÔÊ¾Êý¾Ý´¦Àí³ÌÐò(Process.c)
           Ôö¼ÓÁËÄ£ÄâÍ¨µÀ×Ô¶¯ÁãµãÆ½ºâ¹¦ÄÜ(Main.c,Process.c,Calibrat.c)
APP V2.36  ²¿·ÖÐ£×¼²Ù×÷¸ÄÎª×Ô¶¯Ä£Ê½(Calibrat.c,Process.c,Function.c)
           ÐÞ¸ÄÁË¿ª»ú¼ÓÔØ¹¤×÷²ÎÊýµÄ·½Ê½(Main.c)
APP V2.37  ½øÒ»²½ÍêÉÆºÍÓÅ»¯ÁËÏÔÊ¾Êý¾Ý´¦Àí³ÌÐò(Process.c)
           ÐÞ¸ÄÁË32Î»ÓÐ·ûºÅ¼°ÎÞ·ûºÅÕûÊý×ª»»³ÌÐòËÄÉáÎåÈëµÄBUG(Function.c)
           Ôö¼ÓÁËÊ±¼äÆµÂÊÂö¿íÕ¼¿Õ±È²âÁ¿¹¦ÄÜ(Process.c, Menu.c)
APP V2.40  Ôö¼ÓÁËÐ´UÅÌ´´½¨ÎÄ¼þÃû¹¦ÄÜ(Main.c, Flies.c, dosfs.c)
           ÐÞ¸Ä´æÅÌÊ±ÏÔÊ¾ÎÄ¼þÐòºÅBUG(Menu.c) 
APP V2.41  Ôö¼ÓÁËÎÄ¼þ¸ñÊ½Îª.BUFµÄ¶Á/Ð´²ÉÑù»º³åÇøÊý¾ÝÎÄ¼þ(Main.c,Flies.c,Menu.c)
           Ôö¼ÓÁËÎÄ¼þ¸ñÊ½Îª.CSVµÄµ¼³ö²ÉÑù»º³åÇøÊý¾ÝÎÄ¼þ(Main.c,Flies.c,Menu.c)
APP V2.42  Îª½ÚÊ¡¿Õ¼ä½«ÎÄ¼þÏµÍ³×ªÒÆµ½SYS_V1.40Ä£¿éÉÏ(ASM.s, Flies.c, dosfs.c)
           ¸ÄÎªÓÃ"SerialNo.WPT"µÄÎÄ¼þÐÎÊ½±£´æ¹¤×÷²ÎÊý±í(Flies.c)
           ×¢£ºAPP V2.42ÒÔÉÏ°æ±¾±ØÐëÓëSYS V1.40ÒÔÉÏ°æ±¾Ò»ÆðÅäºÏÊ¹ÓÃ
APP V2.43  ÐÞ¸ÄÁËÄ£ÄâÍ¨µÀµµÎ»µ÷ÕûÊ±µÄBUG(Main.c)
APP V2.44  ÐÞ¸ÄÁËÐ£×¼²Ù×÷Ê±±£´æ²ÎÊýµÄBUG(Calibrat.c)
           Ôö¼ÓÁË¿ª»ú¼ÓÔØ²ÎÊý³É¹¦Óë·ñÌáÊ¾(Main.c)
APP V2.45  ÐÞ¸ÄÁË¶ÁÐ´BUFÎÄ¼þÊ±»Ö¸´ÏÔÊ¾²Ëµ¥ÖÐ¸÷¸ö¶ÔÓ¦ÏîÊ±µÄBUG(Files.c)
           É¾³ýÁË¶ÁBUFÎÄ¼þÊ±µÄ²âÊÔÐÅÏ¢·´À¡ÏÔÊ¾(Main.c)
APP V2.50  ÖØÐ´ÁË»ùÓÚÐÂFAT12ÎÄ¼þÏµÍ³µÄÎÄ¼þ¶ÁÐ´³ÌÐò(Files.c, ASM.s)
           ÐÞ¸ÄÁËTH,TL²âÁ¿ÏÔÊ¾µÄBUG(Menu.c)
           ÓÅ»¯ÁË´øÁ¿¸ÙÊýÖµÏÔÊ¾Ïà¹Øº¯Êý(Menu.c,Function.c,Calibrat.c)
           ÐÞ¸ÄÁËÂö¿í´¥·¢³ÌÐòµÄBUG(Process.c)
APP V2.51  ÐÞ¸ÄÁËVmin,Vmax,Vpp¼ÆÁ¿µÄBUG(Process.c)
*******************************************************************************/

#define APP_VERSION       "     DS203 Mini DSO Ver 2.51 SMTech 1.1"
//#define APP_VERSION       "     DS203 Mini DSO APP Ver 2.51      "

uc8 PROJECT_STR[20] = "Demo PROG. Ver 1.00";
u8 FlagFrameMode;
u8 OldCurrent;
u8 OldDet;
u8 TrgAuto;

/*******************************************************************************
  main : Main routine.
*******************************************************************************/
int main(void)
{ 
  u32 k;
  s32 i;
 // u32 Licence;
  u16 Count_FPS, Second;//,Offset, Result 
  u8  UpdateMeter, CounterUpdate  ;
  u8 j;
 u16 TmpVT;
u16 Vt1Old;
u16 Vt2Old;
//  u8 N[20];
//  u8 T_Unit[15]={'u','S','u','S','m','S','S'};
 
NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0C000);   // For Application #1
//NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x14000);   // For Application #2
//NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x1C000);   // For Application #3
//NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x24000);   // For Application #4
  
//Note: ÓÃ IAR_V4.x ±àÒëÊ±£¬±ä¸ü App#n »¹ÒªÍ¬Ê±ÐÞ¸Ä lnkarm.xcl ÎÄ¼þÖÐµÄ¶ÔÓ¦Ïî 
//      ÓÃ IAR_V5.x ±àÒëÊ±£¬±ä¸ü App#n »¹ÒªÍ¬Ê±ÐÞ¸Ä xxxxxx.icf ÎÄ¼þÖÐµÄ¶ÔÓ¦Ïî 
  
__USB_Init();

if(__Get(FPGA_OK)== 0){
  
  
  __Display_Str(6*8, 30, YEL, PRN, "      FPGA configuration error       ");
  while (1){};
}

__Display_Str(48, 50, WHT, PRN, APP_VERSION);
Y_Attr = (Y_attr*)__Get(VERTICAL);     
X_Attr = (X_attr*)__Get(HORIZONTAL);
G_Attr = (G_attr*)__Get(GLOBAL);
T_Attr = (T_attr*)__Get(TRIGGER);
Load_Attr();                                 // ¸³ÖµY_AttrµÈ
i = Load_Param(); 
if(i == 0)  // ¶ÁÈ¡Ô¤Éè¿ª»ú²ÎÊý
__Display_Str(6*8, 30, GRN, PRN, "     Reload parameter form disk       ");
else       
__Display_Str(6*8, 30, YEL, PRN, "     Parameter record not found       ");  
//  i = Load_Param(); // ¶ÁÈ¡Ô¤Éè¿ª»ú²ÎÊý 

/*--------------------------- LICENCE_CTRL_DEMO --------------------------------
Offset = Seek_Proj(PROJECT_ID);
if(Offset >= 2048){                          // Project ID not found
Offset = Seek_Blank();
if(Offset == 2048){  
__Display_Str(6*8, 50, GRN, PRN,   "         Licence record full         ");
while (1){};
} else {
Result  = Add_Proj(PROJECT_ID, Offset);  // Set project ID
Result &= Add_Cnt(DEMOCNT, Offset);      // Set max demo run counter      
Result &= Add_Str((u32)PROJECT_STR, Offset); 
if(Result != 1){                                
__Display_Str(6*8,50,GRN, PRN,   "       Project ID writen error       ");
Delayms(500);
}
}
}
Licence = Get_Lic(Offset);                   // Get project licence record
if(__Ident(DEVELOPER_ID, PROJECT_ID, Licence)!= 1){
__Display_Str(6*8, 50, GRN, PRN,   "Please input project licence:00000000");
Licence = Input_Lic((6+29)*8, 50);         // Input Licence
if(__Ident(DEVELOPER_ID, PROJECT_ID, Licence)!=1)  Result = 0;
else{                                      // Licence correct
Result  = Add_Lic(Licence, Offset);            
if(Result == 1)                                
__Display_Str(6*8,50,GRN, PRN, "          Licence writen ok          ");
else  
__Display_Str(6*8,50,GRN, PRN, "         Licence writen error        ");
Delayms(500);
}
if(Result != 1){
__Display_Str(6*8, 30, GRN, PRN, "      Push any key to next step      ");
while(Key_Buffer == 0){};
}
}
//  --------------------------------------------------------------------------*/

Beep_mS = 500;
Balance();
App_init();
Key_Buffer=0;
TrgAuto=1;  //Abilita il trigger automatico all'accensione
      


//--------------------------------- Ö÷Ñ­»· ------------------------------------- 
while (1){
  
  
  if(PD_Cnt == 0){
    __Set(BACKLIGHT, 0);     // ¹Ø±Õ±³¹â
    __Set(STANDBY, EN);      // ½øÈëÊ¡µç×´Ì¬  
  } else {
    Synchro();                              // Í¬²½ÏÔÊ¾¸÷¸ö¹ì¼£²¨ÐÎÊý¾Ý
    Count_FPS++;
    if(Second != Sec_Cnt) {
      Second = Sec_Cnt;
      Result_FPS = Count_FPS;
      Count_FPS = 0;
      Update_Battery();
      for(i=0; i<9; ++i)  Display_Value(i); // Ã¿ÃëË¢ÐÂ²âÁ¿Öµ        
    }
    
    Display_Meter();
    Display_Title();
    if(Update){                             // ´¦Àí°´¼üºóÐèÒªË¢ÐÂµÄÏîÄ¿
      if (TrgAuto==0)  Print_Str(365,  216, 0x0405, PRN, "!Man!");
      if (TrgAuto==1)  Print_Str(365,  216, 0x0405, INV, "Auto");
      Update_Range();
      Update_Base();
      Update_Output();
      Update_Trig();
      Update_Mark();
      __Set(BACKLIGHT, 10*(Title[BK_LIGHT][CLASS].Value+1));
      if(Current != FILE) Update_View_Area();
      _D_V_Source.Flag |= UPDAT;            // Updat delta V 
      _Delta_V.Flag    |= UPDAT; 
      _Delta_T.Flag    |= UPDAT;            // Updat delta T 
      Update = 0;                           // Update finish
    } 
    Blink = 0;
  }
  if((_State.Value == HOLD)&&((__Get(FIFO_FULL)!= 0)||(__Get(FIFO_START)== 0))){
    //    if((_State.Value == HOLD)&&(Twink== 1)){
    _State.Value = 2; 
    _State.Flag |= UPDAT;
  }
  
  
  
    if  (TrgAuto==1){
 
    if ((Current == TRIGG) && (_Det==2)){ Title[Current][_Det].Flag |= UPDAT;_Det=0;}
    if (Title[TRIGG][SOURCE].Value == TRACK1){
      TmpVT=a_Min+((a_Max-a_Min)/2);
      if ((TmpVT>Vt1Old +5) || (TmpVT<Vt1Old -5)){ _Vt1=TmpVT;Update=1;Vt1Old=_Vt1;} 
      
    }
    if (Title[TRIGG][SOURCE].Value == TRACK2){
      TmpVT=b_Min+((b_Max-b_Min)/2);
      if ((TmpVT>Vt2Old +5) || (TmpVT<Vt2Old -5)){ _Vt2=TmpVT;Update=1;Vt2Old=TmpVT;} 
      
    }
  }

  k= (Title[T_BASE][1].Value);                                                // Tempo Base
  
  if (FlagFrameMode==1){
    Title[T_BASE][XPOSI].Value=0;                        /////
    if ((Current== T_BASE) && (_Det==XPOSI)) _Det=MODE;  //////
    if (k<6)  {FrameMode=1;}  // 17 e 18 
    if ((k>5) && (k<11)) {FrameMode=2;}  // 17 e 18 3
    if (k>10)  {FrameMode=0;}  // 17 e 18 2
  }
  else
  {
    FrameMode=0;
  }
  //////**********************       
  
  
  
  
  
  
  
  
  
  
  //------------------------------ °´¼ü´¦ÀíÑ­»· ---------------------------------- 
  if(Key_Buffer) { 
    
    if(PD_Cnt == 0)  App_init();          // ÍË³öÊ¡µç×´Ì¬
    PD_Cnt = 600;                         // 600Ãë
    //--------------------------------------------------------------------------------------------------------------  
    if(Key_Buffer == KEY1){                                                   // ===--- TASTO 1 PREMUTO ---===
      _State.Value = (_State.Value == 0)? 1 : 0;                              // "RUN/HOLD" 
      _State.Flag |= UPDAT;                                                   // 
      if((Current == FILE)&&(_Curr[2].Value == BUF)) reset_parameter();       //
      if(_Mode == SGL){                                                       //
        for(i=0; i<4*X_SIZE; ++i)  TrackBuff[i] = 0;                          //
        __Set(FIFO_CLR, W_PTR);                                               // FIFO
      }
    }
    //--------------------------------------------------------------------------------------------------------------
    if(Key_Buffer== KEY2){                                                    // ===--- TASTO 2 PREMUTO ---===
      Delay_Cnt = 3000;                                                       // Carica il temporizzatore del tasto permuto
      while (Delay_Cnt > 0){                                                  // Se il tempo non è arrivato a 0
        if((__Get(KEY_STATUS)& KEY2_STATUS)!=0){ 
         if (TrgAuto==0) { TrgAuto=1;break;}
         if (TrgAuto==1) { TrgAuto=0; Title[Current][_Det].Flag |= UPDAT;_Det=0;(Current = TRIGG); (_Det=2);break;}
         
        }  
        
      }
      if((Delay_Cnt == 0) && ((Current == TRACK1)||(Current == TRACK2)) )Calibrat(Current);             // richiama calibrazione canale
    }
    //--------------------------------------------------------------------------------------------------------------     
    if(Key_Buffer== KEY3){                                                    // ===--- TASTO 3 PREMUTO --====
      Delay_Cnt = 2000;                                                       // Carica il temporizzatore del tasto permuto
      while (Delay_Cnt > 0){                                                  // Se il tempo non è arrivato a 0
        if((__Get(KEY_STATUS)& KEY3_STATUS)!=0){ 
          Title[Current][_Det].Flag |= UPDAT;                                 // aggiorna titolo corrente per lasciare lo sfondo acceso
          if ((Current != T_VERNIE) && (Current != V_VERNIE)){                // se il titolo non è gia dei cursori
            OldCurrent=Current;                                               // memorizza il titolo corrente
            OldDet=_Det;                                                      // memorizza il dettaglio corrente
            Current = V_VERNIE;                                               // vai sul titolo del cursore V
            _Det=V1 ;                                                         // imposta il detaglio V1
            break;
          }
          if ((Current==V_VERNIE) && (_Det==V1)){                             // se il corrente è V con dettaglio V1            
            _Det=V2;                                                          // imposta il dettaglio a V2 
            break;
          }
          if ((Current==V_VERNIE) && (_Det==V2)){                             // se il corrente è V con dettaglio V2
            Current=T_VERNIE;                                                 // imposta il titolo a T 
            _Det=T1;                                                          // imposta il dettaglio a T1  
            break;
          }
          if ((Current==T_VERNIE) && (_Det==T1)){                             // se il corrente è T con dettaglio T1
            _Det=T2;                                                          // imposta il dettaglio a T2 
            break;
          }   
          if ((Current==T_VERNIE) && (_Det==T2)){                             // se il corrente è T con dettaglio T2
            Current=OldCurrent;                                               // rirpistina al vecchio titolo
            _Det=OldDet;                                                      // ripristina al vecchio dettaglio
            if (Current==FILE){Current=TRACK1; _Det=0;}
            break;
          }
        }
        if (Delay_Cnt == 0) {   
          Save_Param();                             // ±£´æµ±Ç°²Ù×÷ÉèÖÃ²ÎÊý   
          if(Current != FILE){
            for (j=0; j<5; j++) {
              Print_Str(91, 0, 0x0405, PRN, "       Save Settings       ");
              Delayms(300);
              Print_Str(91, 0, 0x0405, PRN, "                           ");
              Delayms(200);
            }
          }
        }
      }
    }
            
   
    
    //-------------------------------------------------------------------------------------------------------------        
    if(Key_Buffer== KEY4){                                                    // ===--- TASTO 4 PREMUTO --====
      if ((Current != FILE)) {                                                //
        Delay_Cnt = 700;  //*************************************
        while (Delay_Cnt > 0){
          if((__Get(KEY_STATUS)& KEY4_STATUS)!=0){
            if (UpdateMeter<4){
              UpdateMeter=UpdateMeter+1;
            }
            else{
              UpdateMeter=0;
            } 
            switch (UpdateMeter){
            case 0:
            case 1:
            case 2:
            case 3:
              if (UpdateMeter==1) {
                Title[V_VERNIE][2].Value=1;
              }
              else
              {  
                Title[V_VERNIE][2].Value=0;
              }
              Meter[0].Flag |= UPDAT;
              Meter[0].Item  =  VBT;
              Meter[0].Track = 4;
              for(CounterUpdate=1;CounterUpdate<9;++CounterUpdate){ 
                Meter[CounterUpdate].Flag |= UPDAT;
                Meter[CounterUpdate].Item  =  CounterUpdate+1; // selezione item
                Meter[CounterUpdate].Track = UpdateMeter;
              }
              break;
            case 4:
              
              Meter[0].Flag |= UPDAT;
              Meter[0].Item  =  VBT;
              Meter[0].Track = 4;  
              for(CounterUpdate=1;CounterUpdate<9;++CounterUpdate){    
                Meter[CounterUpdate].Flag |= UPDAT;
              }
              Meter[1].Item  =  RMS; Meter[1].Track = 0;
              Meter[2].Item  =  FRQ; Meter[2].Track = 0;
              Meter[3].Item  =  RMS; Meter[3].Track = 1;
              Meter[4].Item  =  FRQ; Meter[4].Track = 1;
              Meter[5].Item  =  RMS; Meter[5].Track = 2;
              Meter[6].Item  =  FRQ; Meter[6].Track = 2;
              Meter[7].Item  =  RMS; Meter[7].Track = 3;
              Meter[8].Item  =  FRQ; Meter[8].Track = 3;
              break;
            }
            break;  
          }
        }
        if(Delay_Cnt == 0){ 
          if (FlagFrameMode==0){
            FlagFrameMode=1;
          }
          else
          {
            FlagFrameMode=0;
          }
        }
      }
      
      
      
      if(Current == FILE){
        Print_Str(230, 0, (SCRN<<8)+ TEXT1, PRN, " >   ");
        if(_Curr[0].Value == SAVE){
          switch (_Curr[2].Value){
          case BMP:
            i = Save_Bmp(_Curr[1].Value);
            break;
          case DAT:
            i=Save_Dat(_Curr[1].Value);
            Delayms(1000);
            break;
          case BUF:
            i=Save_Buf(_Curr[1].Value);
            break;
          case CSV:
            i=Save_Csv(_Curr[1].Value);
            break;
          }
          _Curr[1].Value++;
        } else { 
          i=0;
          if(_Curr[2].Value==DAT) i = Load_Dat(_Curr[1].Value);
          if(_Curr[2].Value==BUF) i = Load_Buf(_Curr[1].Value);
        }
        if       (i == OK  ) Print_Str(230, 0, (SCRN<<8)+ TEXT1, PRN, " OK  ");  
        else{ 
          if     (i == EMPT) Print_Str(230, 0, (SCRN<<8)+ TEXT1, PRN, " NONE");  
          else if(i == OVER) Print_Str(230, 0, (SCRN<<8)+ TEXT1, PRN, " FULL");  
          else               Print_Str(230, 0, (SCRN<<8)+ TEXT1, PRN, " ERR ");  
        }
        Delayms(1000);
        _Curr[1].Flag |= UPDAT; 
        _Curr[2].Flag |= UPDAT;
      }  
    }     

//----------------------------------------------------------------------------------------------------------------
    if(Key_Buffer == K_INDEX_DEC){                                            // ===--- TASTO 5 --- PREMUTO --====
      if(Current < METER_0){
        if((Current == TRIGG)&&(Detail[Current]==2)){         // ´¥·¢µçÆ½µ÷½Ú
          if(V_Trigg[_Trigg[SOURCE].Value].Value > MIN_Y+4) 
            V_Trigg[_Trigg[SOURCE].Value].Value--;
        } else if((Current == BK_LIGHT)||(Current == VOLUME)){// ±³¹â»òÒôÁ¿µ÷½Ú
          if(_Curr[1].Value > 0)   _Curr[1].Value--;
        } else if((Current == T_BASE)&&(_Det == XPOSI)){      // X_POSIµ÷½Ú
          if(_Curr[_Det].Value > 30) _Curr[_Det].Value -= 30; 
          else if(_Curr[_Det].Value > 0) _Curr[_Det].Value--;
          _X_View.Flag |= UPDAT;                              // Ë¢ÐÂX_View
        } else {                                              // µ±Ç°ÏîÎªÆäËû
          if(_Curr[_Det].Value > 0) _Curr[_Det].Value--; 
          else if(_Curr[_Det].MARK & CIRC) _Curr[_Det].Value =_Curr[_Det].Limit;
        }
        if((Current == T_BASE)&&(_Det == MODE)){     // T_BASE MODE Ñ¡Ôñ
          Title[RUNNING][STATE].Value = RUN;         // STATE = RUNNING 
          Title[RUNNING][STATE].Flag |= UPDAT;       // Ë¢ÐÂ RUNNING STATE
        }
        if(((Current == OUTPUT)&&(Title[OUTPUT][SOURCE].Value != DIGI))&&
           (Title[OUTPUT][FRQN].Value > 10)) 
          Title[OUTPUT][FRQN].Value = 10;            // Ä£ÄâÐÅºÅÆµÂÊÉÏÏÞÎª20KHz
        if((Current == FILE)&&(_Curr[0].Value == LOAD)){ 
          if(_Curr[2].Value == BMP) _Curr[2].Value = BUF;  // Ö»ÄÜLoad Dat,BufÎÄ¼þ
          if(_Curr[2].Value == CSV) _Curr[2].Value = BUF;  // Ö»ÄÜLoad Dat,BufÎÄ¼þ
        }
        _Curr[0].Flag |= UPDAT;
        _Curr[1].Flag |= UPDAT;
        _Curr[2].Flag |= UPDAT;
        _Curr[3].Flag |= UPDAT;
      } else {
        Meter[Current-METER_0].Flag |= UPDAT;
        if(Meter[Current-METER_0].Item  > VBT) 
          Meter[Current-METER_0].Item -= 1;          // ¸Ä±ä²âÁ¿ÏîÄ¿ 
        else                     
          Meter[Current-METER_0].Item  = TL;//MIN;
        if(Meter[Current-METER_0].Item == FPS) 
          Meter[Current-METER_0].Track = 4;
        if(Meter[Current-METER_0].Item == TL)//MIN) 
          Meter[Current-METER_0].Track = 0;
      }
    }
    
//---------------------------------------------------------------------------------------------------------------------       
    if(Key_Buffer == K_INDEX_S){                                              // ===--- TASTO 5 centrale PREMUTO --====

      
      if(Current < METER_0){                         // ¸Ä±äDetail
            _Curr[_Det].Flag |= UPDAT;
            if(_Det < 3)    _Det += 1;
            else            _Det  = 0;
            if(_Curr[_Det].MARK & NOT)                        _Det  = 0; 
            if(_Curr[_Det].MARK & NUM2)                       _Det  = 0; 
            if((_Curr[_Det].MARK & NUM3)&&(Current != FILE))  _Det  = 0;  
            _Curr[_Det].Flag |= BLINK;
          } else {                                       // ¸Ä±ä²âÁ¿¶ÔÏó
            Meter[Current-METER_0].Flag |= UPDAT;
            if(Meter[Current-METER_0].Track <=  TRACK4) 
              Meter[Current-METER_0].Track += 1;
            if(Meter[Current-METER_0].Track > TRACK4) 
              Meter[Current-METER_0].Track  = TRACK1;
          }
      
      
      
      
      
      
      
      

        }
//----------------------------------------------------------------------------------------------------------------    
    if(Key_Buffer == K_INDEX_INC){                                            // ===--- TASTO 5 +++ PREMUTO --====
      
      
      if(Current < METER_0){
        if((Current == TRIGG)&&(Detail[Current]==2)){         // ´¥·¢µçÆ½µ÷½Ú
            if(V_Trigg[_Trigg[SOURCE].Value].Value < MAX_Y-4) 
            V_Trigg[_Trigg[SOURCE].Value].Value++;
        } else if((Current == BK_LIGHT)||(Current == VOLUME)){// ±³¹â»òÒôÁ¿µ÷½Ú
          if(_Curr[1].Value < _Curr[1].Limit)   _Curr[1].Value++;
        } else if((Current == T_BASE)&&(_Det == XPOSI)){      // X_POSIµ÷½Ú
          if (_Curr[_Det].Value <(_Curr[_Det].Limit-30)) _Curr[_Det].Value += 30; //
          else if (_Curr[_Det].Value < _Curr[_Det].Limit) _Curr[_Det].Value ++;
          _X_View.Flag |= UPDAT;                      
        } else {                                              // µ±Ç°ÏîÎªÆäËû
          if(_Curr[_Det].Value < _Curr[_Det].Limit)  _Curr[_Det].Value++;
          else if(_Curr[_Det].MARK & CIRC)   _Curr[_Det].Value  = 0;
        }
        if((Current == T_BASE)&&(_Det == MODE)){     // T_BASE MODE Ñ¡Ôñ
          Title[RUNNING][STATE].Value = RUN;         // STATE = RUNNING 
          Title[RUNNING][STATE].Flag |= UPDAT;       // Ë¢ÐÂ RUNNING STATE
        }
        if((Current == OUTPUT)&&(Title[OUTPUT][SOURCE].Value != DIGI)){
          if(Title[OUTPUT][FRQN].Value > 10) 
            Title[OUTPUT][FRQN].Value = 10;          // Ä£ÄâÐÅºÅÆµÂÊÉÏÏÞÎª20KHz
        }
        if((Current == FILE)&&(_Curr[0].Value == 1)){  // Ö»ÄÜLoad Dat,BufÎÄ¼þ
          if(_Curr[2].Value == BMP) _Curr[2].Value = DAT;
          if(_Curr[2].Value == CSV) _Curr[2].Value = DAT;
        }
        _Curr[0].Flag |= UPDAT;
        _Curr[1].Flag |= UPDAT;
        _Curr[2].Flag |= UPDAT;
        _Curr[3].Flag |= UPDAT;
      } else {
        Meter[Current-METER_0].Flag |= UPDAT;
        if(Meter[Current-METER_0].Item < TL)//MIN)  
          Meter[Current-METER_0].Item += 1;          // ¸Ä±ä²âÁ¿ÏîÄ¿
        else                     
          Meter[Current-METER_0].Item  = VBT;
        if(Meter[Current-METER_0].Item == VBT) 
          Meter[Current-METER_0].Track = 4;
        if(Meter[Current-METER_0].Item == VPP) 
          Meter[Current-METER_0].Track = 0;
      }
    }
//---------------------------------------------------------------------------------------------------------------- 
    if(Key_Buffer == K_ITEM_DEC){                                             // ===--- TASTO 6 --- PREMUTO --====
      if(Current < METER_0){
        _Curr[_Det].Flag |= UPDAT;
        if(Current == TRACK1)  Current = VOLUME;
        else                   Current --;
        if(Current == RUNNING) Current --;               // Jump over Item 4
        if(_Det >2)  _Det =0;
        _Curr[_Det].Flag |= BLINK;
        _Curr[0].Flag |= UPDAT;
        _Curr[1].Flag |= UPDAT;
        _Curr[2].Flag |= UPDAT;
        _Curr[3].Flag |= UPDAT;
      } else {
        Meter[Current-METER_0].Flag |= UPDAT; 
        if(Current == METER_0) Current = METER_8;
        else                   Current --;
        Meter[Current-METER_0].Flag |= BLINK;
      }
    }
//---------------------------------------------------------------------------------------------------------------------
    if(Key_Buffer == K_ITEM_S){                                               // ===--- TASTO 6 centrale PREMUTO --====    
      if ((TrgAuto==1)&&(Current == TRIGG) && _Det==2) _Det=0;
      if(Current < METER_0) 
      {
        _Curr[_Det].Flag |= UPDAT;
        Current = METER_0;
       }
      else     
      {
        for(CounterUpdate=0;CounterUpdate<9;++CounterUpdate){  // Udate di tutti i meter per tenterli accesi
                Meter[CounterUpdate].Flag |= UPDAT;
         }
        Current = TRACK1;
       
      }
    }
 //---------------------------------------------------------------------------------------------------------------  
    if(Key_Buffer == K_ITEM_INC){                                             // ===--- TASTO 6 +++ PREMUTO --====
      if(Current < METER_0){
        _Curr[_Det].Flag |= UPDAT;
        if(Current == VOLUME)  Current = TRACK1;
        else                   Current ++;
        if(Current == RUNNING) Current ++;              // Jump over RUNNING
        if(_Det >2)    _Det =0;
        _Curr[_Det].Flag |= BLINK;
        _Curr[0].Flag |= UPDAT;
        _Curr[1].Flag |= UPDAT;
        _Curr[2].Flag |= UPDAT;
        _Curr[3].Flag |= UPDAT;
      } else {
        Meter[Current-METER_0].Flag |= UPDAT;  
        if(Current == METER_8) Current  = METER_0;
        else                   Current ++;
        Meter[Current-METER_0].Flag |= BLINK;
      }
    }
//----------------------------------------------------------------------------------------------------------------     
    Key_Buffer=0;
    Update = 1;
  }
}
}
/*********************************  END OF FILE  ******************************/
