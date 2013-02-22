/******************** (C) COPYRIGHT 2009 e-Design Co.,Ltd. *********************
 File Name : Process.c       
 Version   : DS203_APP Ver 2.3x                                  Author : bure
*******************************************************************************/
#include "Interrupt.h"
#include "Process.h"
#include "Draw.h"
#include "Menu.h"
#include "BIOS.h"
#include "File.h"

// FFT ////////////////////////////////////////////////////////////////////
short fr[FFTSize];
short fi[FFTSize];

u32 NFreq;
char NFreqStr[12];
int imax;
short PeakFreq;
char PeakFreqStr[12];
char FreqDivStr[12];
char FreqT1Str[12];
////////////////////////////////////////////////////////////////////////////

u16 TaS, TbS, TcS, TdS;            // cycles accumulated
u16 PaS, PbS, PcS, PdS;            // pulse width of the cumulative
u16 TaN, TbN, TcN, TdN;            // Cycle Count
u8  a_Mid_H, a_Mid_L;
u8  b_Mid_H, b_Mid_L;
s8  Kab;                                     // analog channel zero balance correction factor

s32 a_Avg, b_Avg, a_Ssq, b_Ssq;              // use signed integers, allows values at bottom of screen to be read
s16  a_Max, b_Max, a_Min, b_Min;              // 0 levels can get pushed up by calibration, bringing bottom of screen below 0

s16 Posi_412,Posi_41_2, Posi_41, Posi_42, Posi_4_2, Posi_4F1, Posi_4F2, Posi_4F3, Posi_4F4;
s16 c_Max, d_Max, A_Posi, B_Posi;

u16 JumpCnt;
u8  FrameMode;
u8  HoldOnNext =0;
u16 bag_max_buf = 4096;
u8  freerun=0;					//flags for auto trig mode
u8  exitflag=0;
u8  entryflag=0;
u8  ADCoffset=54; //shifts ADC/FIFO operating area away from non-linear first xx steps into previously unused linear 200-255 step area
u16 TempKp1=1024;
u8  CalFlag;

uc16 Wait[27]=        {1000, 500, 200, 100, 65, 30, 20, 10, 5, 3,   			
                       2,      2,   2,   2,  2,  2,  2, 2, 2, 2,    			
                        2,      2,   2,   2,  2,  2,  2 };


uc16 shortwait[27]=        {300, 150, 100, 75, 35, 15, 10, 10, 5, 3,   	//for auto trig mode, set time to hold before going auto		
                        2,      2,   2,   2,  2,  2,  2, 2, 2, 2,    			
                        2,      2,   2,   2,  2,  2,  2 };


Y_attr *Y_Attr; 
X_attr *X_Attr; 
G_attr *G_Attr; 
T_attr *T_Attr;

u32 DataBuf[4096];
u8  TrackBuff  [X_SIZE * 4];         // curve track: i +0, i +1, i +2, i +3, respectively, placed one on the 4th track data

s8  Ka1[10] ={   0,    0,    0,    0,    0,    0,    0,    0,    0,    0}; // A channel low error correction coefficient
s8  Kb1[10] ={   0,    0,    0,    0,    0,    0,    0,    0,    0,    0}; // B channel low error correction coefficient
u16 Ka2[10] ={1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024}; // A channel gain error correction coefficient
u16 Kb2[10] ={1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024}; // B channel gain error correction factor
s8  Ka3[10] ={   0,    0,    0,    0,    0,    0,    0,    0,    0,    0}; // A channel high error correction coefficient
s8  Kb3[10] ={   0,    0,    0,    0,    0,    0,    0,    0,    0,    0}; // B-channel high bit error correction coefficient

D_tab D_Tab[23] ={  // pulse waveform output driver table, based on the 72MHz frequency
//    STR      PSC     ARR       DUTY% 
  {" !1Hz! ", 1800-1,  40000-1,     50},
  {" !2Hz! ", 1800-1,  20000-1,     50},
  {" !5Hz! ", 1800-1,   8000-1,     50},
  {" 10Hz ",  180-1,  40000-1,     50},
  {" 20Hz ",  180-1,  20000-1,     50},
  {" 50Hz ",  180-1,   8000-1,     50},
  {"!100Hz!", 180-1,   4000-1,     50},
  {"!200Hz!", 180-1,   2000-1,     50},
  {"!500Hz!", 180-1,    800-1,     50},
  {" 1KHz ",  180-1,    400-1,     50},
  {" 2KHz ",  180-1,    200-1,     50},
  {" 5KHz ",   18-1,    800-1,     50},
  {"!10KHz!",  18-1,    400-1,     50},
  {"!20KHz!",  18-1,    200-1,     50},
  {"!50KHz!",   1-1,   1440-1,     50},
  {"100KHz",    1-1,    720-1,     50},
  {"200KHz",    1-1,    360-1,     50},
  {"500KHz",    1-1,    144-1,     50},
  {" 1MHz ",    1-1,     72-1,     50},
  {" 2MHz ",    1-1,     36-1,     50},
  {" 4MHz ",    1-1,     18-1,     50},
  {" 6MHz ",    1-1,     12-1,     50},
  {" 8MHz ",    1-1,      9-1,     50}};

A_tab A_Tab[15] ={ // analog waveform output driver table synthesis, based on the 72MHz frequency, per 36

//    STR     PSC     ARR 
  {"! 1Hz !", 20-1,  50000-1},
  {"! 2Hz !", 20-1,  20000-1},
  {"! 5Hz !", 20-1,  10000-1},
  {" 10Hz ",  10-1,  10000-1},
  {" 20Hz ",  10-1,   5000-1},
  {" 50Hz ",  10-1,   2000-1},
  {"!100Hz!", 10-1,   1000-1},
  {"!200Hz!", 10-1,    500-1},
  {"!500Hz!", 10-1,    200-1},
  {" 1KHz ",  10-1,    100-1},
  {" 2KHz ",  10-1,     50-1},
  {" 5KHz ",  10-1,     20-1},
  {"!10KHz!", 10-1,     10-1},
  {"!20KHz!", 10-1,      5-1},
  {"!40KHz!",  4-1,      5-1}};


u16   ATT_DATA[72];

u16 SIN_DATA[72] = 
{2048,2226,2404,2578,2748,2914,3072,3223,3364,3496,3617,3726,3822,
3904,3972,4026,4065,4088,4095,4088,4065,4026,3972,3904,3822,3726,3617,
3496,3364,3223,3072,2914,2748,2578,2404,2226,2048,1870,1692,1518,1348,
1182,1024,873,732,600,479,370,274,192,124,70,31,8,0,8,31,70,124,192,274,
370,479,600,732,873,1024,1182,1348,1518,1692,1870,};

u16 TRG_DATA[72] =  // triangle wave data
  {0,114,228,342,456,570,684,798,912,1026,1140,1254,1368,1482,1596,1710,1824,1938,2052,
2166,2280,2394,2508,2622,2736,2850,2964,3078,3192,3306,3420,3534,3648,3762,3876,3990,4095,
3990,3876,3762,3648,3534,3420,3306,3192,3078,2964,2850,2736,2622,2508,2394,2280,2166,2052,
1938,1824,1710,1596,1482,1368,1254,1140,1026,912,798,684,570,456,342,228,114};

u16 SAW_DATA[72] =  // Sawtooth wave data                                                                                             //         
  {0,
57,112,168,224,280,336,392,448,504,560,616,672,728,784,840,896,952,1008,1064,1120,1176,1232,
1288,1344,1400,1456,1512,1568,1624,1680,1736,1792,1848,1904,1960,2016,2072,2128,2184,2240,2296,
2352,2408,2464,2520,2576,2632,2688,2744,2800,2856,2912,2968,3024,3080,3136,3192,3248,3304,3360,
3416,3472,3528,3584,3640,3696,3752,3808,3864,3920,3976};   // 360   

u16 DIGI_DATA[72] =  // Square wave data                                                                                             //         
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  4095,4095,4095,4095,4095,4095,4095,4095,4095,4095,4095,4095,4095,4095,4095,4095,4095,4095,
  4095,4095,4095,4095,4095,4095,4095,4095,4095,4095,4095,4095,4095,4095,4095,4095,4095,4095};   // 360 


void BackGround_Reset(void)
{ 
  u16 i, j;
  
  __Clear_Screen(BLACK);       
  for(i=0; i<13; i++) for(j=0; j<4; j++) Title[i][j].Flag |= UPDAT;
  for(i=0; i<9; i++)                     Meter[i].Flag    |= UPDAT;
  if(Current != FILE) {
    Title[FILE][0].Flag &= !UPDAT;
    Title[FILE][1].Flag &= !UPDAT;
    Title[FILE][3].Flag &= !UPDAT;
  }  
   Update = 1;                  // return back the jumper settings
}

/*******************************************************************************
 App_init: Displays the window waveform data initialization
*******************************************************************************/
void App_init(void)
{ 
  u16 i, j;
  __Set(ADC_CTRL, EN );       
  __Set(STANDBY, DN);          // exit the power saving state
  __Clear_Screen(BLACK);       // clear the screen
  Delayms(20); 
  __Set(FIFO_CLR, W_PTR); 
  for(i=0; i<13; i++) for(j=0; j<4; j++) Title[i][j].Flag |= UPDAT;
  for(i=0; i<9; i++)                     Meter[i].Flag    |= UPDAT;
  if(Current != FILE) {
    Title[FILE][0].Flag &= !UPDAT;
    Title[FILE][1].Flag &= !UPDAT;
    Title[FILE][3].Flag &= !UPDAT;
  }  
  PD_Cnt = 600;
  Update = 1;                  // return back the jumper settings
}
/*******************************************************************************
 _init: Displays the window waveform data initialization <<<< THIS (AND REF_WAVE) COULD BE ELIMINATED? DOES NOT SEEM TO BE USED
*******************************************************************************/
/*void View_init(void)
{ 
  u16 i, j = 0, k = 0;
  for(i = 0; i < X_SIZE * 4; i += 4){
    TrackBuff[i] = 160;
    TrackBuff[i+1] = Get_Ref_Wave(i/4);
    if((i%160)==0) j = 1 - j;
    TrackBuff[i+2] = 60 + (j * 17);
    if((i%64)==0) k = 1 - k;
    TrackBuff[i+3] = 20 + (k * 17);
  }
}*/
/*******************************************************************************
 Update_Range: 
*******************************************************************************/
void Update_Range(void) 
{
  __Set(ADC_CTRL, EN);       
  __Set(ADC_MODE, SEPARATE);                        							// Set Separate mode ((Range + 1)*25)
  __Set(CH_A_COUPLE, Title[TRACK1][COUPLE].Value);
  __Set(CH_A_RANGE,  Title[TRACK1][RANGE].Value);
  __Set(CH_A_OFFSET, (((1024 + Ka3[_A_Range])*_1_posi + 512)/1024)+ADCoffset);		//ADCoffset nulled out when reading FIFO

  __Set(CH_B_COUPLE, Title[TRACK2][COUPLE].Value);
  __Set(CH_B_RANGE,  Title[TRACK2][RANGE].Value);
  __Set(CH_B_OFFSET, (((1024 + Kb3[_B_Range])*_2_posi + 512)/1024)+ADCoffset);

  if(_Status == RUN) __Set(FIFO_CLR, W_PTR);       // FIFO write pointer reset
}
/*******************************************************************************
 Update_Base: 
*******************************************************************************/
void Update_Base(void) 
{
  u16 i;
  
  __Set(ADC_CTRL, EN);       
  i = Title[T_BASE][BASE].Value;     // independent sampling mode
  __Set(T_BASE_PSC, X_Attr[i].PSC);
  __Set(T_BASE_ARR, X_Attr[i].ARR);
  Wait_Cnt = Wait[_T_base];
  if(_Status == RUN) __Set(FIFO_CLR, W_PTR);      // FIFO write pointer reset
}
/*******************************************************************************
 Update_Output: 
*******************************************************************************/
void Update_Output(void) 
{
  
  u8 att;

  switch (_Kind) {
  
  case SINE:
    for(att=0; att <72; att++){ 
     //ATT_DATA[att]=(SIN_DATA[att]*Title[OUTPUT][OUTATT].Value)/100;}	                       //zero axis at 0, causes distortion as level is dropped
     ATT_DATA[att]=(((Title[OUTPUT][OUTATT].Value*(SIN_DATA[att]-2048))/100)+2048);}           //establish zero axis at mid of full output level
     DMA2_Channel4->CCR &= ~DMA_CCR1_EN;
      __Set(ANALOG_PSC,  A_Tab[_Frqn].PSC);								     //used with special sys version	
      __Set(ANALOG_CNT, 72);
      __Set(ANALOG_PTR, (u32)ATT_DATA);
    DMA2_Channel4->CCR |= DMA_CCR1_EN;
      __Set(ANALOG_ARR, A_Tab[_Frqn].ARR);
    
  break;
  
  case SAW:
   for(att=0; att <72; att++){ 
     //ATT_DATA[att]=(SAW_DATA[att]*Title[OUTPUT][OUTATT].Value)/100;}
     ATT_DATA[att]=(((Title[OUTPUT][OUTATT].Value*(SAW_DATA[att]-2048))/100)+2048);}           
     DMA2_Channel4->CCR &= ~DMA_CCR1_EN;
      __Set(ANALOG_PSC,  A_Tab[_Frqn].PSC);
      __Set(ANALOG_CNT, 72);
      __Set(ANALOG_PTR, (u32)ATT_DATA);
     DMA2_Channel4->CCR |= DMA_CCR1_EN;
      __Set(ANALOG_ARR, A_Tab[_Frqn].ARR);
  break;
  
  case TRIANG:
    for(att=0; att <72; att++){ 
     //ATT_DATA[att]=(TRG_DATA[att]*Title[OUTPUT][OUTATT].Value)/100;}
     ATT_DATA[att]=(((Title[OUTPUT][OUTATT].Value*(TRG_DATA[att]-2048))/100)+2048);}          
     DMA2_Channel4->CCR &= ~DMA_CCR1_EN;
      __Set(ANALOG_PSC,  A_Tab[_Frqn].PSC);
      __Set(ANALOG_CNT, 72);
      __Set(ANALOG_PTR, (u32)ATT_DATA);
    DMA2_Channel4->CCR |= DMA_CCR1_EN;
      __Set(ANALOG_ARR, A_Tab[_Frqn].ARR);
  break;
  
  case DIGI:
    for(att=0; att <72; att++){ 
     //ATT_DATA[att]=(DIGI_DATA[att]*Title[OUTPUT][OUTATT].Value)/100;}
     ATT_DATA[att]=(((Title[OUTPUT][OUTATT].Value*(DIGI_DATA[att]-2048))/100)+2048);}           
     DMA2_Channel4->CCR &= ~DMA_CCR1_EN;
      __Set(ANALOG_PSC,  A_Tab[_Frqn].PSC);
      __Set(ANALOG_CNT, 72);
      __Set(ANALOG_PTR, (u32)ATT_DATA);
    DMA2_Channel4->CCR |= DMA_CCR1_EN;
      __Set(ANALOG_ARR, A_Tab[_Frqn].ARR);
  break;

  case PWM:
    __Set(DIGTAL_PSC, D_Tab[_Frqn].PSC);
    __Set(DIGTAL_ARR, D_Tab[_Frqn].ARR);
    __Set(DIGTAL_CCR, ((D_Tab[_Frqn].ARR+1)*(100-Title[OUTPUT][DUTYPWM].Value))/100);
  break;
  
  case NOOUT:
    __Set(DIGTAL_PSC, D_Tab[_Frqn].PSC);
    __Set(DIGTAL_ARR, D_Tab[_Frqn].ARR);
    __Set(DIGTAL_CCR, (D_Tab[_Frqn].ARR+1));
  break;
  }
}

/*******************************************************************************
 Update_Trig: 
*******************************************************************************/
void Update_Trig(void) 
{ 
  if(_T1 > _T2)  __Set(T_THRESHOLD, (_T1 - _T2));  
  else           __Set(T_THRESHOLD, (_T2 - _T1)); 
  __Set(TRIGG_MODE,  (_Tr_source<< 3)+_Tr_kind);
  if(Title[TRIGG][SOURCE].Value == TRACK1){ 
  __Set(V_THRESHOLD, ((((_Vt1-Ka1[_A_Range]-_1_posi)*1024)/Ka2[_A_Range])&0xFF)+ADCoffset+_1_posi); 
  }
  if(Title[TRIGG][SOURCE].Value == TRACK2){ 
  __Set(V_THRESHOLD, ((((_Vt2-Kb1[_B_Range]-_2_posi)*1024)/Kb2[_B_Range])&0xFF)+ADCoffset+_2_posi); 
 }
  if(_Status == RUN) __Set(FIFO_CLR, W_PTR);      // FIFO write pointer reset
}
/*******************************************************************************
 Process: Calculate processing buffer data
*******************************************************************************/
void Process(void)
{ 
  s16 i, j = 0, k, V[8] = {0}, n = 0;
  s32 Tmp;
  u8  Ch[4], C_D=0,discard;
  s8  Sa = 2, Sb = 2, Sc = 2, Sd = 2; // time status
  u16 Ta, Tb, Tc, Td;                 // pulse width count
  u16 Pa, Pb, Pc, Pd;			  // holds + side transition of wave for PaS
  u8  jj;
  u16 h;
  u32 swap; 

  int X,Y;
  
  Ta = Tb = Tc = Td = 0;
  Pa = Pb = Pc = Pd = 0;
  
  PaS = 0; PbS = 0; PcS = 0; PdS = 0; 
  TaN = 0; TbN = 0; TcN = 0; TdN = 0; 

  Posi_412 = _4_posi - _1_posi - _2_posi;
  Posi_41_2 = _4_posi - _1_posi + _2_posi;
  Posi_41  = _4_posi - _1_posi;
  Posi_42  = _4_posi - _2_posi;
  Posi_4_2 = _4_posi + _2_posi;
  Posi_4F1 = _4_posi - FileBuff[ 399];
  Posi_4F2 = _4_posi - FileBuff[ 799];
  Posi_4F3 = _4_posi - FileBuff[ 1199];
  Posi_4F4 = _4_posi - FileBuff[1599];


  //A_Posi   = ((_1_posi-Ka1[_A_Range])*1024)/Ka2[_A_Range];
  //B_Posi   = ((_2_posi-Kb1[_B_Range])*1024)/Kb2[_B_Range];

  A_Posi   = _1_posi-Ka1[_A_Range];    // Kx2 is for signal gain correction, not Y positioning
  B_Posi   = _2_posi-Kb1[_B_Range];

  
  a_Max = A_Posi; b_Max = B_Posi; 
  a_Min = A_Posi; b_Min = B_Posi;             

  if((_3_posi + 20)>= Y_BASE+Y_SIZE)  c_Max = Y_BASE+Y_SIZE-1;
  else                                c_Max = _3_posi + 20;
  if((_4_posi + 20)>= Y_BASE+Y_SIZE)  d_Max = Y_BASE+Y_SIZE-1;
  else                                d_Max = _4_posi + 20;
  
  bag_max_buf = get_bag_max_buf();
  b_Avg = a_Ssq = b_Ssq = a_Avg = bag_max_buf/2;	 // centers value to center of "0" step, makes meters equally sensitive to + and - signals


    if(_Status==RUN)TempKp1=_Kp1;	 // eliminates improper change of wave timebase by Kp1 while in hold mode at faster timebases
    k =((1024 - TempKp1)*150 + 512)/1024 + _X_posi.Value;  // window position in the calculation of the interpolation of the correction value

      if ((exitflag==1)&&(JumpCnt!=0)) exitflag=0;     // leave exitflag on at beginning of frame so if complete frame is done, allow freerun     

      for(i=0; i <bag_max_buf; i++){
      if(((_T_base > 11)||(_Mode == SGL)||(freerun==1)||((_Mode==X_Y)&&(_T_base>9)))&&(_Status == RUN)){				//>200uS/div
        DataBuf[i] = __Read_FIFO();    
	  swap=0x300;
	  swap &= DataBuf[i];
	  if ((swap==0x100)||(swap==0x200))DataBuf[i]^=0x300; //swap 2 least significant bits of chB, fixes error in FPGA programming (V2.61)
      }
      else if((__Get(FIFO_EMPTY)==0)&&(i == JumpCnt)&&(_Status == RUN))     
  	{
        JumpCnt++;
        DataBuf[i] = __Read_FIFO();             // read into the 32-bit FIFO data reading pointer +1
	  swap=0x300;
	  swap &= DataBuf[i];
	  if ((swap==0x100)||(swap==0x200))DataBuf[i]^=0x300; //swap 2 least significant bits of chB, fixes error in FPGA programming (V2.61)
  	}

      Ch[A] = (DataBuf[i] & 0xFF );              	    // now only used for time measurements	
	V[A]=(Ch[A]-ADCoffset);					    // for wave trace,(also now used for meters, with signed vars) load into signed var and no clipping
	if (Ch[A]<ADCoffset) Ch[A]=ADCoffset;   		    // clip at new 0 level, needed for unsigned var	    	
	Ch[A]-=ADCoffset;						    // with ypos shift canceling this, moves operating point back to normal	

      a_Avg += V[A];                          		    // use signed vars, otherwise, values at bottom of screen, pushed up by Kx1, now below zero, fail to register

      Tmp = V[A]- A_Posi;
      a_Ssq +=(Tmp * Tmp);		                      // statistical sum of squares of the A channel
      Ch[B] = ((DataBuf[i] >> 8) & 0xFF);
	V[B]=(Ch[B]-ADCoffset);
	if (Ch[B]<ADCoffset) Ch[B]=ADCoffset;               	    	
	Ch[B]-=ADCoffset;

      b_Avg += V[B];                                     // cumulative average channel B, DC

      Tmp = V[B]- B_Posi;
      b_Ssq +=(Tmp * Tmp);                                // statistical sum of squares of the B channel

		if(i == 0) {	// read first values - max = min = values
		    a_Max = V[A];	// statistics channel A maximum
			a_Min = a_Max;	// statistics channel A minimum
			b_Max = V[B];	// statistics channel B maximum
			b_Min = b_Max;	// statistics channel B minimum
		} else {		// not the first values
			if(V[A] > a_Max)  a_Max = V[A];         // statistics channel A maximum
			if(V[A] < a_Min)  a_Min = V[A];         // statistics channel A minimum
			if(V[B] < b_Min)  b_Min = V[B];         // statistics channel B minimum
			if(V[B] > b_Max)  b_Max = V[B];         // statistics channel B maximum
		}

    // FFT ///////////////////////////////////////
      if ((_4_source==FFT_A) || (_4_source==SPEC_A))
		//{if ((i < FFTSize) && ShowFFT) {
		{if (i < FFTSize) {
			  fr[i] = Ch[A]<<2;
        }
}


         if ((_4_source==FFT_B) || (_4_source==SPEC_B))
        {
           //if ((i < FFTSize) && ShowFFT) {
		   if (i < FFTSize) {
              fr[i] = Ch[B]<<2;
             }
        }
		////////////////////////////////////// FFT ///
       
      if (i>4) {C_D = DataBuf[i-5] >>16;}else C_D=0;	    // align digital chs with analogs	

      if(    (i>3)&&(i<(bag_max_buf-3))){		//exclude possible noise at start & end on some ranges (for time measurements only)

        if(Ch[A] > a_Mid_H){
          if (Sa<2){					//don't initiate if value is above trigger point when starting out
            if(Sa == 0){
		  TaS = i;                    					
              if(Ta==0){				//use Ta to initiate
                Ta=i;					//first + crossing point saved 
		  }else{					//after initialized, count + crossings, add + wave sampling count to PaS
		    TaN++;                          //increment TaN only after initial crossing point saved
		    PaS += Pa;				//only add positive part if whole wave is considered	
              }
		}
            Sa = 1;  
          } 
        } else { 
          if(Ch[A] < a_Mid_L){
            if(Sa == 2){
		  Sa=0; 					//initiate only after going below trigger point so partial wave does not get counted
		  PaS=0;					//initiate PaS
            }
	      if(Sa == 1){
              Sa = 0;
		  Pa = i-TaS;				//save for PaS
            }
          }
        }

       if(Ch[B] > b_Mid_H){
          if (Sb<2){					
            if(Sb == 0){
		  TbS = i;                    					
              if(Tb==0){				
                Tb=i;					 
		  }else{
		    TbN++;                         
		    PbS += Pb;					
              }
		}
            Sb = 1;  
          } 
        } else { 
          if(Ch[B] < b_Mid_L){
            if(Sb == 2){
		  Sb=0; 					
		  PbS=0;					
            }
	      if(Sb == 1){
              Sb = 0;
		  Pb = i-TbS;				
            }
          }
        }

       if(C_D & 1){
          if (Sc<2){					
            if(Sc == 0){
		  TcS = i;                    					
              if(Tc==0){				
                Tc=i;					 
		  }else{
		    TcN++;                         
		    PcS += Pc;					
              }
		}
            Sc = 1;  
          } 
        } else { 
            if(Sc == 2){
		  Sc=0; 					
		  PcS=0;					
            }
	      if(Sc == 1){
              Sc = 0;
		  Pc = i-TcS;				
            }
	  } 

       if(C_D & 2){
          if (Sd<2){					
            if(Sd == 0){
		  TdS = i;                    					
              if(Td==0){				
                Td=i;					 
		  }else{
		    TdN++;                         
		    PdS += Pd;					
              }
		}
            Sd = 1;  
          } 
        } else { 
            if(Sd == 2){
		  Sd=0; 					
		  PdS=0;					
            }
	      if(Sd == 1){
              Sd = 0;
		  Pd = i-TdS;				
            }
        }
      }

      if(i >= k){                               // pointer to reach the specified window position


														// Use V[x] loaded above rather than unsigned Ch[x], prevents clipping.

	if (CalFlag>0) {
        V[A]  = Ka1[_A_Range]+A_Posi+(Ka2[_A_Range]*(V[A]-A_Posi)+ 512)/1024;      // Factor in gain correction for signal (Ka2) from signal zero point
        V[B]  = Kb1[_B_Range]+B_Posi+(Kb2[_B_Range]*(V[B]-B_Posi)+ 512)/1024;      // rather than screen bottom, prevents interference with offsets
	}else{											    
        V[A]  = Ka1[_A_Range]+V[A];      // gain correction switched off
        V[B]  = Kb1[_B_Range]+V[B];      // 
      }
        while(j > 0 ){
          Send_Data( V[A_]+((V[A]-V[A_])*(1024 - j))/1024, // the current CH_A point interpolation
                     V[B_]+((V[B]-V[B_])*(1024 - j))/1024, // the current CH_B point interpolation
                     C_D,                                  // current point digital channel values
                     n++);
          j -= TempKp1+5;
          if(n >= X_SIZE-TRACK_OFFSET){ k = 8192;  break;}     //300
        }
        j += 1024;
        V[A_] = V[A];  V[B_] = V[B];     
        }
    }						// end of for loop

    if ((exitflag==1)&&(JumpCnt<393)) exitflag=0;      //if exitflag was on at start, full frame finished allows freerun     
 
 
 
 

  if ((FrameMode!=0) && (_T_base>11) && (_Mode==SCAN)) __Set(FIFO_CLR, W_PTR);	

  a_Mid_H = 4 +(a_Max + a_Min)/2;
  a_Mid_L = a_Mid_H - 8;
  b_Mid_H = 4 +(b_Max + b_Min)/2;
  b_Mid_L = b_Mid_H - 8;

  TaS -= Ta; TbS -= Tb; TcS -= Tc; TdS -= Td;


   if (Title[TRIGG][SOURCE].Value>1){			//if triggering from digital chs, alignment with analog chs shifts waveforms to the right,
     for(h=0; h<(X_SIZE-5); h++){                     //shift back to original position   
        TrackBuff[h*4] = TrackBuff[(h+5)*4];
        TrackBuff[h*4+1] = TrackBuff[(h+5)*4+1];
        TrackBuff[h*4+2] = TrackBuff[(h+5)*4+2];
        TrackBuff[h*4+3] = TrackBuff[(h+5)*4+3];
     }
   }

   if (_T_base < 17)							// If sngl discard 4
     {if (_T_base > 11)
	 {discard=4;
       }else discard=1;
     }else discard=0;
   if (_Mode==SGL) discard=4;


   for(jj=0; jj<discard; jj++){                            // Discard first pixels, fill with next valid sample
    TrackBuff[jj*4] = TrackBuff[discard*4];		     // conditional in draw.c blanks these 	
    TrackBuff[jj*4+1] = TrackBuff[discard*4+1];            // this way it doesn't misalign wave trace with trigger vernier...
   }

   if (Title[TRIGG][SOURCE].Value<2)discard+=5;            // add 5 to blank C and D if not shifting screen to the left 5 samples
   for(jj=0; jj<(discard); jj++){                          // Discard first pixels, fill with next valid sample
    TrackBuff[jj*4+2] = TrackBuff[(discard)*4+2];           
   }
   if (Title[TRIGG][SOURCE].Value<2){
     if ((_4_source==A_add_B)||(_4_source==A_sub_B))discard-=5;  
   }
   for(jj=0; jj<(discard); jj++){                           	
    TrackBuff[jj*4+3] = TrackBuff[(discard)*4+3];
   }

   // FFT /////////////////////////////
		if (ShowFFT || (_4_source==SPEC_A) || (_4_source==SPEC_B)) {
			  for(i=0; i<FFTSize;i++)	fi[i] = 0;
			  fix_fft(fr, fi, FFTSize);
			  
			  for (i=0; i < FFTSize/2; i++)
			  {
				X= fr[i]; /* Real */
				Y= fi[i];   /* Imag */    
				fr[ i ] = Int_sqrt(X*X+ Y*Y);    
			  }
			  
			  
			  if(_T_Scale < 333) {		// Avoid datatype overflow
				NFreq = 500000 / (_T_Scale / ((double)_T_KP / 1024));
				
				Int2Str(NFreqStr, NFreq, FM_UNIT, 4, UNSIGN);
				Int2Str(FreqT1Str, ((NFreq / FFTBins) * _T1), FM_UNIT, 4, UNSIGN);
				Int2Str(FreqDivStr, ((NFreq / FFTBins) * 30), FM_UNIT, 4, UNSIGN);
			  } else {
				NFreq = 500000000 / (_T_Scale / ((double)_T_KP / 1024));
				// NFreq *= 1000;
				
				Int2Str(NFreqStr, NFreq, F_UNIT, 4, UNSIGN);	
				Int2Str(FreqT1Str, ((NFreq / FFTBins) * _T1), F_UNIT, 4, UNSIGN);
				Int2Str(FreqDivStr, ((NFreq / FFTBins) * 30), F_UNIT, 4, UNSIGN);
			  }
			  
			  PeakFreq = 0;
			  imax = 0;
			  
			  for (i=2; i < FFTSize/2; i++) {
				if (PeakFreq < fr[i]) {  
					PeakFreq= fr[i] ; 
					imax = i;
				}
			  }
			  
			  if (imax>1) {
					if(_T_Scale < 333) 		// Avoid datatype overflow
						// Int2Str(PeakFreqStr, ((NFreq / FFTBins) * imax), FM_UNIT, 4, UNSIGN);
						Int2Str(PeakFreqStr, ((NFreq / FFTBins) * imax * 1000), F_UNIT, 4, UNSIGN);
					else
						Int2Str(PeakFreqStr, ((NFreq / FFTBins) * imax), F_UNIT, 4, UNSIGN);
			  }
		}
  //////////////////////////// FFT ///
  
}

void Send_Data(s16 Va, s16 Vb, u8 C_D, u16 n)  // output display data
{
  s32 Tmp = 0, i;

  i = n*4;
  if(Va >= Y_BASE+Y_SIZE)  TrackBuff[i + TRACK1] = Y_BASE+Y_SIZE-1;
  else if(Va <= Y_BASE+1)  TrackBuff[i + TRACK1] = Y_BASE+1;
  else                     TrackBuff[i + TRACK1] = Va;
  if(Vb >= Y_BASE+Y_SIZE)  TrackBuff[i + TRACK2] = Y_BASE+Y_SIZE-1;
  else if(Vb <= Y_BASE+1)  TrackBuff[i + TRACK2] = Y_BASE+1;
  else                     TrackBuff[i + TRACK2] = Vb;
  
  if(C_D & 1)  TrackBuff[i + TRACK3] = c_Max;
  else         TrackBuff[i + TRACK3] = _3_posi;
  
  switch (_4_source){                       
  case A_add_B:
    Tmp = Posi_412 + Va + Vb;
    break;
  case A_sub_B:
    //Tmp = Posi_412 + Va - Vb;	//did not work right
    Tmp = Posi_41_2 + Va - Vb;	//uses new var with correct values
    break;
  case C_and_D:
    //if((~C_D)& 3) Tmp = d_Max;				// was backwards... 
    //else          Tmp = _4_posi;
    if((~C_D)& 3) Tmp = _4_posi; 
    else          Tmp = d_Max;
    break;  
  case C_or_D:
    if(C_D & 3)   Tmp = d_Max; 
    else          Tmp = _4_posi;
    break;  
  case REC_1:
    Tmp = Posi_4F1 + FileBuff[n];  
    break;
  case REC_2:
    Tmp = Posi_4F2 + FileBuff[n+400];  
    break;
  case REC_3:
    Tmp = Posi_4F3 + FileBuff[n+800];  
    break;
  case REC_4:
    Tmp = Posi_4F4 +  FileBuff[n+1200];  
    break;
  case FFT_A:
  case FFT_B:
  case SPEC_A:
  case SPEC_B:
    break;
  default:
    if(C_D & 2)  Tmp = d_Max;
    else         Tmp = _4_posi;
  }
  if(Tmp >= Y_BASE+Y_SIZE)  TrackBuff[i + TRACK4] = Y_BASE+Y_SIZE-1;
  else if(Tmp <= Y_BASE+1)  TrackBuff[i + TRACK4] = Y_BASE+1;
  else                      TrackBuff[i + TRACK4] = Tmp;
}
/*******************************************************************************
 Synchro: scan synchronization, waveform display by setting the mode
*******************************************************************************/
void Synchro(void)  // scan synchronization: AUTO, NORM, SGL, NONE, SCAN modes
{ 
u16  i;
freerun=0;

if (FrameMode>0)								//single frame buffer
 {
  switch (_Mode)
  {  
     case X_Y:
     case AUTO:
        __Set(TRIGG_MODE,(_Tr_source <<3)+_Tr_kind);  
      if(__Get(FIFO_START)!=0){
        if (entryflag>0){
          JumpCnt=0;
          entryflag=0;
        }
        Process();
        Wait_Cnt=shortwait[_T_base];                                              
        exitflag=1;                                                          
      }else if ((Wait_Cnt==1)&&(_T_base < 9)){
         if (_T_base <6)__Set(FIFO_CLR, W_PTR);                              
       }else if(Wait_Cnt ==0){
            if (_T_base > 8){
              freerun=1;
              entryflag=0;
            }else{
               freerun=2;						  
               entryflag=1;                                                 
             }
            if (JumpCnt > 392)JumpCnt = 0;                     
            Process();
        }
      break;
    case NORH:
      __Set(TRIGG_MODE,(_Tr_source <<3)+_Tr_kind);  
      if(__Get(FIFO_START)!=0)
      {
        Process();                                 
      }else{ 
        if(Wait_Cnt==0) Wait_Cnt = 1;	                      
      }
      break;
    case NORC:
     __Set(TRIGG_MODE,(_Tr_source <<3)+_Tr_kind);  
     if(__Get(FIFO_START)!=0)
        {
          Process();
          Wait_Cnt=Wait[_T_base];
        } else if(Wait_Cnt==0)						//was ==1
         {
          if (_T_base < 7) cleardatabuf( A_Posi,  B_Posi, 393);
          for(i=0; i<4*X_SIZE; ++i)  TrackBuff[i] = 0;
          Wait_Cnt=Wait[_T_base];						//added
      }
      break;
   case SGL:
      __Set(TRIGG_MODE,(_Tr_source <<3)+_Tr_kind);  
      if(__Get(FIFO_START)!=0)  Process();          
      break;
    case SCAN:								
      if (_T_base>11){ 
        __Set(TRIGG_MODE, UNCONDITION);            	// works best at high sweep rates
      }else{                                          // works better at low rates
       __Set(TRIGG_MODE,3);  		                  // same as track1 (0) <<3 + 3 (>vt) (do not use digital chs...) 
       __Set(V_THRESHOLD,255);                        // level then set out of range. Prevents triggering(vt>255), with fifo "splice" shifted left 150pix, gives smooth display at low speeds
      }
      if (_T_base>7){ 
        freerun=1;
        Process();                                  	
      }else{ 								//tbase<8
         freerun=2;
         if ((FlagMeter==0) && (JumpCnt> 389)) JumpCnt=0;
         if ((FlagMeter==1) && (JumpCnt> 304)) JumpCnt=0;
         Process();
      } 
  } //switch
 }
 else										//regular large buffer mode
 { 
 switch (_Mode)
  { 
   case X_Y:
   case AUTO:
       __Set(TRIGG_MODE,(_Tr_source <<3)+_Tr_kind);  
      if(__Get(FIFO_START)!=0) {
        Process();                                 
        Wait_Cnt = Wait[_T_base];
      } else if(Wait_Cnt==0) {
          if(JumpCnt >= 4095)  JumpCnt = 0; 
          Process();   
          Wait_Cnt = Wait[_T_base];
      } break;
    case NORH:
      __Set(TRIGG_MODE,(_Tr_source <<3)+_Tr_kind);  
      if(__Get(FIFO_START)!=0) {
        Process();                                 
        Wait_Cnt = Wait[_T_base];
      } else if(Wait_Cnt==0) {
        Wait_Cnt = Wait[_T_base];
      } break;
     case NORC:
      __Set(TRIGG_MODE,(_Tr_source <<3)+_Tr_kind);  
      if(__Get(FIFO_START)!=0) {
        Process();                                 
        Wait_Cnt = Wait[_T_base];
      } else if(Wait_Cnt==0) {
        for(i=0; i<4*X_SIZE; ++i)  TrackBuff[i] = 0;      // clear screen
        Wait_Cnt = Wait[_T_base];
      } break;
   case SGL:
      __Set(TRIGG_MODE,(_Tr_source <<3)+_Tr_kind);  
      if(__Get(FIFO_START)!=0)  Process();         
      break;
    case SCAN:								
      if (_T_base>11){ 
        __Set(TRIGG_MODE, UNCONDITION);               	
      }else{
       __Set(TRIGG_MODE,3);  
       __Set(V_THRESHOLD,255); 
      }
      if (_T_base>7){ 
        freerun=1;
        Process();                                  
      }else{ 								//tbase<8
         freerun=2;
         if(JumpCnt >= 4095)  JumpCnt = 0; 
         Process();
       } 
  }  										
 }   										// end else (framemode=0)
  

 
  if (_Mode==SCAN) Wait_Cnt = 1;	     	

  if ((FrameMode > 0)&&(_Mode!=SGL)){                     	// in single frame buffer mode
    if (_Mode==SCAN)						     	
    {
      if ((FlagMeter==0) && (JumpCnt> 389)) JumpCnt=0;
      if ((FlagMeter==1) && (JumpCnt> 304)) JumpCnt=0;
    } 
    if (JumpCnt> 392){
        if((__Get(FIFO_START)!=0)&&(freerun!=2))            // resetting FIFO after start flag and frame completed provides proper triggering in single frame buffer mode
	  {
        JumpCnt=0;
        __Set(FIFO_CLR, W_PTR);              	            // FIFO write pointer reset
        }
    }    
  } 

   if(HoldOnNext==1) {
    _State.Value = HOLD;                                    // for single mode
    _State.Flag |= UPDAT;
    HoldOnNext=0;
    return;
   }

 if ((_Status == RUN)&&(_T_base > 11)&&(_T_base < 14)&&(_Mode != SGL))   //special case for "in between" sweep rates...
    if ((__Get(FIFO_START)!=0)&&(FrameMode >0))	__Set(FIFO_CLR, W_PTR);	 // prevents "double looping" in single window buffer mode
                                                                         // with slow or random triggers from failure to "fill" buffer and reset

if ((_Status == RUN)&&(__Get(FIFO_FULL)!=0))  {                         // FIFO is full
    if(_Mode != SGL) {
      __Set(FIFO_CLR, W_PTR);                                            // FIFO write pointer reset
    }
    if (FrameMode == 0)  Wait_Cnt = Wait[_T_base];
    JumpCnt =0;
    if(_Mode == SGL){
      HoldOnNext=1;
    }
  }

  if ((_Mode != AUTO)||(_T_base > 8)||(exitflag==0)||(entryflag==0)) Draw_Window(); //Executing time consuming screen update AFTER resetting FIFO rather than before
                                                                                    //allows enough time for START flag to register at fast time bases - fixes triggering
														//issue of slow/random pulses not triggering
}   										               

u16 get_bag_max_buf(void) {
u16 out = 4096;

  if ((FrameMode>0)&&(_Mode!=SGL)){
    if (_Mode==SCAN){					
        if (FlagMeter==0) out =  390;
        if (FlagMeter==1) out =  305;
        if (_T_base>9) out=390+150;			//moves trigger splice out of view to left edge of screen at fast time bases for cleaner display
    } else out = 393;					
  }
  return out;
}

void cleardatabuf(s16 Afill, s16 Bfill, u16 n)   //Clears/replaces n samples in DataBuf
{
 u32 sample;
 u16 i;

sample=((Bfill+ADCoffset) & 0xff);					 
sample <<= 8;
sample|=((Afill+ADCoffset) & 0xff);
   for (i=0; i<n; i++) DataBuf[i]=sample;      
}


/******************************** END OF FILE *********************************/
