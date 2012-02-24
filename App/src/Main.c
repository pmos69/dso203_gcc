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

#define APP_VERSION       "GCC v1.6 APP (2.51 SmTech 1.8 PMOS69-fixes)"

uc8 PROJECT_STR[20] = "Demo PROG. Ver 1.00";
u8 OldCurrent;
u8 OldDet;
u8 OldMode;
u8 OldFrame;
u16 OldPosi;
u8 OldMeter;
u8 FlagFrameMode;
u8 FlagMeter;
u8 TrgAuto;
u8 TrgAutoOld;
u16 OldPosX;
u16 OldPosY;
u16 OldTrack1X;
u16 OldTrack2X;

extern u8 _vectors[];


/*******************************************************************************
  main : Main routine.
*******************************************************************************/
int main(void)
{ 
  s32 i;
  u16 Count_FPS = 0, Second = 0;//,Offset, Result 
  u8  UpdateMeter=0, CounterUpdate  ;
  u8 j;
  //u8 z;
  u16 TmpVT;
  u16 Vt1Old=0;
  u16 Vt2Old=0;
  u16 _Vt1Old;
  
//  u8 N[20];
//  u8 T_Unit[15]={'u','S','u','S','m','S','S'};
  
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, (u32)&_vectors);
  
//Note: ÓÃ IAR_V4.x ±àÒëÊ±£¬±ä¸ü App#n »¹ÒªÍ¬Ê±ÐÞ¸Ä lnkarm.xcl ÎÄ¼þÖÐµÄ¶ÔÓ¦Ïî 
//      ÓÃ IAR_V5.x ±àÒëÊ±£¬±ä¸ü App#n »¹ÒªÍ¬Ê±ÐÞ¸Ä xxxxxx.icf ÎÄ¼þÖÐµÄ¶ÔÓ¦Ïî 
  
  __USB_Init();
  
  if(__Get(FPGA_OK)== 0){
    __Display_Str(6*8, 30, YEL, PRN, "      FPGA configuration error       ");
    while (1){};
  }
  
  __Display_Str(32, 50, WHT, PRN, APP_VERSION);
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
  
  if (FlagMeter==1)MAX_X=300+MIN_X;
  else MAX_X=386+MIN_X;
  
  OldMode=_Mode;
  OldTrack1X=Title[TRACK1][POSI].Value;
  OldTrack2X=Title[TRACK2][POSI].Value;
  _Vt1Old=_Vt1;
  TrgAutoOld=TrgAuto;   
  
   //-------------------------------------------------------------------------- ************** LOOP MAIN *****************
  while (1){
  
  
	//-------------------------------------------------------------------------- Gestione Entra e Uscita da Modalità X Y
       if (_Mode!=OldMode)
    {
      if (_Mode ==X_Y ){       // entra in XY_S 
        BackGround_Reset();
        if (OldMode<4){
          OldTrack1X=Title[TRACK1][POSI].Value;
          OldTrack2X=Title[TRACK2][POSI].Value;
        }
        Title[TRACK1][POSI].Value=100;
        Title[TRACK2][POSI].Value=100;
        OffsetX=135;
        OffsetY=100;
        OldMeter=FlagMeter;
        if (FlagMeter==0) EnableMeter();
        _Vt1Old=_Vt1;
        TrgAutoOld=TrgAuto;
      }
     
      if (_Mode ==X_Y_A ){  // entra in XY_A 
        BackGround_Reset();
         if (OldMode<4){           // se era in oscilloscopio abilita i meter e memorizza come erano
          OldTrack1X=Title[TRACK1][POSI].Value;
          OldTrack2X=Title[TRACK2][POSI].Value;
          OldMeter=FlagMeter;
          if (FlagMeter==0) EnableMeter();
        } 
        if (OldMode==X_Y){       // se era in XY_S ripristina il trigger
         _Vt1=_Vt1Old;
        TrgAuto=TrgAutoOld;
        }
        Title[TRACK1][POSI].Value=100;
        Title[TRACK2][POSI].Value=100;
        OffsetX=135;
        OffsetY=100;
       
      }
         
      if ((_Mode<4) &&(OldMode>3)){     // esce da XY_S o XY_A
        Title[TRACK1][POSI].Value = OldTrack1X;
        Title[TRACK2][POSI].Value = OldTrack2X;
        BackGround_Reset();
        if (OldMeter==0)  DisableMeter();
        if (OldMode==X_Y){       // se era in XY_S ripristina il trigger
         _Vt1=_Vt1Old;
        TrgAuto=TrgAutoOld;
        }
        
      }
    }
   
     OldMode=_Mode;
   // if (_Mode==X_Y) Title[TRACK1][POSI].Value=120;
   // if (_Mode==X_Y) Title[TRACK2][POSI].Value=100;
    if ((_Mode==X_Y) ||  (_Mode==X_Y_A))  Title[TRACK1][POSI].Value=101+MIN_Y;
    if ((_Mode==X_Y) ||  (_Mode==X_Y_A))   Title[TRACK2][POSI].Value=100;
    
    if (_Mode == X_Y){ 
      TrgAuto=0;
      _Vt1=200;
    }
	
    //-------------------------------------------------------------------------- ********* LIFE *************    
    if(PD_Cnt == 0){
      __Set(BACKLIGHT, 0);     // ¹Ø±Õ±³¹â
      __Set(STANDBY, EN);      // ½øÈëÊ¡µç×´Ì¬  
    } else {
      Synchro();                              // Í¬²½ÏÔÊ¾¸÷¸ö¹ì¼£²¨ÐÎÊý¾Ý
	  if (TimedDeltaView>0){
        Title[T_VERNIE][2].Flag |= UPDAT; 
        _D_V_Source.Flag |= UPDAT;            // Updat delta V 
        _Delta_V.Flag    |= UPDAT; 
        _Delta_T.Flag    |= UPDAT;            // Updat delta T 
      }
	  
      Count_FPS++;
      if(Second != Sec_Cnt) {
        Second = Sec_Cnt;
        Result_FPS = Count_FPS;
        Count_FPS = 0;
        Update_Battery();
        if (FlagMeter==1) { for(i=0; i<9; ++i)  Display_Value(i);} // Ã¿ÃëË¢ÐÂ²âÁ¿Öµ        
      }
      if (FlagMeter==1) Display_Meter();
      Display_Title();
      if(Update){                             // ´¦Àí°´¼üºóÐèÒªË¢ÐÂµÄÏîÄ¿
	    if (TrgAuto==0)  Print_Str(365,  216, ((SCRN<<8)+Title[TRIGG][SOURCE].Value), PRN, "!Man!");
        if (TrgAuto==1)  Print_Str(365,  216, ((SCRN<<8)+Title[TRIGG][SOURCE].Value), INV, "!1/4!");
        if (TrgAuto==2)  Print_Str(365,  216, ((SCRN<<8)+Title[TRIGG][SOURCE].Value), INV, "!1/2!");
        if (TrgAuto==3)  Print_Str(365,  216, ((SCRN<<8)+Title[TRIGG][SOURCE].Value), INV, "!3/4!");
        Update_Range();
        Update_Base();
        Update_Output();
        if (_Mode!=X_Y)  Update_Trig();
        if ((_Mode!=X_Y) && (_Mode!=X_Y_A)) Update_Mark();
        __Set(BACKLIGHT, 10*(Title[BK_LIGHT][CLASS].Value+1));
        if(Current != FILE) Update_View_Area();
		if (FlagMeter==1) {
			_D_V_Source.Flag |= UPDAT;            // Updat delta V 
			_Delta_V.Flag    |= UPDAT; 
			_Delta_T.Flag    |= UPDAT;            // Updat delta T 
		}
        Update = 0;                           // Update finish
      } 
      Blink = 0;
    }
    if((_State.Value == HOLD)&&((__Get(FIFO_FULL)!= 0)||(__Get(FIFO_START)== 0))){
//    if((_State.Value == HOLD)&&(Twink== 1)){
      _State.Value = 2; 
      _State.Flag |= UPDAT;
    }

     //----------------------------------------------------------------------------------  PWM 
//    if (Current == OUTPUT) {
//      if ((_Det==DUTYPWM) || (_Det==OUTATT)){ z=Twink; } else { z=INV;}
//      if  (Title[OUTPUT][KIND].Value == PWM)
//      {
//        u8ToDec3(Title[OUTPUT][DUTYPWM].Str, Title[OUTPUT][DUTYPWM].Value);
//        Print_Str(156,  216, (Title[OUTPUT][DUTYPWM].Color[0]), z, "Duty % "); //225,202[Title[OUTPUT][PWM].Value]
//        Print_Str(Title[OUTPUT][DUTYPWM].XPOS, Title[OUTPUT][DUTYPWM].YPOS,
//                  (Title[OUTPUT][DUTYPWM].Color[0]),z,Title[OUTPUT][DUTYPWM].Str);
//      }  
//      else
//      {
//        Print_Str(156,  216, (Title[OUTPUT][DUTYPWM].Color[0]), z, "!!Out ");//230,202
//        Int2Str(NumStr, (Title[OUTPUT][OUTATT].Value*26000), V_UNIT, 2, UNSIGN);
//        Print_Str(Title[OUTPUT][OUTATT].XPOS, Title[OUTPUT][OUTATT].YPOS,
//                  (Title[OUTPUT][OUTATT].Color[0]),z,NumStr);
//      }
//    }
   
    //--------------------------------------------------------------------------  TRIGGER AUTOMATICO
    if  (TrgAuto>0){                                                                   
      if ((Current == TRIGG) && (_Det==2)){ Title[Current][_Det].Flag |= UPDAT;_Det=0;}
      if (Title[TRIGG][SOURCE].Value == TRACK1){
        TmpVT=a_Min+((a_Max-a_Min)*(TrgAuto*2)/8);
        if ((TmpVT>Vt1Old +5) || (TmpVT<Vt1Old -5)){ _Vt1=TmpVT;Update=1;Vt1Old=_Vt1;} 
      }
      if (Title[TRIGG][SOURCE].Value == TRACK2){
        TmpVT=b_Min+((b_Max-b_Min)*(TrgAuto*2)/8);
        if ((TmpVT>Vt2Old +5) || (TmpVT<Vt2Old -5)){ _Vt2=TmpVT;Update=1;Vt2Old=TmpVT;} 
      }
    }
    //--------------------------------------------------------------------------  FRAME MODE
    
    if (FlagFrameMode==1){
          if (_Mode==SCAN) {
          //  _X_posi.Value=200;
            _X_posi.Value=0;
              FrameMode=1;
            
          } 
          else 
          {
              _X_posi.Value=0;
             if (Title[T_BASE][1].Value<11) {FrameMode=2; } //2
             else { FrameMode=0;} //0
          }
    }
    else
    {
      FrameMode=0;
    }
 
   //--------------------------------------------------------------------------  GESTIONE TASTI MENU	
    if(Key_Buffer) { 
      if(PD_Cnt == 0)  App_init();          // ÍË³öÊ¡µç×´Ì¬
      PD_Cnt = 600;                         // 600Ãë
	  //--------------------------------------------------------------------------------------------------------------  
      if(Key_Buffer == KEY1){
        _State.Value = (_State.Value == 0)? 1 : 0;       						// "RUN/HOLD" ×´Ì¬»¥»»
        _State.Flag |= UPDAT;                          							// ÖÃÏàÓ¦µÄ¸üÐÂ±êÖ¾
        if((Current == FILE)&&(_Curr[2].Value == BUF)) reset_parameter();		//
        if(_Mode == SGL){														//
          for(i=0; i<4*X_SIZE; ++i)  TrackBuff[i] = 0; 							// Çå³ý¾ÉµÄÏÔÊ¾²¨ÐÎ
          __Set(FIFO_CLR, W_PTR);                      							// FIFOÐ´Ö¸Õë¸´Î»
        }
      }
      //--------------------------------------------------------------------------------------------------------------
      if(Key_Buffer== KEY2){                                                    // ===--- TASTO 2 PREMUTO ---===
        Delay_Cnt = 3000;                                                       // Carica il temporizzatore del tasto permuto
        while (Delay_Cnt > 0){                                                  // Se il tempo non è arrivato a 0
          if((__Get(KEY_STATUS)& KEY2_STATUS)!=0){                              // CAMBIO MODO TRIGGER
            TrgAuto++;
            if (TrgAuto>3) TrgAuto=0;
            Title[Current][_Det].Flag |= UPDAT;
            if (TrgAuto==0) {(Current = TRIGG); _Det=2;}
            break;
          }  
        }
        if((Delay_Cnt == 0) && ((Current == TRACK1)||(Current == TRACK2)) )Calibrat(Current);  // CALIBRAZIONE
      }
      //--------------------------------------------------------------------------------------------------------------     
      if(Key_Buffer== KEY3){                                                    // ===--- TASTO 3 PREMUTO --====
        Delay_Cnt = 1500;                                                       // Carica il temporizzatore del tasto permuto
        while (Delay_Cnt > 0){                                                  // Se il tempo non è arrivato a 0
          if((__Get(KEY_STATUS)& KEY3_STATUS)!=0){ 
            if (FlagMeter==1)  
            {
              if ((_Mode != X_Y)  &&  (_Mode!=X_Y_A)){
                DisableMeter();
              }
              break;
            }
            else
            {
                EnableMeter();
              break;       
            }
              
//             Title[Current][_Det].Flag |= UPDAT;                                // aggiorna titolo corrente per lasciare lo sfondo acceso
//            if ((Current != T_VERNIE) && (Current != V_VERNIE)){                // se il titolo non è gia dei cursori
//              OldCurrent=Current;                                               // memorizza il titolo corrente
//              OldDet=_Det;                                                      // memorizza il dettaglio corrente
//              Current = V_VERNIE;                                               // vai sul titolo del cursore V
//              _Det=V1 ;                                                         // imposta il detaglio V1
//              break;
//            }
//            if ((Current==V_VERNIE) && (_Det==V1)){                             // se il corrente è V con dettaglio V1            
//              _Det=V2;                                                          // imposta il dettaglio a V2 
//              break;
//            }
//            if ((Current==V_VERNIE) && (_Det==V2)){                             // se il corrente è V con dettaglio V2
//              Current=T_VERNIE;                                                 // imposta il titolo a T 
//              _Det=T1;                                                          // imposta il dettaglio a T1  
//              break;
//            }
//            if ((Current==T_VERNIE) && (_Det==T1)){                             // se il corrente è T con dettaglio T1
//              _Det=T2;                                                          // imposta il dettaglio a T2 
//              break;
//            }   
//            if ((Current==T_VERNIE) && (_Det==T2)){                             // se il corrente è T con dettaglio T2
//              Current=OldCurrent;                                               // rirpistina al vecchio titolo
//              _Det=OldDet;                                                      // ripristina al vecchio dettaglio
//              if (Current==FILE){Current=TRACK1; _Det=0;}
//              break;
//            }
          }
          if (Delay_Cnt == 0) {   
            Save_Param();                             // ±£´æµ±Ç°²Ù×÷ÉèÖÃ²ÎÊý   
            if(Current != FILE){
              for (j=0; j<4; j++) {
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
              if (FlagMeter==1){ 
                if (UpdateMeter<4){
                  UpdateMeter=UpdateMeter+1;
                  if  (((_Mode==X_Y) ||  (_Mode==X_Y_A))&& (UpdateMeter>1)) UpdateMeter=4;
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
                    _D_V_Source.Value=1;
                  }
                  else
                  {  
                    _D_V_Source.Value=0;
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
                  if ((_Mode==X_Y_A)||(_Mode==X_Y) || ( (Title[TRACK3][SOURCE].Value == HIDE) && (Title[TRACK4][SOURCE].Value == HIDE)))
                  {
                    Meter[1].Item  =  RMS; Meter[1].Track = 0;
                    Meter[2].Item  =  VDC; Meter[2].Track = 0;
                    Meter[3].Item  =  VPP; Meter[3].Track = 0;
                    Meter[4].Item  =  FRQ; Meter[4].Track = 0;
                    Meter[5].Item  =  RMS; Meter[5].Track = 1;
                    Meter[6].Item  =  VDC; Meter[6].Track = 1;
                    Meter[7].Item  =  VPP; Meter[7].Track = 1;
                    Meter[8].Item  =  FRQ; Meter[8].Track = 1;
                  }  
                  else  
                  {
                    Meter[1].Item  =  RMS; Meter[1].Track = 0;
                    Meter[2].Item  =  FRQ; Meter[2].Track = 0;
                    Meter[3].Item  =  RMS; Meter[3].Track = 1;
                    Meter[4].Item  =  FRQ; Meter[4].Track = 1;
                    Meter[5].Item  =  RMS; Meter[5].Track = 2;
                    Meter[6].Item  =  FRQ; Meter[6].Track = 2;
                    Meter[7].Item  =  RMS; Meter[7].Track = 3;
                    Meter[8].Item  =  FRQ; Meter[8].Track = 3;
                  }
                
                
                
                  break;
                }
               // break;  
              }
              break;
            }
          }
                   
          if(Delay_Cnt == 0){ 
            if (FlagFrameMode==0){
              FlagFrameMode=1;
              OldPosi=_X_posi.Value; //Fronte positivo entra in frame mode memorizza posizione
            
             
             }
            else
            {
              FlagFrameMode=0;
             _X_posi.Value=OldPosi; //Fronte negativo esce dal frame mode ripristina posizione
            
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
	    
         if ((_Mode == X_Y) || (_Mode==X_Y_A)){
           if((Current == TRACK1)&&(_Det == POSI) && (OffsetX>45)) OffsetX=OffsetX-2;
           if((Current == TRACK2)&&(_Det == POSI) && (OffsetY>30)) OffsetY=OffsetY-2;
         }  
 
        if ((Current == T_VERNIE) || ((Current == V_VERNIE)&& (FlagMeter==0))) TimedDeltaView=150;
		
        if(Current < METER_0){
          if((Current == TRIGG)&&(Detail[Current]==2)){         // ´¥·¢µçÆ½µ÷½Ú
            if(V_Trigg[_Trigg[SOURCE].Value].Value > MIN_Y+4) 
              V_Trigg[_Trigg[SOURCE].Value].Value--;
          } else if((Current == BK_LIGHT)||(Current == VOLUME)){// ±³¹â»òÒôÁ¿µ÷½Ú
            if(_Curr[1].Value > 0)   _Curr[1].Value--;
          } else if((Current == T_BASE)&&(_Det == XPOSI)&& (FlagFrameMode==0)){      // X_POSIµ÷½Ú
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
          if((Current == OUTPUT)&&(_Kind != PWM)){
             if (Title[OUTPUT][FRQN].Value > 14) 
            Title[OUTPUT][FRQN].Value = 14;            // Ä£ÄâÐÅºÅÆµÂÊÉÏÏÞÎª20KHz
           } 
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
	    _Curr[_Det].Flag |= UPDAT;
          if ((Current == TRACK3)&&(_Det == 0))_Det = 2;
            if ((Current == TRACK4)&&(_Det == 0))_Det = 2;
       
        
        if ((FlagMeter==0)&&(Current == V_VERNIE)&& (_Det==V2)){ _Curr[_Det].Flag |= UPDAT;_Det=V1;goto EndKIndex;}
		
        if(Current < METER_0){                         // ¸Ä±äDetail
          _Curr[_Det].Flag |= UPDAT;
          if(_Det < 3)    _Det += 1;
          else            _Det  = 0;
		  
          if ((Current == OUTPUT) && (Title[OUTPUT][KIND].Value==PWM) && (_Det==OUTATT)) _Det=0;
          if ((Current == OUTPUT) && (Title[OUTPUT][KIND].Value!=PWM) && (_Det==DUTYPWM)) _Det+=1;  
          
          if(_Curr[_Det].MARK & NOT)                        _Det  = 0; 
          if(_Curr[_Det].MARK & NUM2)                       _Det  = 0; 
          if((_Curr[_Det].MARK & NUM3)&&(Current != FILE)&& (Current != OUTPUT) )  _Det  = 0;  
          _Curr[_Det].Flag |= BLINK;
        } else {                                       // ¸Ä±ä²âÁ¿¶ÔÏó
          Meter[Current-METER_0].Flag |= UPDAT;
          if(Meter[Current-METER_0].Track <=  TRACK4) 
            Meter[Current-METER_0].Track += 1;
          if(Meter[Current-METER_0].Track > TRACK4) 
            Meter[Current-METER_0].Track  = TRACK1;
        }
      EndKIndex:
       ; 
      }
      //----------------------------------------------------------------------------------------------------------------    
      if(Key_Buffer == K_INDEX_INC){                                            // ===--- TASTO 5 +++ PREMUTO --====
        
     if ((_Mode == X_Y) || (_Mode==X_Y_A)){
           if((Current == TRACK1)&&(_Det == POSI) && (OffsetX<215)) OffsetX=OffsetX+2;
           if((Current == TRACK2)&&(_Det == POSI) && (OffsetY<170)) OffsetY=OffsetY+2;
         }  
     
        if ((Current == T_VERNIE) || ((Current == V_VERNIE) && (FlagMeter==0))) TimedDeltaView=150;
        if ((TrgAuto>0)&&(Current == TRIGG) && _Det==2) _Det=0;
        if(Current < METER_0){
          if((Current == TRIGG)&&(Detail[Current]==2)){         // ´¥·¢µçÆ½µ÷½Ú
            if(V_Trigg[_Trigg[SOURCE].Value].Value < MAX_Y-4) 
              V_Trigg[_Trigg[SOURCE].Value].Value++;
          } else if ((Current == BK_LIGHT)||(Current == VOLUME)){// ±³¹â»òÒôÁ¿µ÷½Ú
            if(_Curr[1].Value < _Curr[1].Limit)   _Curr[1].Value++;
          } else if ((Current == T_BASE)&&(_Det == XPOSI) && (FlagFrameMode==0)){      // X_POSIµ÷½Ú
            if (_Curr[_Det].Value <30){ _Curr[_Det].Value ++; goto OkSlow; }//
            else if (_Curr[_Det].Value <(_Curr[_Det].Limit-30)) _Curr[_Det].Value += 30; //
            else if (_Curr[_Det].Value < (_Curr[_Det].Limit)) _Curr[_Det].Value ++;
          OkSlow:
            _X_View.Flag |= UPDAT;                      
          }  else if ((Current == T_VERNIE) && ((_Det==T1) || (_Det==T2))){
            
                    if (FlagMeter==1){
                      if (_Curr[_Det].Value <(_Curr[_Det].Limit-100)) _Curr[_Det].Value ++;
                    }
                    else
                    {
                      if (_Curr[_Det].Value <_Curr[_Det].Limit-MIN_X-1) _Curr[_Det].Value ++; 
                    }
           }
          else {                                              // µ±Ç°ÏîÎªÆäËû
            if(_Curr[_Det].Value < _Curr[_Det].Limit)  _Curr[_Det].Value++;
            
            
            
            else if(_Curr[_Det].MARK & CIRC)   _Curr[_Det].Value  = 0;
          }
          if((Current == T_BASE)&&(_Det == MODE)){     // T_BASE MODE Ñ¡Ôñ
            Title[RUNNING][STATE].Value = RUN;         // STATE = RUNNING 
            Title[RUNNING][STATE].Flag |= UPDAT;       // Ë¢ÐÂ RUNNING STATE
          }
          if((Current == OUTPUT)&&(_Kind != PWM)){
            if(Title[OUTPUT][FRQN].Value > 14) 
              Title[OUTPUT][FRQN].Value = 14;          // Ä£ÄâÐÅºÅÆµÂÊÉÏÏÞÎª20KHz
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
          if (FlagMeter==1){
            if(Current == TRACK1)  {Current = VOLUME;}
            else                  {Current --;}
          }
          else
          {
            if(Current == TRACK1)  {Current = T_VERNIE;}
            else                   {Current --;}
          }
          if(Current == RUNNING) Current --;               // Jump over Item 4
		  
          //if(_Det >2)  _Det =0;
          _Curr[_Det].Flag |= BLINK;
		   _Curr[_Det].Flag |= UPDAT;
          if (Current==FILE){
            _Curr[0].Flag |= UPDAT;
            _Curr[1].Flag |= UPDAT;
            _Curr[2].Flag |= UPDAT;
            _Curr[3].Flag |= UPDAT;
		  }
        } else {
          Meter[Current-METER_0].Flag |= UPDAT; 
          if(Current == METER_0) Current = METER_8;
          else                   Current --;
          Meter[Current-METER_0].Flag |= BLINK;
        }
      }
	  //---------------------------------------------------------------------------------------------------------------------
      if(Key_Buffer == K_ITEM_S){    // ===--- TASTO 6 centrale PREMUTO --====   
        _Curr[_Det].Flag |= UPDAT;
        if (FlagMeter==0){
          if (((Current <= METER_8)&& (Current >= METER_0)) || (Current==BK_LIGHT) || (Current==VOLUME) ){Current=TRACK1 ;}
          
          goto EndItem_S;
        }
        if(Current < METER_0) 
        {
          Current = METER_0;
        }
        else     
        {
          for(CounterUpdate=0;CounterUpdate<9;++CounterUpdate){  // Udate di tutti i meter per tenterli accesi
            Meter[CounterUpdate].Flag |= UPDAT;
          }
          Current = TRACK1;
        }
      EndItem_S:;
      }
      //---------------------------------------------------------------------------------------------------------------  
      if(Key_Buffer == K_ITEM_INC){                                             // ===--- TASTO 6 +++ PREMUTO --====
        if(Current < METER_0){
          _Curr[_Det].Flag |= UPDAT;
          if ((FlagMeter==0) && (Current == T_VERNIE))  {Current = TRACK1;goto NoInc;}
          if(Current == VOLUME)  Current = TRACK1;
          else                   Current ++;
        NoInc: 
          if(Current == RUNNING) Current ++;              // Jump over RUNNING
      //    if(_Det >2)    _Det =0;
          _Curr[_Det].Flag |= BLINK;
          _Curr[_Det].Flag |= UPDAT;
          if (Current==FILE){
            _Curr[0].Flag |= UPDAT;
            _Curr[1].Flag |= UPDAT;
            _Curr[2].Flag |= UPDAT;
            _Curr[3].Flag |= UPDAT;
          }
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

void DisableMeter(void)
{
              if (((Current <= METER_8)&& (Current >= METER_0)) || (Current==BK_LIGHT) || (Current==VOLUME) ){_Curr[_Det].Flag |= UPDAT;Current=TRACK1 ;}
              FlagMeter=0;// modo no meter
              MAX_X=(X_SIZE+MIN_X);
              if ((Current == V_VERNIE)&&(_Det == 2)) _Det=0;
              }
 
void EnableMeter(void)
{
  u8 CounterUpdate;
  u8 i;
  
            if (Title[T_VERNIE][T1].Value>Title[T_VERNIE][T1].Limit-100) Title[T_VERNIE][T1].Value=Title[T_VERNIE][T1].Limit-100;
            if (Title[T_VERNIE][T2].Value>Title[T_VERNIE][T2].Limit-100) Title[T_VERNIE][T2].Value=Title[T_VERNIE][T2].Limit-100;
            Clear_Meter_Area();
            FlagMeter=1;
            MAX_X=(300 + MIN_X);
            Update=1;
            for(CounterUpdate=0;CounterUpdate<9;++CounterUpdate){ 
              Meter[CounterUpdate].Flag |= UPDAT;
            }
            Title[V_VERNIE][2].Flag |= UPDAT; 
            Title[T_VERNIE][2].Flag |= UPDAT;
            Title[V_VERNIE][3].Flag |= UPDAT; 
            Title[T_VERNIE][3].Flag |= UPDAT;
            Title[BK_LIGHT][0].Flag |= UPDAT; 
            Title[VOLUME][0].Flag |= UPDAT; 
            Title[BK_LIGHT][1].Flag |= UPDAT; 
            Title[VOLUME][1].Flag |= UPDAT; 
            Display_Meter();
            for(i=0; i<9; ++i)  Display_Value(i);
           

}         
/*********************************  END OF FILE  ******************************/
