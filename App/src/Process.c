/******************** (C) COPYRIGHT 2009 e-Design Co.,Ltd. *********************
 File Name : Process.c       
 Version   : DS203_APP Ver 2.3x                                  Author : bure
*******************************************************************************/
#include "Interrupt.h"
#include "Function.h"
#include "Process.h"
#include "Draw.h"
#include "Menu.h"
#include "BIOS.h"
#include "File.h"

u16 TaS, TbS, TcS, TdS;            // 周期累计
u16 PaS, PbS, PcS, PdS;            // 脉宽累计
u16 TaN, TbN, TcN, TdN;            // 周期计数
u8  a_Mid_H, a_Mid_L;
u8  b_Mid_H, b_Mid_L;

s8  Kab;                                     // 模拟通道零点平衡校正系数
u32 a_Avg, b_Avg, a_Ssq, b_Ssq;              // 平均值累计,平方和累计
u8  a_Max, b_Max, a_Min, b_Min;              // 原始最大值,原始最小值
s16 Posi_412, Posi_41, Posi_42, Posi_4_2, Posi_4F1, Posi_4F2, Posi_4F3, Posi_4F4;
s16 c_Max, d_Max, A_Posi, B_Posi;
u8  Full=1, Interlace;
u16 JumpCnt;
u8 FrameMode;
u16 Sample;
u8 Pagina;

uc16 Wait[27]= {1000, 500, 200, 100, 50, 20, 10, 5, 2, 2,   
                2,      2,   2,   2,  2,  2,  2, 2, 2, 2,    
                2,      2,   2,   2,  2,  2,  2 };
                     
Y_attr *Y_Attr; 
X_attr *X_Attr; 
G_attr *G_Attr; 
T_attr *T_Attr; 

u32 DataBuf[4096];
u8  TrackBuff  [X_SIZE * 4];         // 曲线轨迹缓存：i+0,i+1,i+2,i+3,分别存放1～4号轨迹数据


s8  Ka1[10] ={   0,    0,    0,    0,    0,    0,    0,    0,    0,    0}; // A通道低位误差校正系数
s8  Kb1[10] ={   0,    0,    0,    0,    0,    0,    0,    0,    0,    0}; // B通道低位误差校正系数
u16 Ka2[10] ={1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024}; // A通道增益误差校正系数
u16 Kb2[10] ={1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024}; // B通道增益误差校正系数
s8  Ka3[10] ={   0,    0,    0,    0,    0,    0,    0,    0,    0,    0}; // A通道高位误差校正系数
s8  Kb3[10] ={   0,    0,    0,    0,    0,    0,    0,    0,    0,    0}; // B通道高位误差校正系数

D_tab D_Tab[23] ={  // 脉冲波形输出驱动表, 基于72MHz主频
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


//A_tab A_Tab[11] ={ // 模拟波形输出驱动表, 基于72MHz主频, 每周期36点合成
////    STR     PSC     ARR 
//  {" 10Hz ",  20-1,  10000-1},
//  {" 20Hz ",  20-1,   5000-1},
//  {" 50Hz ",  20-1,   2000-1},
//  {"!100Hz!", 20-1,   1000-1},
//  {"!200Hz!", 20-1,    500-1},
//  {"!500Hz!", 20-1,    200-1},
//  {" 1KHz ",  20-1,    100-1},
//  {" 2KHz ",  20-1,     50-1},
//  {" 5KHz ",  20-1,     20-1},
//  {"!10KHz!", 20-1,     10-1},
//  {"!20KHz!", 20-1,      5-1}};
A_tab A_Tab[15] ={ // 模拟波形输出驱动表, 基于72MHz主频, 每周期36点合成
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

//u16 SIN_DATA[72] =  // Sine wave data                                                                                                 //         
//  {0x000,0x027,0x08E,0x130,0x209,0x311,0x441,0x58F,0x6F0,    // 90
//   0x85A,0x9C0,0xB19,0xC59,0xD76,0xE68,0xF26,0xFAB,0xFF3,    // 180
//   0xFFF,0xFD7,0xF70,0xECE,0xDF5,0xCED,0xBBD,0xA6F,0x90E,    // 270
//   0x7A4,0x63E,0x4E5,0x3A5,0x288,0x196,0x0D8,0x053,0x00B,};  // 360   
//u16 SIN_DATA[72] = 
//{2048,2404,2748,3072,3364,3617,3822,3972,4065,4096,4065,3972,3822,3617,3364,
//3072,2748,2404,2048,1692,1348,1024,732,479,274,124,31,0,31,124,274,479,732,1024,
//1348,1692,
//2048,2404,2748,3072,3364,3617,3822,3972,4065,4096,4065,3972,3822,3617,3364,
//3072,2748,2404,2048,1692,1348,1024,732,479,274,124,31,0,31,124,274,479,732,1024,
//1348,1692,};

u16 SIN_DATA[72] = 
{2048,2226,2404,2578,2748,2914,3072,3223,3364,3496,3617,3726,3822,
3904,3972,4026,4065,4088,3000,4088,4065,4026,3972,3904,3822,3726,3617,
3496,3364,3223,3072,2914,2748,2578,2404,2226,2048,1870,1692,1518,1348,
1182,1024,873,732,600,479,370,274,192,124,70,100,100,100,100,100,70,124,192,274,
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

u16 DIGI_DATA[72] =  // Sawtooth wave data                                                                                             //         
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
   Update = 1;                  // 返回后恢复原来的档位设置
}

/*******************************************************************************
 App_init: 显示窗口波形数据初始化
*******************************************************************************/
void App_init(void)
{ 
  u16 i, j;
  __Set(ADC_CTRL, EN );       
  __Set(STANDBY, DN);          // 退出省电状态
  __Clear_Screen(BLACK);       // 清屏幕
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
  Update = 1;                  // 返回后恢复原来的档位设置
}
/*******************************************************************************
 View_init: 显示窗口波形数据初始化
*******************************************************************************/
void View_init(void)
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
}
/*******************************************************************************
 Update_Range: 
*******************************************************************************/
void Update_Range(void) 
{
  Interlace = 0;
  __Set(ADC_CTRL, EN);       
  __Set(ADC_MODE, SEPARATE);                        // Set Separate mode ((Range + 1)*25)
  __Set(CH_A_COUPLE, Title[TRACK1][COUPLE].Value);
  __Set(CH_A_RANGE,  Title[TRACK1][RANGE].Value);
  __Set(CH_A_OFFSET, ((1024 + Ka3[_A_Range])*_1_posi + 512)/1024);
//  if(_1_source == HIDE){
//    if(_T_base > 16){
//      Interlace = 1;                      
//      __Set(CH_A_RANGE, G_Attr[0].Yp_Max+1);        // A通道合并到B通道 
//      __Set(ADC_CTRL, EN + 2);       
//      __Set(ADC_MODE, INTERLACE);                   // Set Interlace mode
//    }  
//  }
  __Set(CH_B_COUPLE, Title[TRACK2][COUPLE].Value);
  __Set(CH_B_RANGE,  Title[TRACK2][RANGE].Value);
  __Set(CH_B_OFFSET, ((1024 + Kb3[_B_Range])*_2_posi + 512)/1024);
//  if(_2_source == HIDE){
//    if(_T_base > 16){
//      Interlace = 1;                        
//      __Set(CH_B_RANGE,  G_Attr[0].Yp_Max+1);      // B通道合并到A通道
//      __Set(ADC_CTRL, EN + 2);       
//      __Set(ADC_MODE, INTERLACE);                  // Set Interlace mode
//    }
//  } 
  if(_Status == RUN) __Set(FIFO_CLR, W_PTR);       // FIFO写指针复位
}
/*******************************************************************************
 Update_Base: 
*******************************************************************************/
void Update_Base(void) 
{
  u16 i;
  
  __Set(ADC_CTRL, EN);       
  if(Interlace == 0)  i = Title[T_BASE][BASE].Value;     // 独立采样模式
  else                i = Title[T_BASE][BASE].Value + 5; // 交替采样模式
  __Set(T_BASE_PSC, X_Attr[i].PSC);
  __Set(T_BASE_ARR, X_Attr[i].ARR);
  Wait_Cnt = Wait[_T_base];
  if(_Status == RUN) __Set(FIFO_CLR, W_PTR);      // FIFO写指针复位
}
/*******************************************************************************
 Update_Output: 
*******************************************************************************/
void Update_Output(void) 


{
  
  u8 att;
  
  
  
//   DMA2_Channel4->CCR &= ~DMA_CCR1_EN;
//    __Set(ANALOG_CNT, sine_count);
//    __Set(ANALOG_PTR, (u32)sine_table);
//    DMA2_Channel4->CCR |= DMA_CCR1_EN;
//
//    // Now set ARR and we are done
//    __Set(ANALOG_ARR, arr);
  
  
  
  if(_Kind == SINE){
    for(att=0; att <72; att++){ 
     ATT_DATA[att]=(SIN_DATA[att]*Title[OUTPUT][OUTATT].Value)/100;}
     DMA2_Channel4->CCR &= ~DMA_CCR1_EN;
      __Set(ANALOG_PSC,  A_Tab[_Frqn].PSC);
      __Set(ANALOG_CNT, 72);
      __Set(ANALOG_PTR, (u32)ATT_DATA);
    DMA2_Channel4->CCR |= DMA_CCR1_EN;
      __Set(ANALOG_ARR, A_Tab[_Frqn].ARR);
    
  }
  if(_Kind == SAW){
   for(att=0; att <72; att++){ 
     ATT_DATA[att]=(SAW_DATA[att]*Title[OUTPUT][OUTATT].Value)/100;}
     DMA2_Channel4->CCR &= ~DMA_CCR1_EN;
      __Set(ANALOG_PSC,  A_Tab[_Frqn].PSC);
      __Set(ANALOG_CNT, 72);
      __Set(ANALOG_PTR, (u32)ATT_DATA);
     DMA2_Channel4->CCR |= DMA_CCR1_EN;
      __Set(ANALOG_ARR, A_Tab[_Frqn].ARR);
  }
  if(_Kind == TRIANG){
    for(att=0; att <72; att++){ 
     ATT_DATA[att]=(TRG_DATA[att]*Title[OUTPUT][OUTATT].Value)/100;}
     DMA2_Channel4->CCR &= ~DMA_CCR1_EN;
      __Set(ANALOG_PSC,  A_Tab[_Frqn].PSC);
      __Set(ANALOG_CNT, 72);
      __Set(ANALOG_PTR, (u32)ATT_DATA);
    DMA2_Channel4->CCR |= DMA_CCR1_EN;
      __Set(ANALOG_ARR, A_Tab[_Frqn].ARR);
  }
  
  if(_Kind == DIGI){
    for(att=0; att <72; att++){ 
     ATT_DATA[att]=(DIGI_DATA[att]*Title[OUTPUT][OUTATT].Value)/100;}
     DMA2_Channel4->CCR &= ~DMA_CCR1_EN;
      __Set(ANALOG_PSC,  A_Tab[_Frqn].PSC);
      __Set(ANALOG_CNT, 72);
      __Set(ANALOG_PTR, (u32)ATT_DATA);
    DMA2_Channel4->CCR |= DMA_CCR1_EN;
      __Set(ANALOG_ARR, A_Tab[_Frqn].ARR);
  }
  
  
//  if(_Kind == DIGI){
//    __Set(DIGTAL_PSC, D_Tab[_Frqn].PSC);
//    __Set(DIGTAL_ARR, D_Tab[_Frqn].ARR);
//    __Set(DIGTAL_CCR, (D_Tab[_Frqn].ARR+1)/2);
//  }
  if(_Kind == PWM){
    __Set(DIGTAL_PSC, D_Tab[_Frqn].PSC);
    __Set(DIGTAL_ARR, D_Tab[_Frqn].ARR);
    __Set(DIGTAL_CCR, ((D_Tab[_Frqn].ARR+1)*(100-Title[OUTPUT][DUTYPWM].Value))/100);
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
    __Set(V_THRESHOLD, (((_Vt1-Ka1[_A_Range])*1024)/Ka2[_A_Range])&0xFF); 
  }
  if(Title[TRIGG][SOURCE].Value == TRACK2){ 
    __Set(V_THRESHOLD, (((_Vt2-Kb1[_B_Range])*1024)/Kb2[_B_Range])&0xFF); 
  }
  if(_Status == RUN) __Set(FIFO_CLR, W_PTR);      // FIFO写指针复位
}
/*******************************************************************************
 Process: 计算处理缓冲区数据 
*******************************************************************************/
void Process(void)
{ 
  s16 i, j = 0, k, V[8] = {0}, n = 0;
  s32 Tmp;
  u8  Ch[4], C_D;
  s8  Sa = 2, Sb = 2, Sc = 2, Sd = 2; // 计时状态
  u16 Ta, Tb, Tc, Td;                 // 脉宽计数
  u16 bag_max_buf = 4096;
  
  Ta = Tb = Tc = Td = 0;
  
  PaS = 0; PbS = 0; PcS = 0; PdS = 0; 
  TaN = 0; TbN = 0; TcN = 0; TdN = 0; 

  Posi_412 = _4_posi - _1_posi - _2_posi;
  Posi_41  = _4_posi - _1_posi;
  Posi_42  = _4_posi - _2_posi;
  Posi_4_2 = _4_posi + _2_posi;
  Posi_4F1 = _4_posi - FileBuff[ 399];
  Posi_4F2 = _4_posi - FileBuff[ 799];
  Posi_4F3 = _4_posi - FileBuff[ 1199];
  Posi_4F4 = _4_posi - FileBuff[1599];
  A_Posi   = ((_1_posi-Ka1[_A_Range])*1024)/Ka2[_A_Range];
  B_Posi   = ((_2_posi-Kb1[_B_Range])*1024)/Kb2[_B_Range];
  
  a_Max = A_Posi; b_Max = B_Posi; 
  a_Min = A_Posi; b_Min = B_Posi;             
  
  //a_Avg = 2048;   b_Avg = 2048;   #pmos69 - seems weird to start the average (total) counter with anything but zero 
  //a_Ssq = 2048;   b_Ssq = 2048;   #pmos69 - seems weird to start the square (total) counter with anything but zero   
  a_Avg = 0;   b_Avg = 0; 
  a_Ssq = 0;   b_Ssq = 0;
  
  if((_3_posi + 20)>= Y_BASE+Y_SIZE)  c_Max = Y_BASE+Y_SIZE-1;
  else                                c_Max = _3_posi + 20;
  if((_4_posi + 20)>= Y_BASE+Y_SIZE)  d_Max = Y_BASE+Y_SIZE-1;
  else                                d_Max = _4_posi + 20;
 
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



  if(Interlace == 0){                           // 独立采样模式
  
    k =((1024 -_Kp1)*150 + 512)/1024 + _X_posi.Value;//  // 计算插值运算后窗口位置的修正值
   
    for(i=0; i <bag_max_buf; i++){
       
             if((_T_base > 15)&&(_Status == RUN))  DataBuf[i] = __Read_FIFO(); // 读入32位FIFO数据, 读后指针+1
          else if((__Get(FIFO_EMPTY)==0)&&(i == JumpCnt)&&(_Status == RUN)){
            JumpCnt++;
            DataBuf[i] = __Read_FIFO();             // 读入32位FIFO数据, 读后指针+1
          }
         
      
          Ch[A] = (DataBuf[i] & 0xFF );              
          a_Avg += Ch[A];                           // 累计A通道直流平均值              
          Tmp = Ch[A]- A_Posi;
          a_Ssq +=(Tmp * Tmp);                      // 统计A通道平方和
          Ch[B] = ((DataBuf[i] >> 8) & 0xFF);       
          b_Avg += Ch[B];                           // 累计B通道直流平均值
          Tmp = Ch[B]- B_Posi;
          b_Ssq +=(Tmp * Tmp);                      // 统计B通道平方和
            
          if(Ch[A] > a_Max)  a_Max = Ch[A];         // 统计A通道最大值  
          if(Ch[A] < a_Min)  a_Min = Ch[A];         // 统计A通道最小值  
          if(Ch[B] > b_Max)  b_Max = Ch[B];         // 统计B通道最大值  
          if(Ch[B] < b_Min)  b_Min = Ch[B];         // 统计B通道最小值  
           
          C_D = DataBuf[i] >>16;
          if((i>1)&&(i<4094)){
            if(Sa == 2){TaS = i; Ta = i; PaS = 0;}
            if(Ch[A] > a_Mid_H){
              if(Sa == 0){ TaS = i; TaN++;} 
              Sa = 1;  
            } else { 
              if(Ch[A] < a_Mid_L) if(Sa == 1){Sa = 0; PaS += i-TaS;}
            }
            if(Sb == 2){TbS = i; Tb = i; PbS = 0;}
            if(Ch[B] > b_Mid_H){
              if(Sb == 0){ TbS = i; TbN++;} 
              Sb = 1;  
            } else { 
              if(Ch[B] < b_Mid_L) if(Sb == 1){Sb = 0; PbS += i-TbS;}
            }
            if(Sc == 2){TcS = i; Tc = i; PcS = 0;}
            if(C_D & 1){
              if(Sc == 0){ TcS = i; TcN++;} 
              Sc = 1;  
            } else {
              if(Sc == 1){Sc = 0; PcS += i-TcS;} 
            }
            if(Sd == 2){TdS = i; Td = i; PdS = 0;}
            if(C_D & 2){
              if(Sd == 0){ TdS = i; TdN++;} 
              Sd = 1;  
            } else {
              if(Sd == 1){Sd = 0; PdS += i-TdS;} 
            }
          }
     
      if(i >= k){                               // 指针到达指定窗口位置
        V[A]  = Ka1[_A_Range] +(Ka2[_A_Range] *Ch[A]+ 512)/1024;      
        V[B]  = Kb1[_B_Range] +(Kb2[_B_Range] *Ch[B]+ 512)/1024;      //当前点的主值
        while(j > 0 ){
          Send_Data( V[A_]+((V[A]-V[A_])*(1024 - j))/1024, //当前CH_A点的插值
                     V[B_]+((V[B]-V[B_])*(1024 - j))/1024, //当前CH_B点的插值
                     C_D,                                  //当前点数字通道值
                     n++);
          j -= _Kp1+5;
          if(n >= X_SIZE+TRACK_OFFSET){ k = 8192;  break;}     //300
        }
        j += 1024;
        V[A_] = V[A];  V[B_] = V[B];     
      }
     }
  } 
  else {                            // 交替采样模式
    k =((1024 -_Kp2)*150 + 512)/1024 + _X_posi.Value;  // 计算插值运算后窗口位置的修正值
    for(i=0; i <bag_max_buf; i++){
      if(_Status == RUN)  DataBuf[i] = __Read_FIFO(); // 读入32位FIFO数据, 读后指针+1
      C_D    = DataBuf[i] >>16;
      if(_2_source == HIDE){                            // B通道合并到A通道时
        Ch[A]  = (DataBuf[i] & 0xFF );              
        Ch[B]  = ((DataBuf[i] >> 8) & 0xFF);//+ Kab;
        Tmp    = (Ch[A]- A_Posi);
        a_Ssq += (Tmp * Tmp)/2;
        Tmp    = (Ch[B]- A_Posi);
        a_Ssq += (Tmp * Tmp)/2;                         // 统计平方和                    
      } else {                                          // A通道合并到B通道时
        Ch[B]  = (DataBuf[i] & 0xFF );//- Kab;              
        Ch[A]  = ((DataBuf[i] >> 8) & 0xFF);  
        Tmp    = (Ch[A]- B_Posi);
        a_Ssq += (Tmp * Tmp)/2;
        Tmp    = (Ch[B]- B_Posi);
        a_Ssq += (Tmp * Tmp)/2;                                    // 统计平方和                    
      }
      a_Avg += (Ch[A]+Ch[B])/2;                                   // 累计直流平均值                                   
      if(Ch[A] > a_Max)  a_Max = Ch[A];          
      if(Ch[B] > a_Max)  a_Max = Ch[B];                          // 统计最大值 
      if(Ch[A] < a_Min)  a_Min = Ch[A];         
      if(Ch[B] < a_Min)  a_Min = Ch[B];                          // 统计最小值  

      if(i >= k){                 // 第1点指针到达指定窗口位置
        if(_2_source == HIDE){                            // B通道合并到A通道时
          V[A] = Ka1[_A_Range] +(Ka2[_A_Range]*Ch[A]+ 512)/1024;    //计算当前第1点的主值
          V[B] = Ka1[_A_Range] +(Ka2[_A_Range]*Ch[B]+ 512)/1024;    //计算当前第2点的主值
        } else {                                          // A通道合并到B通道时
          V[A] = Kb1[_B_Range] +(Kb2[_B_Range]*Ch[A]+ 512)/1024;   //计算当前第1点的主值
          V[B] = Kb1[_B_Range] +(Kb2[_B_Range]*Ch[B]+ 512)/1024;   //计算当前第2点的主值
        }
        while(j > 0 ){
          Tmp = V[B_]+((V[A]- V[B_])*(1024 - j))/1024; //当前第1点的插值
          Send_Data(Tmp, Tmp, C_D, n++);
          j -= _Kp2+5;
          if(n >= X_SIZE+TRACK_OFFSET){ k = 8192;  break;} //300
        }
        j += 1024;
      }
      if(i >= k){                 // 第2点指针到达指定窗口位置
        while(j > 0 ){
          Tmp = V[A]+((V[B]- V[A])*(1024 - j))/1024;  //当前第2点的插值
          Send_Data(Tmp, Tmp, C_D, n++);
          j -= _Kp2+5;
          if(n >= X_SIZE+TRACK_OFFSET){ k = 8192;  break;} //300
        }
        j += 1024;
        V[B_] = V[B];       
      }
    }
    b_Avg  = a_Avg;
    b_Ssq  = a_Ssq;
    b_Max  = a_Max;
    b_Min  = a_Min;

    if(_1_source == HIDE)  a_Avg = _1_posi*4096;   // A通道合并到B通道时      
    if(_2_source == HIDE)  b_Avg = _2_posi*4096;   // B通道合并到A通道时
  }

  if ((FrameMode!=0) && (_Mode==SCAN)) __Set(FIFO_CLR, W_PTR);
  
  
  a_Mid_H = 4 +(a_Max + a_Min)/2;
  a_Mid_L = a_Mid_H - 8;
  b_Mid_H = 4 +(b_Max + b_Min)/2;
  b_Mid_L = b_Mid_H - 8;

  TaS -= Ta; TbS -= Tb; TcS -= Tc; TdS -= Td;

// for(j=0; j<4; j++){                               // 消除屏幕端点连线
//   
//    TrackBuff[(  0)*4+ j] = TrackBuff[(  1)*4 + j];
//   TrackBuff[(299)*4+ j] = TrackBuff[(298)*4 + j];
//  
//
// }
 
 
    
 for(j=0; j<X_SIZE; j++){                               // 消除屏幕端点连线
   
  
  TrackBuff[(j)*4] = TrackBuff[(j+TRACK_OFFSET)*4];
  TrackBuff[(j)*4+1] = TrackBuff[(j+TRACK_OFFSET)*4+1];
  TrackBuff[(j)*4+2] = TrackBuff[(j+TRACK_OFFSET)*4+2];
  TrackBuff[(j)*4+3] = TrackBuff[(j+TRACK_OFFSET)*4+3];
   

 }
 
 
 
}

void Send_Data(s16 Va, s16 Vb, u8 C_D, u16 n)  //输出显示数据
{
  s32 Tmp = 0, i;

  i = n*4;
  if(Va >= Y_BASE+Y_SIZE)  TrackBuff[i + TRACK1] = Y_BASE+Y_SIZE-1;
  else if(Va <= Y_BASE+1)  TrackBuff[i + TRACK1] = Y_BASE+1;
  else TrackBuff[i + TRACK1] = Va;
  if(Vb >= Y_BASE+Y_SIZE)  TrackBuff[i + TRACK2] = Y_BASE+Y_SIZE-1;
  else if(Vb <= Y_BASE+1)  TrackBuff[i + TRACK2] = Y_BASE+1;
  else TrackBuff[i + TRACK2] = Vb;
  
  if(C_D & 1)  TrackBuff[i + TRACK3] = c_Max;
  else         TrackBuff[i + TRACK3] = _3_posi;
  
  switch (_4_source){                       
  case A_add_B:
    if(Interlace == 0) Tmp = Posi_412 + Va + Vb;
    else{
      if(_1_source != HIDE) Tmp = Posi_41  + Va;
      if(_2_source != HIDE) Tmp = Posi_42  + Vb;
    } break;
  case A_sub_B:
    if(Interlace == 0) Tmp = Posi_412 + Va - Vb;
    else{
      if(_1_source != HIDE) Tmp = Posi_41  + Va;
      if(_2_source != HIDE) Tmp = Posi_4_2 - Vb;
    } break;
  case C_and_D:
    if((~C_D)& 3) Tmp = d_Max; 
    else          Tmp = _4_posi;
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
  default:
    if(C_D & 2)  Tmp = d_Max;
    else         Tmp = _4_posi;
  }
  if(Tmp >= Y_BASE+Y_SIZE)  TrackBuff[i + TRACK4] = Y_BASE+Y_SIZE-1;
  else if(Tmp <= Y_BASE+1)  TrackBuff[i + TRACK4] = Y_BASE+1;
  else                      TrackBuff[i + TRACK4] = Tmp;
}
/*******************************************************************************
 Synchro: 扫描同步处理，按设定模式显示波形 
*******************************************************************************/
void Synchro(void)  //扫描同步方式共有：AUTO、NORM、SGL、NONE、SCAN 5种模式
{ 
 // u16  i;

  switch (_Mode){ 
    case X_Y_A:
  case AUTO:
    __Set(TRIGG_MODE,(_Tr_source <<3)+_Tr_kind);  // 设触发条件
    
    if(__Get(FIFO_START)!=0){
      Process();                                  // 生成新的显示波形
     
      Wait_Cnt = Wait[_T_base];
    } else if(Wait_Cnt==0){
      
   //  if (FrameMode==0){
      
      if(JumpCnt >= 4095)  JumpCnt = 0; 
   //    }
   //   else{
   //     if(JumpCnt >= (FrameMode*300)-1){ __Set(FIFO_CLR, W_PTR); JumpCnt = 0;}
   //   }
      
      
      Process();   
    
      Wait_Cnt = Wait[_T_base];
    } break;
  case NORM:
    __Set(TRIGG_MODE,(_Tr_source <<3)+_Tr_kind);  // 设触发条件
    if(__Get(FIFO_START)!=0){
      Process();                                  // 生成新的显示波形
      Wait_Cnt = Wait[_T_base];
    } else if(Wait_Cnt==0){
    //  for(i=0; i<4*X_SIZE; ++i)  TrackBuff[i] = 0;// 清除旧的显示波形
      Wait_Cnt = Wait[_T_base];
    } break;
  case SGL:
    __Set(TRIGG_MODE,(_Tr_source <<3)+_Tr_kind);  // 设触发条件
    if(__Get(FIFO_START)!=0)  Process();          // 生成新的显示波形
    break;
  
  case X_Y:
  case SCAN:
    __Set(TRIGG_MODE, UNCONDITION);               // 设为无条件触发
    Process();                                    // 生成当前显示波形
  
   }
  Draw_Window();                                  // 刷新屏幕波形显示区
  
  if ((_Mode==SCAN) || (_Mode==X_Y)) Wait_Cnt = 1;
 
  if ((FrameMode>0) && (_Status == RUN)){   //_Mode == SCAN &&
    Wait_Cnt = 1;
    
   
    
    if (_Mode==SCAN)
      {
          if ((FlagMeter==0) && (JumpCnt> ((390*FrameMode)-1))) JumpCnt=0;
          if ((FlagMeter==1) && (JumpCnt> ((305*FrameMode)-1))) JumpCnt=0;
         
      } 
    else
      {
        if (JumpCnt> ((300*FrameMode)-1)) JumpCnt=0;	// bag   -1 ((X_SIZE*FrameMode)-1))
      }
    

        
  // __Set(FIFO_CLR, W_PTR);
    
  }

  if((_Status == RUN)&&(__Get(FIFO_FULL)!=0)){    // FIFO is full
    __Set(FIFO_CLR, W_PTR);                       // FIFO写指针复位
    Wait_Cnt = Wait[_T_base];
    JumpCnt =0;
    if(_Mode == SGL){
      _Status = HOLD;                             // 一帧完后，进入暂停
      _State.Flag |= UPDAT;
    }
//    if(_Mode == SCAN){
//      Wait_Cnt = 1;
//      for(i=0; i<X_SIZE; i++){                    // 重建当前轨迹基线                   
//        TrackBuff[i*4 + TRACK1] = _1_posi;
//        TrackBuff[i*4 + TRACK2] = _2_posi; 
//        TrackBuff[i*4 + TRACK3] = _3_posi; 
//        TrackBuff[i*4 + TRACK4] = _4_posi;
//      }
//    
//    }
  }    
}  
/******************************** END OF FILE *********************************/
