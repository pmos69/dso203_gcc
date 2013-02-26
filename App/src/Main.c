	/********************* (C) COPYRIGHT 2010 e-Design Co.,Ltd. ********************
 File Name : Main.c  
 Version   : DS203_APP Ver 2.5x                                  Author : bure
*******************************************************************************/
#include "Interrupt.h"
#include "Function.h"
#include "Calibrat.h"
#include "Process.h"
// #include "Ident.h"
#include "Draw.h"
#include "BIOS.h"
#include "Menu.h"
#include "File.h"

/*******************************************************************************
Version changes
APP V2.30: since this version no longer to compatible PCB_V2.6 the following version of the motherboard
           Since this version is no longer compatible SYS_V1.31 the following version
           Sleep timing was revised to 600 seconds (Main.c)
           Modify, and add a new SYS library functions (BIOS.s)
           Modify the boot information display program (Main.c)
APP V2.31: an increase of boot identify FPGA load configuration completion discriminant (Main.c)
           Increase the Licence rights management functionality Demo program example (Ident.c, Main.c),,
           Modified analog channel correction function entry and exit operations (calibrat.c)
           Increase the the 144MHz alternately sampling mode function (Process.c)
APP V2.32 from the version from the IAR 4.42 and version 5.0 can be used in parallel
           Source did not change, the increase of the folder IAR_V5_Prpject
APP V2.33 modified scan <1uS, display the refresh BUG (Process.c,)
           Modified in the calibration state, the operation message BUG (Calibrat.c)
APP V2.34 changed by channel separate calibration (Calibrat.c & Main.c,)
           Modify the calibration mode of operation (option Calibrat.c)
APP V2.35 modified in the calibration process BUG (Calibrat.c),
           Modified, <5uS scan, the suspension can not BUG (Process.c)
           To optimize the display data handler (Process.c)
           Increase of the analog channel automatic zero balance function (Main.c, Process.c, Calibrat.c)
APP V2.36 part of the calibration operation into automatic mode (Calibrat.c, Process.c, Function.c)
           Modify the boot loader to the operating parameters (Main.c)
APP V2.37 to further improve and optimize the display data handler (Process.c)
           Modify the 32-bit signed and unsigned integer conversion process rounded BUG (Function.c,)
           Increase the pulse width duty cycle of the time and frequency measurement function (Process.c, Menu.c)
APP V2.40 increase write U disk to create the file function (Main.c, and Flies.c dosfs.c)
           Modify the save file number is displayed when BUG (Menu.c)
APP V2.41 increase the file format for the BUF's read / write sample buffer data files (Main.c, Flies.c, Menu.c)
           Increased the file format for the CSV export sample buffer data files (Main.c Flies.c, Menu.c)
APP V2.42 for space-saving file system to SYS_V1.40 module (ASM.s, Flies.c, dosfs.c)
           Changed use "SerialNo.WPT" file is stored parameter table (Flies.c)
           Note: the APP V2.42 or later must be used in conjunction with the SYS V1.40 or later
APP V2.43 modify the adjustment of the analog channels stalls BUG (Main.c),
APP V2.44 modified to save the parameters in the calibration operation BUG (Calibrat.c),
           Increase the power load parameters, the success of Tips (Main.c)
APP V2.45 modified to read and write BUF file recovery Display the corresponding menu BUG (Files.c)
           Delete the read test information when the BUF file feedback (Main.c)
APP V2.50 rewrite based on the new FAT12 file system, file read and write procedures (Files.c, ASM.s)
           Changes to TH, TL measurements the display BUG (Menu.c,)
           Optimized with dimensionless values ??show the correlation function (Menu.c Function.c, Calibrat.c)
           Modify the pulse width trigger the BUG (Process.c)
APP V2.51 modify Vmin and Vmax, Vpp measured BUG (Process.c)
*******************************************************************************/

#define APP_VERSION       "     GCC v1.28 APP Community Edition"

u8 OldMode;
u16 OldPosi;
u8 OldMeter;
u8 FlagFrameMode;
u8 FlagMeter;
u8 TrgAuto;
u8 TrgAutoOld;
u16 OldTrack1X;
u16 OldTrack2X;
s16 PreviousTrigSelect[4];
u8  OldTrigSource;
u8  LastSelected=0;
u8  XposFlag=0;
u8  C_Dmeter[8]={7,8,9,10,11,7,10,11};
u8  UpdateMeter=4;
extern u8 _vectors[];

uc16 TrigReset[27]=   {300, 150, 100, 75, 35, 15, 10, 10, 5, 3,   	//for auto trig mode, set time to reset after loosing data		
                        2,      2,   2,   2,  2,  2,  2, 2, 2, 2,    			
                        2,      2,   2,   2,  2,  2,  2 };

u8 ShowFFT = 0;						
						
/*******************************************************************************
  main : Main routine.
*******************************************************************************/
int main(void)
{ 
  s32 i;
  u16 Count_FPS = 0, Second = 0;//,Offset, Result 
  u8  CounterUpdate;			
  u8 j;
  u8 k;
  u16 TmpVT;
  u16 Vt1Old=0;
  u16 Vt2Old=0;
  u16 _Vt1Old;
  u16 _Vt2Old;
  u16 loopcount=0;

  NVIC_SetVectorTable(NVIC_VectTab_FLASH, (u32)&_vectors);
  
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
  Load_Attr();                                 // assignment Y_Attr
  i = Load_Param(); 
  if(i == 0)  // read the default boot parameters
    __Display_Str(6*8, 30, GRN, PRN, "    Reloaded parameters from disk     ");
  else       
    __Display_Str(6*8, 30, YEL, PRN, "     Parameter record not found       ");  

  
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
  _Vt2Old=_Vt2;
  TrgAutoOld=TrgAuto;
//-------------------------------------------------------------------------- ************** LOOP MAIN *****************
  while (1){
  
	//-------------------------------------------------------------------------- Gestione Entra e Uscita da Modalità X Y
    if (_Mode!=OldMode)
    {
      if (_Mode ==X_Y ){       // entra in XY_S 
       BackGround_Reset();
        if (OldMode<5){        //4
          OldTrack1X=Title[TRACK1][POSI].Value;
          OldTrack2X=Title[TRACK2][POSI].Value;
        }
        Title[TRACK1][POSI].Value=100;			// ? gets reset below...
        Title[TRACK2][POSI].Value=100;
        OffsetX=135;
        OffsetY=100;
        OldMeter=FlagMeter;
        if (FlagMeter==0) EnableMeter();
        _Vt1Old=_Vt1;
        _Vt2Old=_Vt2;
	  Vt1Old=0;
	  Vt2Old=0;
        TrgAutoOld=TrgAuto;
        TrgAuto=2;
      }
     
      //if ((_Mode<5) &&(OldMode>4)){     // esce da XY_S o XY_A
	  if ((_Mode<5 ) &&(OldMode>4 )){     // esce da XY_S o XY_A
        Title[TRACK1][POSI].Value = OldTrack1X;
        Title[TRACK2][POSI].Value = OldTrack2X;
        BackGround_Reset();
        if (OldMeter==0)  DisableMeter();
        if (OldMode==X_Y){       // se era in XY_S ripristina il trigger
        _Vt1=_Vt1Old;
        _Vt2=_Vt2Old;
        TrgAuto=TrgAutoOld;
        }
      }
    }
   
    OldMode=_Mode;
    if (_Mode==X_Y)  Title[TRACK1][POSI].Value=101+MIN_Y;
    if (_Mode==X_Y)  Title[TRACK2][POSI].Value=100;


    if (Title[TRIGG][SOURCE].Value != OldTrigSource){
      if ((Title[TRIGG][SOURCE].Value == TRACK3)||(Title[TRIGG][SOURCE].Value == TRACK4)){
        if (OldTrigSource<2)TrgAutoOld=TrgAuto;
        TrgAuto=2;
	  OldTrigSource=Title[TRIGG][SOURCE].Value;
        Update=1;
      }else{
        if (OldTrigSource>1)TrgAuto=TrgAutoOld;
	  OldTrigSource=Title[TRIGG][SOURCE].Value;
        Update=1;
      }
    }
	
	if ((_4_source==FFT_A) || (_4_source==FFT_B))
		{ ShowFFT=1;}
	else
		{ ShowFFT=0;}
	
    //-------------------------------------------------------------------------- ********* LIFE *************    
    if (__Get(USB_POWER)>0) PD_Cnt = 600;	    //don't time out into standby on external power
    if(PD_Cnt == 0){               
      __Set(BACKLIGHT, 0);                    // turn off the backlight
      __Set(STANDBY, EN);                     // enter low power states
    } else {
      Synchro();                              // simultaneous display of waveform data of each track
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
        if (FlagMeter==1) { for(i=0; i<9; ++i)  Display_Value(i);} // refresh the measured values ??per second
      }
      if (FlagMeter==1) Display_Meter();
      Display_Title();				    	

    if ((TrgAuto>0)&&(_Mode!=SCAN)&&(_Mode!=AUTO)&&(_Mode!=SGL)){  
      if(__Get(FIFO_START)== 0){			                   //check to see if not triggered w/auto trig on
        loopcount++;							
        if (loopcount>TrigReset[_T_base]){			       //after proper wait time, re-initialize trig	
          _Vt1=_1_posi;							       // set trig at signal zero point in hope to get device to trigger
          _Vt2=_2_posi;
          Update_Trig();
          if (_Mode!=X_Y) Update_Mark();
          loopcount=0;
        }
      }else loopcount=0;
    }else loopcount=0;


      if(Update){                             // handle button to refresh item
	  if (TrgAuto==0)  Print_Str(365,  216, ((SCRN<<8)+Title[TRIGG][SOURCE].Value), PRN, "!Man!");
        if (TrgAuto==1)  Print_Str(365,  216, ((SCRN<<8)+Title[TRIGG][SOURCE].Value), INV, "!1/4!");
        if (TrgAuto==2)  Print_Str(365,  216, ((SCRN<<8)+Title[TRIGG][SOURCE].Value), INV, "!1/2!");
        if (TrgAuto==3)  Print_Str(365,  216, ((SCRN<<8)+Title[TRIGG][SOURCE].Value), INV, "!3/4!");
        Update_Range();
        Update_Base();
        Update_Output();
        Update_Trig();
        //if (_Mode!=X_Y) Update_Mark();
		if ((_Mode!=X_Y)) Update_Mark();
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
      _State.Value = 2; 
      _State.Flag |= UPDAT;
    }
   
    //--------------------------------------------------------------------------  TRIGGER AUTOMATICO 
    if  ((TrgAuto>0)&&(_Mode!=SCAN)){                                                                   
      if ((Current == TRIGG) && (_Det==2)){ Title[Current][_Det].Flag |= UPDAT;_Det=0;}
      if (Title[TRIGG][SOURCE].Value == TRACK1){
        TmpVT=a_Min+Ka1[_A_Range]+((a_Max-a_Min)*(TrgAuto*2)/8);
        if ((TmpVT>Vt1Old +5) || (TmpVT<Vt1Old -5)){ _Vt1=TmpVT;Update=1;Vt1Old=_Vt1;} 
      }
      if (Title[TRIGG][SOURCE].Value == TRACK2){
        TmpVT=b_Min+Kb1[_B_Range]+((b_Max-b_Min)*(TrgAuto*2)/8);
        if ((TmpVT>Vt2Old +5) || (TmpVT<Vt2Old -5)){ _Vt2=TmpVT;Update=1;Vt2Old=_Vt2;} 
      }
    }
    //--------------------------------------------------------------------------  FRAME MODE
    
    if (FlagFrameMode==1){										//short buffer mode
          if (_Mode==SCAN) {
              if (_T_base>9) _X_posi.Value=150; else _X_posi.Value=0;                             
              FrameMode=1;
          } 
          else 
          {
             if (_Mode!=SGL) _X_posi.Value=0;
             if (Title[T_BASE][1].Value<30) {FrameMode=2; } //  was Value < 11
             else { FrameMode=0;} //0
          }
    }
    else   													//if in long buffer mode
    {
      if ((_Mode==SCAN)&&(_T_base>9)){
          if (XposFlag==0){
            _X_posi.Value=150;									// shift trigger "splice" to left of screen
            if (XposFlag==0)Update=1;							
            XposFlag=1;
          }                                                         
      }else if (XposFlag==1){			                        
        if (_Mode==SCAN)_X_posi.Value=0;
        XposFlag=0;
        Update = 1;
      }else if (_Mode==SCAN){
        if (_X_posi.Value>0){
          _X_posi.Value=0;
          Update = 1;
        }
      }
      FrameMode=0;
    }


   //--------------------------------------------------------------------------  GESTIONE TASTI MENU	
    if(Key_Buffer) { 
      if(PD_Cnt == 0)  App_init();          // exit the power saving state
      PD_Cnt = 600;                         // 600 seconds
	  //--------------------------------------------------------------------------------------------------------------

	   for (i=0;i<4;i++){												//clear memorized trigger settings of ch if ch is turned off
 	     if (Title[i][SOURCE].Value == HIDE){
		 if (PreviousTrigSelect[0]==i)PreviousTrigSelect[0]=0;			      
         	 for (j=1;j<4;j++){										
		   if (PreviousTrigSelect[j]==i) for (k=j;k>0;k--) PreviousTrigSelect[k]=PreviousTrigSelect[k-1];
	       }
	     }
	   }
	//---------------------------------------------------------------------------------------------------------------------

    
      if(Key_Buffer == KEY1){

      Delay_Cnt = 500;
    while (Delay_Cnt > 0)
    {
       if((__Get(KEY_STATUS)& KEY1_STATUS)!=0){

        _State.Flag |= UPDAT;                          						// set the corresponding update flag
        if((Current == FILE)&&(_Curr[2].Value == BUF)) reset_parameter();

        if(_Mode == SGL){
	    if(_State.Value > 0){										// going from HOLD to RUN
             for(i=0; i<4*X_SIZE; ++i)  TrackBuff[i] = 0; 						// clear the old waveform
		 __Set(FIFO_CLR, W_PTR);
          } else   __Set(FIFO_CLR, W_PTR);                      					// FIFO write pointer reset
        }
        _State.Value = (_State.Value == 0)? 1 : 0;       						// "RUN/HOLD" state swap

        if ((_Mode !=SGL)&&(_State.Value == 0)) for(i=0; i<4*X_SIZE; ++i)  TrackBuff[i] = 0; 
	  break;
	 }				//if get key status
	}				//while

      if(Delay_Cnt == 0){ 

	  //ShowFFT = (ShowFFT == 0)? 1 : 0;                   // Toggle show/hide FFT
	  
   	 _Curr[_Det].Flag |= UPDAT;
 
	 switch (Current){	
	   case TRACK1:
           if (_Det==RANGE){
              Current = TRACK2;
		  _Det = RANGE;
		  if  (Title[TRACK2][SOURCE].Value == HIDE)  Title[TRACK2][SOURCE].Value = CH_B;
		  LastSelected=2;
           }else{
              Current = TRACK1;
		  _Det = RANGE;
		  if  (Title[TRACK1][SOURCE].Value == HIDE)  Title[TRACK1][SOURCE].Value = CH_A;
		  LastSelected=1;
	     }	
              _Curr[_Det].Flag |= BLINK;
		  _Curr[_Det].Flag |= UPDAT;
		  break;
	   case TRACK2:
           if (_Det==RANGE){
              Current = TRACK1;
		  _Det = RANGE;
		  if  (Title[TRACK1][SOURCE].Value == HIDE)  Title[TRACK1][SOURCE].Value = CH_A;
		  LastSelected=1;
           }else{
              Current = TRACK2;
		  _Det = RANGE;
		  if  (Title[TRACK2][SOURCE].Value == HIDE)  Title[TRACK2][SOURCE].Value = CH_B;
		  LastSelected=2;
	     }	
       } //switch

	    if ((Current!=TRACK1)&&(Current!=TRACK2)){	
            if ((Title[TRACK1][SOURCE].Value != HIDE)&&(Title[TRACK2][SOURCE].Value == HIDE))			//Cha A on B off
            {
              Current = TRACK1;
		  _Det = RANGE;
		  LastSelected=1;
		} else if ((Title[TRACK1][SOURCE].Value == HIDE)&&(Title[TRACK2][SOURCE].Value != HIDE))		//Cha B on A off
	 	{
              Current = TRACK2;
		  _Det = RANGE;
		  LastSelected=2;
		} else 															//both on or off
		{
		  switch (LastSelected)
		  {
		  case 0:
		  case 1:
                Current = TRACK1;
    		    _Det = RANGE;
		    if  (Title[TRACK1][SOURCE].Value == HIDE)  Title[TRACK1][SOURCE].Value = CH_A;
		    LastSelected=1;
                break;
		  case 2:
                Current = TRACK2;
    		    _Det = RANGE;
		    if  (Title[TRACK2][SOURCE].Value == HIDE)  Title[TRACK2][SOURCE].Value = CH_B;
		    LastSelected=2;
		  } 			//switch
            }			//if title
          }				//if current
	    Title[TRACK1][SOURCE].Flag |= UPDAT;	
	    Title[TRACK2][SOURCE].Flag |= UPDAT;
          _Curr[_Det].Flag |= BLINK;
	    _Curr[_Det].Flag |= UPDAT;

        }				//if delay count
     } 				//if key buffer
      //--------------------------------------------------------------------------------------------------------------
      if(Key_Buffer== KEY2){                                                    // ===--- TASTO 2 PREMUTO ---===   XXXXXXXXXXXXXXXXXXXXXXXXXXXXX
        Delay_Cnt = 3000;                                                       // Carica il temporizzatore del tasto permuto
        while (Delay_Cnt > 2500){                                                  // Se il tempo non è arrivato a 0
          if((__Get(KEY_STATUS)& KEY2_STATUS)!=0){                              // CAMBIO MODO TRIGGER
           if ((Title[TRIGG][SOURCE].Value != TRACK3)&&(Title[TRIGG][SOURCE].Value != TRACK4)){
        	_Curr[_Det].Flag |= UPDAT;
            if (TrgAuto==0){
		  TrgAuto=2;
		  Vt1Old=0;
		  Vt2Old=0;
		}else {
             TrgAuto++;					  // AUTO TRIGGER
             }          
            if (TrgAuto>3) {TrgAuto=1;}							  // short press
            Title[Current][_Det].Flag |= UPDAT;
           } 
            break;
          }//if get key
        }//while

	  while ((Delay_Cnt >0)&&(Delay_Cnt <=2500)){					// long press
          if((__Get(KEY_STATUS)& KEY2_STATUS)!=0){
           if ((Title[TRIGG][SOURCE].Value != TRACK3)&&(Title[TRIGG][SOURCE].Value != TRACK4)){
         	_Curr[_Det].Flag |= UPDAT;
		if (TrgAuto>0) TrgAuto=0;
		Current = TRIGG;
		_Det=2;
            Title[Current][_Det].Flag |= UPDAT;
           }
          break;
          }
        }
        if((Delay_Cnt == 0) && ((Current == TRACK1)||(Current == TRACK2)) )Calibrat(Current);  // CALIBRAZIONE extra long press
      }
      //--------------------------------------------------------------------------------------------------------------     
      if(Key_Buffer== KEY3){                                                    // ===--- TASTO 3 PREMUTO --====
        Delay_Cnt = 1500;                                                       // Carica il temporizzatore del tasto permuto
        while (Delay_Cnt > 0){                                                  // Se il tempo non è arrivato a 0
          if((__Get(KEY_STATUS)& KEY3_STATUS)!=0){ 
            if (FlagMeter==1)  
            {
              //if ((_Mode != X_Y)  &&  (_Mode!=X_Y_A)){
              if (_Mode != X_Y){
                DisableMeter();
              }
              break;
            }
            else
            {
                EnableMeter();
              break;       
            }
              
          }

          if (Delay_Cnt == 0) {   
            Save_Param();                             // save the current operation to set parameters
            if(Current != FILE){
              for (j=0; j<2; j++) {
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
                                                    // ===--- TASTO 4 PREMUTO --====
						//Key 4 short press has been swapped with center press left toggle
if(Key_Buffer== KEY4){

    Delay_Cnt = 500;
    while (Delay_Cnt > 0)
    {
       if((__Get(KEY_STATUS)& KEY4_STATUS)!=0){

	    _Curr[_Det].Flag |= UPDAT;
          if ((Current == TRACK3)&&(_Det == 0))_Det = 2;
            if ((Current == TRACK4)&&(_Det == 0))_Det = 2;
       
        
           if ((FlagMeter!=0)||(Current != V_VERNIE)|| (_Det!=V2))
		{
			if(Current < METER_0){                         // change the Detail
			  _Curr[_Det].Flag |= UPDAT;
			  if(_Det < 3)    _Det += 1;
			  else            _Det  = 0;
			  
			  if ((Current == OUTPUT) && (Title[OUTPUT][KIND].Value==PWM) && (_Det==OUTATT)) _Det=0;
			  if ((Current == OUTPUT) && (Title[OUTPUT][KIND].Value!=PWM) && (_Det==DUTYPWM)) _Det+=1;  
			  
			  if(_Curr[_Det].MARK & NOT)                        _Det  = 0; 
			  if(_Curr[_Det].MARK & NUM2)                       _Det  = 0; 
			  if((_Curr[_Det].MARK & NUM3)&&(Current != FILE)&& (Current != OUTPUT) )  _Det  = 0;  
			  _Curr[_Det].Flag |= BLINK;
			} else {                                       // change the measurement object
			  Meter[Current-METER_0].Flag |= UPDAT;
			  if(Meter[Current-METER_0].Track <=  TRACK4) 
				Meter[Current-METER_0].Track += 1;
			  if(Meter[Current-METER_0].Track > TRACK4) 
				Meter[Current-METER_0].Track  = TRACK1;
			}
	      } else {
			_Curr[_Det].Flag |= UPDAT;
			_Det=V1;
		}
		break;
          }                     //if get key status
        }                      //while delay count >0

          if(Delay_Cnt == 0){

		_Curr[_Det].Flag |= UPDAT;

		if (Current==TRACK1)LastSelected=1;
		if (Current==TRACK2)LastSelected=2;
            Current = T_BASE;
		_Det = BASE;
            _Curr[_Det].Flag |= BLINK;
		_Curr[_Det].Flag |= UPDAT;
          }
	 }			      //if key buffer
//----------------------------------------------------------------------------------------------------------------
      if(Key_Buffer == K_INDEX_DEC){                                            // ===--- TASTO 5 --- PREMUTO --====
	    
        //if ((_Mode == X_Y) || (_Mode==X_Y_A)){
        if (_Mode == X_Y){
           if((Current == TRACK1)&&(_Det == POSI) && (OffsetX>45)) OffsetX=OffsetX-2;
           if((Current == TRACK2)&&(_Det == POSI) && (OffsetY>30)) OffsetY=OffsetY-2;
        }  
 
        if ((Current == T_VERNIE) || ((Current == V_VERNIE)&& (FlagMeter==0))) TimedDeltaView=150;
		
        if(Current < METER_0){

			if ((Current==TRACK4)&&(_Det == SOURCE)&&(Title[TRACK4][SOURCE].Value == 0)){ //About to go to FFT mode -> Change to full buffer
				if (FlagFrameMode==1){
					FlagFrameMode=0;
					if (_Mode!=SCAN) _X_posi.Value=OldPosi; //Fronte negativo esce dal frame mode ripristina posizione
					Update_View_Area();
				}
			}
		
          for (i=0;i<4;i++){
			if ((Current==i)&&(_Det == SOURCE)){							//ch selected and submenu on source
				if (Title[i][SOURCE].Value == 1){								//about to be shut down
					if (Title[TRIGG][SOURCE].Value ==i){						      //if trig selected
						Title[TRIGG][SOURCE].Value = PreviousTrigSelect[3];  				
						for (j=3;j>0;j--) PreviousTrigSelect[j]=PreviousTrigSelect[j-1];
						PreviousTrigSelect[0]=0;
						Title[TRIGG][0].Flag=UPDAT;							
						Title[TRIGG][1].Flag=UPDAT;							
						Title[TRIGG][2].Flag=UPDAT;							
						Update_Trig();						
					}												//if not selected, about to be shut down, purge # from buffer
					if (PreviousTrigSelect[0]==i)PreviousTrigSelect[0]=0;			//clear to default (ch1) first entry if last ch memorized shut down
					for (j=1;j<4;j++){
						if (PreviousTrigSelect[j]==i) for (k=j;k>0;k--) PreviousTrigSelect[k]=PreviousTrigSelect[k-1];
					}
					
				}    
			}
          }

          if((Current == TRIGG)&&(Detail[Current]==2)){         // trigger level adjustment
            if(V_Trigg[_Trigg[SOURCE].Value].Value > MIN_Y+4) 
              V_Trigg[_Trigg[SOURCE].Value].Value--;
          } else if((Current == BK_LIGHT)||(Current == VOLUME)){//  adjust backlight or volume
            if(_Curr[1].Value > 0)   _Curr[1].Value--;
          } else if((Current == T_BASE)&&(_Det == XPOSI)&&((FlagFrameMode==0)||(_Mode == SGL))){      // X_POSI adjustment  
            if(_Curr[_Det].Value > 30) _Curr[_Det].Value -= 30; 
            else if(_Curr[_Det].Value > 0) _Curr[_Det].Value--;
            _X_View.Flag |= UPDAT;                              // refresh X_View
          } else {                                              //  for other current item
		if ((Current==TRACK1)&&(_Det==SOURCE)) LastSelected=1;  //ChA is about to be turned on
		if ((Current==TRACK2)&&(_Det==SOURCE)) LastSelected=2;  //ChB is about to be turned on
            if(_Curr[_Det].Value > 0) _Curr[_Det].Value--; 
            else if(_Curr[_Det].MARK & CIRC) _Curr[_Det].Value =_Curr[_Det].Limit;
          }
          if((Current == T_BASE)&&(_Det == MODE)){     // T_BASE MODE selection
            if ((_Mode==SCAN)&&(FrameMode==0)) OldPosi=_X_posi.Value;
            if ((_Mode==X_Y)&&(FrameMode==0)) _X_posi.Value=OldPosi;
            Title[RUNNING][STATE].Value = RUN;         // STATE = RUNNING 
            Title[RUNNING][STATE].Flag |= UPDAT;       // refresh RUNNING STATE
          }
          if((Current == OUTPUT)&&(_Kind != PWM)&&(_Kind != NOOUT)){
             if (Title[OUTPUT][FRQN].Value > 14) 
            Title[OUTPUT][FRQN].Value = 14;            // upper limit of the analog signal frequency is 50KHz
           } 
          if((Current == FILE)&&(_Curr[0].Value == LOAD)){ 
            if(_Curr[2].Value == BMP) _Curr[2].Value = BUF;  // only the Load Dat, Buf file
            if(_Curr[2].Value == CSV) _Curr[2].Value = BUF;  // only the Load Dat, Buf file
          }
          _Curr[0].Flag |= UPDAT;
          _Curr[1].Flag |= UPDAT;
          _Curr[2].Flag |= UPDAT;
          _Curr[3].Flag |= UPDAT;
        } else {
          Meter[Current-METER_0].Flag |= UPDAT;
          if(Meter[Current-METER_0].Item  > VBT) 
            Meter[Current-METER_0].Item -= 1;          // change the measurement items
          else                     
            Meter[Current-METER_0].Item  = TL;//MIN;
          if(Meter[Current-METER_0].Item == FPS) 
            Meter[Current-METER_0].Track = 4;
          if(Meter[Current-METER_0].Item == TL)//MIN) 
            Meter[Current-METER_0].Track = 0;
        }
      }
      //---------------------------------------------------------------------------------------------------------------------       
      if(Key_Buffer == K_INDEX_S)
	{                                              // ===--- TASTO 5 centrale PREMUTO --====
						//center press left toggle has been swapped with key 4 short press
 	    if ((Current != FILE)) {

		Delay_Cnt = 500;
      	while (Delay_Cnt > 0){
                                                
            if((__Get(KEY_STATUS)& K_INDEX_S_STATUS)!=0){

          		for (i=0;i<4;i++){											
                    if (Current==i){									//================================================================
			    if (Title[i][SOURCE].Value != HIDE){
				 if (Title[TRIGG][SOURCE].Value !=i){						//if not already selected
	   		         for (j=0;j<3;j++) PreviousTrigSelect[j]=PreviousTrigSelect[(j+1)];	//lifo stack
		               PreviousTrigSelect[3]=Title[TRIGG][SOURCE].Value;				//save previous
                           Title[TRIGG][SOURCE].Value = i;							//set trig to selected track
				   Title[TRIGG][0].Flag=UPDAT;							
	     			   Title[TRIGG][1].Flag=UPDAT;							
	                     Title[TRIGG][2].Flag=UPDAT;							
				   Update_Trig();						
				 }
		          }  
	              }
                  }
          break;
          }					// if keystatus
	}
          if(Delay_Cnt == 0){
 	      if (FlagMeter==1){
                 if (UpdateMeter<4){
                  UpdateMeter=UpdateMeter+1;
                  if  ((_Mode==X_Y)&&(UpdateMeter>1)) UpdateMeter=4;
                }
                else{
                  UpdateMeter=0;
                } 
                switch (UpdateMeter){
                case 0:
                case 1:
                  if (UpdateMeter==1) {
                    _D_V_Source.Value=1;
                  }
                  else	// originally included case 2,3...											
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
                case 2:
                  Meter[0].Flag |= UPDAT;
                  Meter[0].Item  =  VBT;
                  Meter[0].Track = 4;
                  for(CounterUpdate=1;CounterUpdate<9;++CounterUpdate){ 
                    Meter[CounterUpdate].Flag |= UPDAT;
                    Meter[CounterUpdate].Item  = C_Dmeter[CounterUpdate-1]; // selezione item
			  if (CounterUpdate<6)Meter[CounterUpdate].Track = 2; else Meter[CounterUpdate].Track = 3;
                  }
			break;
                case 3:
                  Meter[0].Flag |= UPDAT;
                  Meter[0].Item  =  VBT;
                  Meter[0].Track = 4;
                  for(CounterUpdate=1;CounterUpdate<9;++CounterUpdate){ 
                    Meter[CounterUpdate].Flag |= UPDAT;
                    Meter[CounterUpdate].Item  = C_Dmeter[CounterUpdate-1]; // selezione item
			  if (CounterUpdate<6)Meter[CounterUpdate].Track = 3; else Meter[CounterUpdate].Track = 2;
                  }
                 break;
                case 4:
                  
                  Meter[0].Flag |= UPDAT;
                  Meter[0].Item  =  VBT;
                  Meter[0].Track = 4;  
                  for(CounterUpdate=1;CounterUpdate<9;++CounterUpdate){    
                    Meter[CounterUpdate].Flag |= UPDAT;
                  }
                  if ((_Mode==X_Y) || ( (Title[TRACK3][SOURCE].Value == HIDE) && (Title[TRACK4][SOURCE].Value == HIDE)))
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
                    Meter[1].Item  =  FRQ; Meter[1].Track = 0;
                    Meter[2].Item  =  VDC; Meter[2].Track = 0;
                    Meter[3].Item  =  FRQ; Meter[3].Track = 1;
                    Meter[4].Item  =  VDC; Meter[4].Track = 1;
                    Meter[5].Item  =  FRQ; Meter[5].Track = 2;
                    Meter[6].Item  =  CIR; Meter[6].Track = 2;
                    Meter[7].Item  =  FRQ; Meter[7].Track = 3;
                    Meter[8].Item  =  CIR; Meter[8].Track = 3;
                  }
                  break;
                }				// switch
              }else{			// flagmeter
                if (CalFlag==0){
                  CalFlag=1;
              	for (j=0; j<2; j++) {
              	  Print_Str(91, 0, 0x0405, PRN, "      CALIBRATION ON       ");
              	  Delayms(400);
              	  Print_Str(91, 0, 0x0405, PRN, "                           ");
              	  Delayms(200);
              	}
                  Print_Str(137,216,0x0405,PRN, "!C!");
                }else{
                  CalFlag=0;
              	for (j=0; j<2; j++) {
                	  Print_Str(91, 0, 0x0405, PRN, "      CALIBRATION OFF      ");
                	  Delayms(400);
                	  Print_Str(91, 0, 0x0405, PRN, "                           ");
                	  Delayms(200);
                  }
                  Print_Str(137,216,0x0405,PRN, "Un");
                }
             }
	    }					// delay cnt=0
        }          		      // if current

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
      if(Key_Buffer == K_INDEX_INC){                                            // ===--- TASTO 5 +++ PREMUTO --====
        
		if (_Mode == X_Y){
           if((Current == TRACK1)&&(_Det == POSI) && (OffsetX<215)) OffsetX=OffsetX+2;
           if((Current == TRACK2)&&(_Det == POSI) && (OffsetY<170)) OffsetY=OffsetY+2;
         }  
     
        if ((Current == T_VERNIE) || ((Current == V_VERNIE) && (FlagMeter==0))) TimedDeltaView=150;
        if ((TrgAuto>0)&&(Current == TRIGG) && _Det==2) _Det=0;
        if(Current < METER_0){
		
			if ((Current==TRACK4)&&(_Det == SOURCE)&&(Title[TRACK4][SOURCE].Value >= 9)){ //About to go to FFT mode -> Change to full buffer
				if (FlagFrameMode==1){
					FlagFrameMode=0;
					if (_Mode!=SCAN) _X_posi.Value=OldPosi; //Fronte negativo esce dal frame mode ripristina posizione
					Update_View_Area();
				}
			}
		
			for (i=0;i<4;i++){							//tracks 1 through 4
				if ((Current==i)&&(_Det == SOURCE)){
					if (Title[TRIGG][SOURCE].Value ==i){						      //if trig selected
						switch (Current){
							case TRACK1:
							case TRACK2:
								if (Title[i][SOURCE].Value == 2){
								   Title[TRIGG][SOURCE].Value = PreviousTrigSelect[3];
								   for (j=3;j>0;j--) PreviousTrigSelect[j]=PreviousTrigSelect[(j-1)];			
								   PreviousTrigSelect[0]=0;  
								}		
								break;
							case TRACK3:
								if (Title[i][SOURCE].Value == 1){
									Title[TRIGG][SOURCE].Value = PreviousTrigSelect[3];  
									for (j=3;j>0;j--) PreviousTrigSelect[j]=PreviousTrigSelect[(j-1)];
									PreviousTrigSelect[0]=0;  
								}		
								break;
							case TRACK4:
								if (Title[i][SOURCE].Value == 13){
									Title[TRIGG][SOURCE].Value = PreviousTrigSelect[3];
									for (j=3;j>0;j--) PreviousTrigSelect[j]=PreviousTrigSelect[(j-1)];
									PreviousTrigSelect[0]=0;  
								}
						}
						Title[TRIGG][0].Flag=UPDAT;							
						Title[TRIGG][1].Flag=UPDAT;							
						Title[TRIGG][2].Flag=UPDAT;
						Update_Trig();						
					}												//if not selected, about to be shut down, purge # from buffer
					if (PreviousTrigSelect[0]==i) PreviousTrigSelect[0]=0;			      //clear to default (ch1) first entry if last ch memorized shut down
					for (j=1;j<4;j++){										
						if (PreviousTrigSelect[j]==i) for (k=j;k>0;k--) PreviousTrigSelect[k]=PreviousTrigSelect[k-1];
					}  
				}
			}	

			if((Current == TRIGG)&&(Detail[Current]==2)){         // trigger level adjustment
				if(V_Trigg[_Trigg[SOURCE].Value].Value < MAX_Y-4) 
				V_Trigg[_Trigg[SOURCE].Value].Value++;
			} else if ((Current == BK_LIGHT)||(Current == VOLUME)){// adjust backlight or volume
				if(_Curr[1].Value < _Curr[1].Limit)   _Curr[1].Value++;
    	    } else if ((Current == T_BASE)&&(_Det == XPOSI) ){                         // *******>>>>>> ALT X_POSI adjustment
				if (_Curr[_Det].Value <30){ _Curr[_Det].Value ++; goto OkSlow; }//
				else if (_Curr[_Det].Value <(_Curr[_Det].Limit-30)) _Curr[_Det].Value += 30; //
				else if (_Curr[_Det].Value < (_Curr[_Det].Limit)) _Curr[_Det].Value ++;
    OkSlow:
				_X_View.Flag |= UPDAT;                      
			} else if ((Current == T_VERNIE) && ((_Det==T1) || (_Det==T2))){
				if (FlagMeter==1){
                    if (_Curr[_Det].Value <(_Curr[_Det].Limit-100)) _Curr[_Det].Value ++;
                } else {
                    if (_Curr[_Det].Value <_Curr[_Det].Limit-MIN_X-1) _Curr[_Det].Value ++; 
                }
			} else {                                              // the current item for otheR
				if ((Current==TRACK1)&&(_Det==SOURCE)) LastSelected=1;  //ChA is about to be turned on
				if ((Current==TRACK2)&&(_Det==SOURCE)) LastSelected=2;  //ChB is about to be turned on
				if(_Curr[_Det].Value < _Curr[_Det].Limit)  _Curr[_Det].Value++;
				else if(_Curr[_Det].MARK & CIRC)   _Curr[_Det].Value  = 0;
			}
			if((Current == T_BASE)&&(_Det == MODE)){     // T_BASE MODE selection
				if ((_Mode==SCAN)&&(FrameMode==0)) OldPosi=_X_posi.Value;
				if ((_Mode==AUTO)&&(FrameMode==0)) _X_posi.Value=OldPosi;
				Title[RUNNING][STATE].Value = RUN;         // STATE = RUNNING 
				Title[RUNNING][STATE].Flag |= UPDAT;       // refresh RUNNING STATE
			}
			if((Current == OUTPUT)&&(_Kind != PWM)&&(_Kind != NOOUT)){
				if(Title[OUTPUT][FRQN].Value > 14) 
					Title[OUTPUT][FRQN].Value = 14;          // upper limit of the analog signal frequency is 50KHz
			}
			if((Current == FILE)&&(_Curr[0].Value == 1)){  // only the Load Dat, Buf file
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
				Meter[Current-METER_0].Item += 1;          //  change the measurement items
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
            //if(Current == TRACK1)  {Current = VOLUME;}	//loop around
            if(Current == TRACK1)  {Current = TRACK1;}	//don't loop around
            else                  {Current --;}
          }
          else
          {
            //if(Current == TRACK1)  {Current = T_VERNIE;}  //loop around
            if(Current == TRACK1)  {Current = TRACK1;}	//don't loop around
            else                   {Current --;}
          }
          if(Current == RUNNING) Current --;               // Jump over Item 4
		  
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
          if(Current == METER_0)
			Current = METER_8;
          else
			Current --;
          Meter[Current-METER_0].Flag |= BLINK;
        }
      }
	  //---------------------------------------------------------------------------------------------------------------------
      if(Key_Buffer == K_ITEM_S){    // ===--- TASTO 6 centrale PREMUTO --====
    Delay_Cnt = 500;
    while (Delay_Cnt > 0)
    {
       if((__Get(KEY_STATUS)& K_ITEM_S_STATUS)!=0){
            if (FlagFrameMode==0){
              FlagFrameMode=1;
             if (_Mode!=SCAN) OldPosi=_X_posi.Value; //Fronte positivo entra in frame mode memorizza posizione
              }
            else
            {
              FlagFrameMode=0;
             if (_Mode!=SCAN) _X_posi.Value=OldPosi; //Fronte negativo esce dal frame mode ripristina posizione
            }															
		Update_View_Area();
	      break;
	 }				//if get key status
	}				//while

      if(Delay_Cnt == 0){
 
       _Curr[_Det].Flag |= UPDAT;
        if (FlagMeter==0){
			if (((Current <= METER_8)&& (Current >= METER_0)) || (Current==BK_LIGHT) || (Current==VOLUME) ){
				Current=TRACK1 ;
			}
        } else {
			if(Current < METER_0) {
				Current = METER_0;
			} else {
				for(CounterUpdate=0;CounterUpdate<9;++CounterUpdate){  // Udate di tutti i meter per tenterli accesi
					Meter[CounterUpdate].Flag |= UPDAT;
				}
			  Current = TRACK1;
			}
	  }
       }//delay count

     }
      //---------------------------------------------------------------------------------------------------------------  
      if(Key_Buffer == K_ITEM_INC){                                             // ===--- TASTO 6 +++ PREMUTO --====
        if(Current < METER_0){
          _Curr[_Det].Flag |= UPDAT;
          if ((FlagMeter==0) && (Current == T_VERNIE))  {
			//Current = TRACK1;				//loop around
			Current = T_VERNIE;				// dont
			goto NoInc;
		  }
          if(Current == VOLUME)					
			//Current = TRACK1;				//loop
 			Current = VOLUME;					//don't
         else
			Current ++;
        NoInc: 
          if(Current == RUNNING) Current ++;              // Jump over RUNNING
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

              if ((UpdateMeter==4)&&(FlagMeter==1)){
                  Meter[0].Flag |= UPDAT;
                  Meter[0].Item  =  VBT;
                  Meter[0].Track = 4;  
                  for(CounterUpdate=1;CounterUpdate<9;++CounterUpdate){    
                    Meter[CounterUpdate].Flag |= UPDAT;
                  } 
                  if ((_Mode==X_Y) || ( (Title[TRACK3][SOURCE].Value == HIDE) && (Title[TRACK4][SOURCE].Value == HIDE)))
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
                    Meter[1].Item  =  FRQ; Meter[1].Track = 0;
                    Meter[2].Item  =  VDC; Meter[2].Track = 0;
                    Meter[3].Item  =  FRQ; Meter[3].Track = 1;
                    Meter[4].Item  =  VDC; Meter[4].Track = 1;
                    Meter[5].Item  =  FRQ; Meter[5].Track = 2;
                    Meter[6].Item  =  CIR; Meter[6].Track = 2;
                    Meter[7].Item  =  FRQ; Meter[7].Track = 3;
                    Meter[8].Item  =  CIR; Meter[8].Track = 3;
                  }
                 }

      Key_Buffer=0;
      Update = 1;
    }
  }
}

void DisableMeter(void)
{
              if (((Current <= METER_8)&& (Current >= METER_0)) || (Current==BK_LIGHT) || (Current==VOLUME) ){_Curr[_Det].Flag |= UPDAT;Current=TRACK1 ;}
              FlagMeter=0;// modo no meter
              MAX_X=(386+MIN_X);
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
