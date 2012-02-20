/********************* (C) COPYRIGHT 2010 e-Design Co.,Ltd. ********************
 File Name : File.c  
 Version   : DS203_APP Ver 2.5x                                  Author : bure
*******************************************************************************/
#include <string.h>
#include "Function.h"
#include "Process.h"
#include "BIOS.h"
#include "File.h"
#include "Menu.h"


u8  SecBuff[512];
u8  FileBuff[1600];
u16 TempPar[66];
u16 BMP_Color[16] = { WHT,  CYAN, CYAN_,  YEL,  YEL_, PURPL, PURPL_, GRN,   
                      GRN_, GRAY, ORANGE, BLUE, RED,  BLACK, BLACK,  BLACK};    
u8  BmpHead[54]   = { 0X42, 0X4D, 0XF8, 0XB, 0X00, 0X00, 0X00, 0X00, 
                      0X00, 0X00, 0X76, 0X0, 0X00, 0X00, 0X28, 0X00,
                      0X00, 0X00, 0X90, 0X1, 0X00, 0X00, 0XF0, 0X00,
                      0X00, 0X00, 0X01, 0X0, 0X04, 0X00, 0X00, 0X00,
                      0X00, 0X00, 0X82, 0XB, 0X00, 0X00, 0X12, 0X0B,
                      0X00, 0X00, 0X12, 0XB, 0X00, 0X00, 0X00, 0X00,
                      0X00, 0X00, 0X00, 0X0, 0X00, 0X00};

/*******************************************************************************
 Open the specified file extension input: The file extension return value: 0x00 = successful 
*******************************************************************************/
u8 Make_Filename(u8 FileNum, char* FileName)
{
  char Num[4];
  
  u8ToDec3(Num, FileNum);
  FileName[4]  = Num[0];
  FileName[5]  = Num[1];
  FileName[6]  = Num[2];
//  FileName[12] = 0;
  return 0;
} 
/*******************************************************************************
 Obtained corresponds to the current color palette number
*******************************************************************************/
u8 Color_Num(u16 Color)
{
  if(Color == WHT)                   return 0;
  else if((Color & CYAN  )== CYAN  ) return 1;
  else if((Color & YEL   )== YEL   ) return 3;
  else if((Color & PURPL )== PURPL ) return 5;
  else if((Color & GRN   )== GRN   ) return 7;
  else if((Color & CYAN_ )== CYAN_ ) return 2;
  else if((Color & YEL_  )== YEL_  ) return 4;
  else if((Color & PURPL_)== PURPL_) return 6;
  else if((Color & GRN_  )== GRN_  ) return 8;
  else if((Color & GRAY  )== GRAY  ) return 9;
  else if((Color & ORANGE)== ORANGE) return 10;
  else if((Color & BLUE  )== BLUE  ) return 11;
  else if((Color & RED   )== RED   ) return 12;
  else                               return 13;
}
/*******************************************************************************
Load_Dat: Load the saved screen image of the original data input: The file number is the return value: 0x00 = successful 
*******************************************************************************/
u8 Load_Dat(u8 FileNum)
{
  char  Filename[12] = "FILE    DAT"; 
  u16 i;
  
  u16 pCluster[3];
  u32 pDirAddr[1]; 
  
  Make_Filename(FileNum, Filename);
  i = __OpenFileRd(SecBuff, Filename, pCluster, pDirAddr);
  if(i != OK) return i;
  for(i=0; i<4; i++){
    if(__ReadFileSec(SecBuff, pCluster)!= OK) return RD_ERR;
    memcpy(&FileBuff[i*400], SecBuff,400);
  }
  return 0;
}
/*******************************************************************************
Save_Dat: save the current screen to display the original image data input: The file number is the return value: 0x00 = successful 
*******************************************************************************/
u8 Save_Dat(u8 FileNum)
{
  char  Filename[13] = "FILE    DAT"; 
  u16 i, j;
  u16 pCluster[3];
  u32 pDirAddr[1]; 
  
  Make_Filename(FileNum, Filename);
  if(__OpenFileWr(SecBuff, Filename, pCluster, pDirAddr)!=OK) return DISK_ERR;
  memset(SecBuff, 0, 512);
  for(j=0; j<4; j++){
    for(i=0; i<399; i++)  SecBuff[i+TRACK_OFFSET] = TrackBuff[i*4 + j];
    SecBuff[399] = Title[j][POSI].Value;//299
    if(__ProgFileSec(SecBuff, pCluster)!= OK) return WR_ERR; // write data
    Print_Clk(254, 0, (SCRN<<8)+ TEXT1, j & 3);              // progress indication
  }
  if(__CloseFile(SecBuff, 0x0800, pCluster, pDirAddr)!= OK) return WR_ERR;
  return OK;
}
/*******************************************************************************
Save_Bmp: to save the current screen image as BMP format input: file number returns the value: 0x00 = successful 
*******************************************************************************/
u8 Save_Bmp(u8 FileNum)
{
  char  Filename[12] = "IMAG    BMP"; 
  u16 k, i, j, x=0, y=0, ColorH, ColorL;

  u16 pCluster[3];
  u32 pDirAddr[1]; 

//   u8 Num[10];  
/* 
  Make_Filename(FileNum, Filename);
  if(__OpenFileWr(SecBuff, Filename, pCluster, pDirAddr)!=OK) return DISK_ERR;
//  
  Shor2Hex(Num, *pCluster); /////////////////////////////// 
  __Display_Str( 0*8, 220, WHT, PRN, Num);/////////////////
  Shor2Hex(Num, *(pCluster+1)); /////////////////////////// 
  __Display_Str( 5*8, 220, GRN, PRN, Num);////////////////
//  
  memcpy(SecBuff, BmpHead, 54);
  i = 0x0036; // palette to store the starting address
  for(j=0; j<16; ++j){
    SecBuff[j*4 +i+0]=(BMP_Color[j]& 0xF800)>>8; // Bule
    SecBuff[j*4 +i+1]=(BMP_Color[j]& 0x07E0)>>3; // Green
    SecBuff[j*4 +i+2]=(BMP_Color[j]& 0x001F)<<3; // Red
    SecBuff[j*4 +i+3]= 0;                        // Alpha
  }
  i = 0x0076; // the image data stored address
  k = 0;
  for(y=0; y<240; y++){
    for(x=0; x<400 ; x+=2){
      __Point_SCR(x, y);
      ColorH =__LCD_GetPixl();
      __Point_SCR(x+1, y);
      ColorL =__LCD_GetPixl();
      SecBuff[i] =(Color_Num(ColorH)<<4)+ Color_Num(ColorL);
      i++;
      if(i>=512){
        if(__ProgFileSec(SecBuff, pCluster)!= OK) return WR_ERR; // write data
        Print_Clk(254, 0, (SCRN<<8)+ TEXT1, (k++ >>1)& 3);    // progress indicator
        i=0; 
//   
//  Filename[11] = 0;  
//  __Display_Str(10*8, 220, WHT, PRN, Filename);
  Char2Hex(Num, *pCluster);  
  __Display_Str(k*4*8, 220, WHT, PRN, Num);
//  Char2Hex(Num, *(pCluster+1));  
//  __Display_Str(28*8, 220, WHT, PRN, Num);
  while(1);
  Char2Hex(Num, k);////////////////////////////////  
  __Display_Str(20*8, 215, YEL, PRN, Num);/////////
  Shor2Hex(Num, *pCluster); /////////////////////// 
  __Display_Str(25*8, 215, WHT, PRN, Num);/////////
  Shor2Hex(Num, *(pCluster+1));///////////////////  
  __Display_Str(30*8, 215, GRN, PRN, Num);////////
  Key_Buffer = 0;/////////////////////////////////
  while(!Key_Buffer);/////////////////////////////
//
      }
    }
  }
  if(i!=0) if(__ProgFileSec(SecBuff, pCluster)!= OK) return WR_ERR; // write data
//  while(1);
  if(__CloseFile(SecBuff, 0xBC00, pCluster, pDirAddr)!= OK) return WR_ERR;
  return 0; 
  k=0;
 Char2Hex(Num, *(pCluster+1)); __Display_Str((k%24)*16, (k/24)*15, WHT, PRN, Num); k++;
 Char2Hex(Num, *pCluster    ); __Display_Str((k%24)*16, (k/24)*15, GRN, PRN, Num); k++;
 if(*pCluster > 0x154){Key_Buffer = 0; while(Key_Buffer == 0) {} }
// if(k >= 16*24){__Clear_Screen(BLACK); k = 0;}
*/ 
  
  Make_Filename(FileNum, Filename);
  if(__OpenFileWr(SecBuff, Filename, pCluster, pDirAddr)!=OK) return DISK_ERR;
  memcpy(SecBuff, BmpHead, 54);
  i = 0x0036; // palette to store the starting address
  for(j=0; j<16; ++j){
    SecBuff[j*4 +i+0]=(BMP_Color[j]& 0xF800)>>8; // Bule
    SecBuff[j*4 +i+1]=(BMP_Color[j]& 0x07E0)>>3; // Green
    SecBuff[j*4 +i+2]=(BMP_Color[j]& 0x001F)<<3; // Red
    SecBuff[j*4 +i+3]= 0;                        // Alpha
  }
  i = 0x0076; // the image data stored address
  k = 0;
  for(y=0; y<240; y++){
    for(x=0; x<400 ; x+=2){
      __Point_SCR(x, y);
      ColorH =__LCD_GetPixl();
      __Point_SCR(x+1, y);
      ColorL =__LCD_GetPixl();
      SecBuff[i] =(Color_Num(ColorH)<<4)+ Color_Num(ColorL);
      i++;
      if(i>=512){
        if(__ProgFileSec(SecBuff, pCluster)!= OK) return WR_ERR; // write data
// Char2Hex(Num, *(pCluster+1)); __Display_Str((k%24)*16, (k/24)*15, WHT, PRN, Num); k++;
// Char2Hex(Num, *pCluster    ); __Display_Str((k%24)*16, (k/24)*15, GRN, PRN, Num); k++;
// if(*pCluster > 0){Key_Buffer = 0; while(Key_Buffer == 0) {} }
// if(k >= 16*24){__Clear_Screen(BLACK); k = 0;}
        Print_Clk(254, 0, (SCRN<<8)+ TEXT1, (k++ >>1)& 3);    // progress indicator
        i=0; 
      }
    }
  }
  if(i!=0) if(__ProgFileSec(SecBuff, pCluster)!= OK) return WR_ERR; // write data
  if(__CloseFile(SecBuff, 0xBC00, pCluster, pDirAddr)!= OK) return WR_ERR;
  return 0; 
}
/*******************************************************************************
Save_Buf: save the data collection buffer BUF format input: the file number returns the value: 0x00 = successful      
*******************************************************************************/
u8 Save_Buf(u8 FileNum)
{
  u8   i;
  char Filename[12] = "DATA    BUF"; 
  u16* p ;
  
  u16 pCluster[3];
  u32 pDirAddr[1]; 
  
  Make_Filename(FileNum, Filename);
  if(__OpenFileWr(SecBuff, Filename, pCluster, pDirAddr)!=OK) return DISK_ERR;
  for(i=0; i<32; i++){
    memcpy(SecBuff, &(DataBuf[i*512/4]), 512);
    if(__ProgFileSec(SecBuff, pCluster)!= OK) return WR_ERR; // write data
    Print_Clk(254, 0, (SCRN<<8)+ TEXT1, (i >>1)& 3);        // progress indication
  }
  memset(SecBuff, 0, 512);
  p =(u16*)SecBuff;
  for(i=0; i<4; i++){                     // save each corresponding value in the display menu
    *p++ = Title[i][0].Value;
    *p++ = Title[i][1].Value;
    *p++ = Title[i][2].Value;
    *p++ = Title[i][3].Value;
  }
  *p++ = Title[5][0].Value;
  *p++ = Title[5][1].Value;

  *p++ = 0x00FF & Ka1[_A_Range];
  *p++ = Ka2[_A_Range];
  *p++ = 0x00FF & Kb1[_B_Range];
  *p++ = Kb2[_B_Range];

  if(__ProgFileSec(SecBuff, pCluster)!= OK) return WR_ERR; // write data
  if(__CloseFile(SecBuff, 0x4200, pCluster, pDirAddr)!= OK) return WR_ERR;
  return 0;
}
/*******************************************************************************
Load_Dat: load saved data acquisition buffer input: the file number returns the value: 0x00 = successful
*******************************************************************************/
u8 Load_Buf(u8 FileNum)
{
  u8  i;
  char Filename[13] = "DATA    BUF"; 
  u16 *p;
  
  u16 pCluster[3];
  u32 pDirAddr[1]; 
  
  p = TempPar;
  *p++ = 0xAA55;
  for(i=0; i<4; i++){                     // save each corresponding value in the display menu
    *p++ = Title[i][0].Value;
    *p++ = Title[i][1].Value;
    *p++ = Title[i][2].Value;
    *p++ = Title[i][3].Value;
  }
  *p++ = Title[5][0].Value;
  *p++ = Title[5][1].Value;
  for(i=0; i<8; i++){
    *p++ = 0x00FF & Ka1[i];
    *p++ = Ka2[i];
    *p++ = 0x00FF & Kb1[i];
    *p++ = Kb2[i];
  }
  Make_Filename(FileNum, Filename);
  i = __OpenFileRd(SecBuff, Filename, pCluster, pDirAddr);
  if(i != OK) return i;
  for(i=0; i<32; i++){
    if(__ReadFileSec(SecBuff, pCluster)!= OK) return RD_ERR;
    memcpy(&(DataBuf[i*512/4]),SecBuff,512);
  }
  if(__ReadFileSec(SecBuff, pCluster)!= OK) return RD_ERR;
  p =(u16*)SecBuff;
  for(i=0; i<4; i++){
    Title[i][0].Value = *p++;             // load before each corresponding value in the display menu title
    Title[i][1].Value = *p++ ;
    Title[i][2].Value = *p++;
    Title[i][3].Value = *p++;
    Title[i][0].Flag  = UPDAT;
    Title[i][1].Flag  = UPDAT;
    Title[i][2].Flag  = UPDAT;
    Title[i][3].Flag  = UPDAT;
  }
  Title[5][0].Value = *p++;
  Title[5][1].Value = *p++;
  Title[5][0].Flag  = UPDAT;
  Title[5][1].Flag  = UPDAT;
      
  Ka1[_A_Range] = (*p++ );
  Ka2[_A_Range] = (*p++ );
  Kb1[_B_Range] = (*p++);
  Kb2[_B_Range] = (*p++); /**/

  Title[RUNNING][STATE].Value = 1;                 // set to "HOLD" status
  Title[RUNNING][STATE].Flag |= UPDAT;             // set the update flag
  return 0;
}

void reset_parameter(void)
{
  u16* p;
  u8   i;
  
  p=TempPar;
  p++;
  if(TempPar[0]!=0xAA55) return;
  for(i=0; i<4; i++){
    Title[i][0].Value = *p++;             // load before each corresponding value in the display menu title
    Title[i][1].Value = *p++;
    Title[i][2].Value = *p++;
    Title[i][3].Value = *p++;
    Title[i][0].Flag  = UPDAT;
    Title[i][1].Flag  = UPDAT;
    Title[i][2].Flag  = UPDAT;
    Title[i][3].Flag  = UPDAT;
  }
  Title[5][0].Value = *p++;
  Title[5][1].Value = *p++;
  Title[5][0].Flag  = UPDAT;
  Title[5][1].Flag  = UPDAT;
  
  for(i=0; i<8; i++){
    Ka1[i] = (s8)(*p++ );//& 0xff);
    Ka2[i] = (*p++ );
    Kb1[i] = (s8)(*p++);// & 0xff);
    Kb2[i] = (*p++);
  }
  p = TempPar;
  *p++ = 0;
}

void make_Vertical(u8 TRACK,u8* buf,u8* len)
{
  u8 i=0;
  char* ptr;

  ptr = &Vertical[0][0] + 10*Title[TRACK][2].Value;
  while(*ptr != 0){
    if(*ptr == 0x21)  buf[i] = 0x20;
    else              buf[i] = *ptr;
    ptr++;
    i++;
  };
  buf[i]   = 0x2c;
  buf[i+1] = 0;
  *len = i+1;
}
/*******************************************************************************
Save_Csv: save the acquisition buffer input for the CSV format: file number returns the value: 0x00 = successful      
*******************************************************************************/
u8 Save_Csv(u8 FileNum)
{
  u8  track[4]; 
  char Num[4];
  char Filename[12] = "DATA    CSV"; 
  u32 i, k = 0;
  s16 temp;
  u8  count, j, n = 0;
  
  u16 pCluster[3];
  u32 pDirAddr[1]; 

  Make_Filename(FileNum, Filename);
  if(__OpenFileWr(SecBuff, Filename, pCluster, pDirAddr)!=OK) return DISK_ERR;
  memcpy(SecBuff, "TRACK1 ", 7);
  make_Vertical(TRACK1, &SecBuff[7], &count);
  k = 7 + count;
  memcpy(&SecBuff[k], "TRACK2 ", 7);
  make_Vertical(TRACK2, &SecBuff[k+7], &count);
  k += 7 + count;
  memcpy(&SecBuff[k], "TRACK3,TRACK4,\r\n", 15);
  k += 15;
  for(i=0; i<4096; i++){
    temp=Ka1[_A_Range]+((Ka2[_A_Range]*(DataBuf[i] & 0xff))+512)/1024;
    if(temp > 0){
      if(temp > 200)  track[0] = 199;
      else            track[0] = temp;
    } else            track[0] = 0;
    temp = Kb1[_B_Range]+((Kb2[_B_Range]*(DataBuf[i] & 0xff00)>>8)+512)/1024;
    if(temp > 0){
      if(temp > 200)  track[1] = 199;
      else            track[1] = temp;
    } else            track[1] = 0;
    if((DataBuf[i] & 0x010000)==0)  track[2] = Title[TRACK3][POSI].Value;
    else                            track[2] = 20 + Title[TRACK3][POSI].Value;
    if((DataBuf[i] & 0x020000)==0)  track[3] = Title[TRACK4][POSI].Value;
    else                            track[3] = 20 + Title[TRACK4][POSI].Value;
    for(j=0; j<4; j++){
      u8ToDec3(Num, track[j]);
      for(count=0; count<3; count++){
        if(Num[count] == 0) break;
        SecBuff[k++] = Num[count];
        if(k >= 512){
          if(__ProgFileSec(SecBuff, pCluster)!= OK) return WR_ERR; // write data
          Print_Clk(254, 0, (SCRN<<8)+ TEXT1, (n++ >>1)& 3);    // progress indicator
          k = 0;
        }
      }
      SecBuff[k++] = 0x2c;
      if(k >= 512){
        if(__ProgFileSec(SecBuff, pCluster)!= OK) return WR_ERR; // write data
        Print_Clk(254, 0, (SCRN<<8)+ TEXT1, (n++ >>1)& 3);    // progress indicator
        k = 0;
      }
    }
    SecBuff[k++] = 0x0d;
    if(k >= 512){
      if(__ProgFileSec(SecBuff, pCluster)!= OK) return WR_ERR; // write data
      Print_Clk(254, 0, (SCRN<<8)+ TEXT1, (n++ >>1)& 3);    // progress indicator
      k = 0;
    }
    SecBuff[k++] = 0x0a;
    if(k >= 512){
      if(__ProgFileSec(SecBuff, pCluster)!= OK) return WR_ERR; // write data
      Print_Clk(254, 0, (SCRN<<8)+ TEXT1, (n++ >>1)& 3);    // progress indicator
      k = 0;
    }
  }
  if(k != 0){
    SecBuff[k++]=0x0d;
    SecBuff[k++]=0x0a;
    memset(&SecBuff[k],0,(512-k));
    k=0;
    if(__ProgFileSec(SecBuff, pCluster)!= OK) return WR_ERR; // write data
    Print_Clk(254, 0, (SCRN<<8)+ TEXT1, (n >>1)& 3);    // progress indicator
  }
  if(__CloseFile(SecBuff, n*512, pCluster, pDirAddr)!= OK) return WR_ERR;
  return OK;
}
/*******************************************************************************
Load_Parameter: before loading the operating parameters Return: 0 = Success,
*******************************************************************************/
u8 Load_Param(void)
{ 
  u8  Sum = 0, Versions = 0x06; 
  char Filename[12];
  u16 i;
  u16* p =(u16*)SecBuff;
  
  u16 pCluster[3];
  u32 pDirAddr[1]; 
  
  Word2Hex(Filename, __GetDev_SN());
  Filename[8] = 'W'; Filename[9] = 'P'; Filename[10] = 'T'; Filename[11] = 0; 
  i = __OpenFileRd(SecBuff, Filename, pCluster, pDirAddr);
  if(i != OK) return i;
//  if(__OpenFileRd(SecBuff, Filename, pCluster, pDirAddr) != OK) return NO_FILE;
  if(__ReadFileSec(SecBuff, pCluster)!= OK) return RD_ERR;
  if(Versions !=(*p & 0xFF)) return VER_ERR;          // version of the error return
  for(i=0; i<512; ++i) Sum += SecBuff[i];
  if(Sum != 0) return SUM_ERR;                        // checksum error return
  Current =(*p++ >>8);                                // load before Current Title
  for(i=0; i<7; i++){ 
    Detail[i*2]  = *p;
    Detail[i*2+1]= (*p++ >>8);                        // load before the Detail
  }
  for(i=0; i<13; i++){
    Title[i][0].Value = *p++;                         // load before each corresponding value in the display menu title
    Title[i][1].Value = *p++;
    Title[i][2].Value = *p++;
    Title[i][3].Value = *p++;
  }
  for(i=0; i<9; i++){
    Meter[i].Item     = *p;
    Meter[i].Track    =(*p++ >>8);       // load measurements and the measurement object before the meter
  }
  for(i=0; i<10; i++){
    Ka1[i] = *p;                         // restore the original channel A low error correction coefficient
    Kb1[i] =(*p++ >>8);                  // restore the original B-channel low error correction coefficient
    Ka2[i] = *p++;                       // restore the original channel A gain error correction factor
    Kb2[i] = *p++;                       // load before the B-channel gain error correction coefficient
    Ka3[i] = *p;                         // restore the original channel A high error correction coefficient
    Kb3[i] =(*p++ >>8);                  // restore the original B-channel high error correction factor
  }
  V_Trigg[A].Value = *p++;
  V_Trigg[B].Value = *p++;               // restore the original A and B channel trigger threshold
  FlagFrameMode= *p++;
  FlagMeter= *p++;
  TrgAuto=*p++;
  OffsetX=*p++;
  OffsetY=*p++;
  
  
 
  return OK;
}
/*******************************************************************************
 Save_Parameter: save the current operating parameters of the Return: 0 = Success,
*******************************************************************************/
u8 Save_Param(void)             // save the operating parameters table
{
  u8  Sum = 0, Versions = 0x06; 
  char Filename[12];
  u16 i, Tmp[2];
  u16* p =(u16*)SecBuff;
  
  u16 pCluster[3];
  u32 pDirAddr[1]; 

  Word2Hex(Filename, __GetDev_SN());
  Filename[8] = 'W'; Filename[9] = 'P'; Filename[10] = 'T';
  switch (__OpenFileRd(SecBuff, Filename, pCluster, pDirAddr)){
  case OK:                                                     // original WPT file exists
    Tmp[0] = *pCluster;
    Filename[8] = 'B'; Filename[9] = 'A'; Filename[10] = 'K';  // turn into a BAK file
    if(__OpenFileWr(SecBuff, Filename, pCluster, pDirAddr)!= OK) return DISK_ERR;
    if(__ReadFileSec(SecBuff, Tmp     )!= OK) return RD_ERR;
    if(__ProgFileSec(SecBuff, pCluster)!= OK) return WR_ERR;  // save the BAK file
    if(__CloseFile(SecBuff, 512, pCluster, pDirAddr)!= OK) return WR_ERR;/**/
  case NEW:                                                    // original WPT file does not exist
    Filename[8] = 'W'; Filename[9] = 'P'; Filename[10] = 'T';  // create WPT files
    if(__OpenFileWr(SecBuff, Filename, pCluster, pDirAddr)!= OK) return DISK_ERR;
    memset(SecBuff, 0, 512);
    *p++ =(Current <<8)+ Versions;           // save the parameter table version number and the current Title
    for(i=0; i<7; i++){ 
      *p++ =(Detail[i*2+1]<<8)+ Detail[i*2]; // Save the Detail
    }
    for(i=0; i<13; i++){                     // Save display each value of the corresponding item in the menu
      *p++ = Title[i][0].Value;
      *p++ = Title[i][1].Value;
      *p++ = Title[i][2].Value;
      *p++ = Title[i][3].Value;
    }
    for(i=0; i<9; i++){
      *p++ =(Meter[i].Track<<8)+ Meter[i].Item; // Save the measurement items and measurement object
    }
    for(i=0; i<10; i++){
      *p++ = (Kb1[i]<<8)+ Ka1[i];             // save the current A and B channel low error correction coefficient
      *p++ =  Ka2[i];                         // save the current channel A gain error correction coefficient
      *p++ =  Kb2[i];                         // save the current B-channel gain error correction factor
      *p++ = (Kb3[i]<<8)+ Ka3[i];             // save the current, B, channel high error correction factor
    }
    *p++ = V_Trigg[A].Value;
    *p++ = V_Trigg[B].Value;                  // save the current A and B channels trigger threshold
    *p++ = FlagFrameMode;
    *p++ = FlagMeter;
    *p++=TrgAuto;
    *p++=OffsetX;
    *p++=OffsetY; 
    
    for(i=0; i<511; i++)  Sum += SecBuff[i];  // calculate the parameter table checksum
    SecBuff[511] = (~Sum)+ 1;
    if(__ProgFileSec(SecBuff, pCluster)!= OK) return WR_ERR; // write data
    if(__CloseFile(SecBuff, 512, pCluster, pDirAddr)!= OK) return WR_ERR;
    return OK;
    default:  return WR_ERR;
  }
}

/******************************** END OF FILE *********************************/

