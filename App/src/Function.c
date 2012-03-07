/********************* (C) COPYRIGHT 2010 e-Design Co.,Ltd. ********************
 File Name : Function.c  
 Version   : DS203 APP Ver 2.5x                          Author : bure & Kewei
*******************************************************************************/
#include "Interrupt.h"
#include "Function.h"
#include "BIOS.h"

// u32 TestCnt = 0;

/*******************************************************************************
 
*******************************************************************************/
u32 Power(u8 x, u8 y) 
{ 
  u32 m = x;
  
  if(y == 0) return 1;
  while (--y) m *= x;
  return m;
}
/*******************************************************************************
 delayms:  milliseconds (mS) time delay procedure		Input: milliseconds to wait (72MHz frequency case)
*******************************************************************************/
void Delayms(u16 mS) 
{ 
  Delay_Cnt = mS;
  while (Delay_Cnt > 0){}
}




/*******************************************************************************
 Sign_int2Str: 32-digit switch to e-bit effective number of strings + dimensionless string
******************************************************************************/

void Int2Str(char *p, long n, char *pUnit, u8 e, u8 Mode)
{
  long i, j, m, c;
  char  *k;
  u8  v=0;
  
  if(n == 0x80000000)
  {               // This value is defined as an invalid value
    *p++ = ' ';
	*p++ = '.';
    while(--e)  *p++ = ' '; 
    *p = 0;  return;
  }
  if(Mode == SIGN)
  {
    if(n>0)
    {
      *p++ = '+';
      v='+';
    }
    if(n<0)
    {
      *p++ = '-';
      n=-n;
      v='-';
    }
    if(n==0)
    {
     *p++ = '0';
     *p = '.';
    }
  }
  m = n;
  i = 0;
  c = 5;
  while(m >= 10)                    // detection of n significant digits
  {
     m /= 10;
	 if(++i > e)    // number of significant digits than e calculate the rounded median
		c *= 10;
  }
  if(i >= e)                  // plus the rounded value
	n += c;
  m = n;
  i = 0;
  while(m >= 10)                    // re-detection of n significant digits
  {
	m /= 10;
	i++;
  }
  i++;   
  if(e > 3)
  {
    m = (i-1)/3;
    i %= 3;
    if(i == 0)
		i = 3;
  }
  else
  {
    m = i/3;
    i %= 3;
  }
  
  p += e;
  k = p+1;  
  while(n >= Power(10, e))
	n /= 10;  // interception of the highest e effective number of bits
  for(j=0; j<m; j++)
	while(*pUnit++);
	
  do *k++ = *pUnit;
  while(*pUnit++); // dimensional character string
 
  while(e--)
  {
    *p-- = '0'+(n%10);
	n /= 10;
    if((Mode != STD)&&(m > 0)&&(i == e))//&&(Mode != SIGN)
	{
		*p-- = '.';
	}
  }
  if((Mode!=STD)&&(Mode!=SIGN)&&(m==0))
  {
    p++; //*p++;
    *p-- = '.';
    *p= '0';
  }
 
  if((Mode==STD)&&(m==0))
    *p-- = ' ';

  if((Mode == SIGN)&&(p[1] == '.')&&(p[0] != '0'))
  {
    p[7]=0;
    p[6]=p[5];
    p[5]=p[4];
    p[4]=p[3];
    p[3]=p[2];
    p[2]='.';
    p[1]='0';
    if(v=='-')
     p[0]='-';
    else
     p[0]='+';
   }
}


/*******************************************************************************
 Two ASCII character Change to 1 Byte HEX data 
*******************************************************************************/
u8 Str2Byte(char x,char y) // double-ASCII characters to 1 byte binary number
{
  uc8 Hexcode[17]="0123456789ABCDEF";
  u8 i, Temp=0;
  
  if(x>='a' && x<='z')  x-=32;     // lowercase change to uppercase
  if(y>='a' && y<='z')  y-=32;     // lowercase change to uppercase
  for(i=0;i<16;i++){
    if(Hexcode[i]==x)  Temp+=i*16; // characters into the high four hexadecimal values
  }
  for(i=0;i<16;i++){
    if(Hexcode[i]==y)  Temp+=i;    // characters into four hexadecimal values
  }
  return Temp;
}

/*******************************************************************************
 u16ToDec5: Change 2 Byte to 5 decimal number string   
*******************************************************************************/
void u16ToDec5(char *p, u16 n)
{
    *p++ = '0'+n/10000;
    n %= 10000;
    *p++ = '0'+n/1000;
    n %= 1000;
    *p++ = '0'+n/100;
    n %= 100;
    *p++ = '0'+n/10;
    n %= 10;
    *p++ = '0'+n;
    *p = 0;
}
/*******************************************************************************
 u8ToDec3: Change Byte to 3 decimal number string   
*******************************************************************************/
void u8ToDec3(char *p, u8 n)
{
    *p++ = '0'+n/100;
    n %= 100;
    *p++ = '0'+n/10;
    n %= 10;
    *p++ = '0'+n;
    *p = 0;
}
/*******************************************************************************
 s8ToPercen: Change sign char to +(-)x.xx string   
*******************************************************************************/
void s8ToPercen(char *p, s8 n)
{
    if(n >= 0)  *p++ = '+';
    else {
      *p++ = '-';
      n = -n;
    }
    *p++ = '0'+n/100;
    n %= 100;
    *p++ = '.';
    *p++ = '0'+n/10;
    n %= 10;
    *p++ = '0'+n;
    *p = 0;
}
/*******************************************************************************
 u8ToDec2: Change Byte to 2 decimal number string   
*******************************************************************************/
void u8ToDec2(char *p, u8 n)
{
//    *p++ = '0'+n/100;
    n %= 100;
    *p++ = '0'+n/10;
    n %= 10;
    *p++ = '0'+n;
    *p = 0;
}
/*******************************************************************************
 Char2Hex: Change Byte to 2 hex number string  
*******************************************************************************/
void Char2Hex(char *p, u8 n)
{
    if(n/16>9) *p++ = 'A'+(n/16-10);
    else       *p++ = '0'+n/16;
    n %= 16;
    if(n>9)    *p++ = 'A'+(n-10);
    else       *p++ = '0'+n;
    *p = 0;
}
/*******************************************************************************
 Shor2Hex: Change 2Bytes to 4 hex number string  
*******************************************************************************/
void Shor2Hex(char *p, u16 n)
{
    if(n/0x1000 >9) *p++ = 'A'+(n/0x1000-10);
    else            *p++ = '0'+ n/0x1000;
    n %= 0x1000;
    if(n/0x100 >9)  *p++ = 'A'+(n/0x100-10);
    else            *p++ = '0'+ n/0x100;
    n %= 0x100;
    if(n/0x10 >9)   *p++ = 'A'+(n/0x10-10);
    else            *p++ = '0'+ n/0x10;
    n %= 0x10;
    if(n >9)        *p++ = 'A'+(n-10);
    else            *p++ = '0'+n;
    *p = 0;
}
/*******************************************************************************
 Word2Hex: Change 4 Bytes to 8 hex number string  
*******************************************************************************/
void Word2Hex(char *p, u32 n)
{
    if(n/0x10000000>9) *p++ = 'A'+(n/0x10000000-10);
    else               *p++ = '0'+ n/0x10000000;
    n %= 0x10000000;
    if(n/0x01000000>9) *p++ = 'A'+(n/0x01000000-10);
    else               *p++ = '0'+ n/0x01000000;
    n %= 0x01000000;
    if(n/0x00100000>9) *p++ = 'A'+(n/0x00100000-10);
    else               *p++ = '0'+ n/0x00100000;
    n %= 0x00100000;
    if(n/0x00010000>9) *p++ = 'A'+(n/0x00010000-10);
    else               *p++ = '0'+ n/0x00010000;
    n %= 0x00010000;
    if(n/0x00001000>9) *p++ = 'A'+(n/0x00001000-10);
    else               *p++ = '0'+ n/0x00001000;
    n %= 0x00001000;
    if(n/0x00000100>9) *p++ = 'A'+(n/0x00000100-10);
    else               *p++ = '0'+ n/0x00000100;
    n %= 0x00000100;
    if(n/0x00000010>9) *p++ = 'A'+(n/0x00000010-10);
    else               *p++ = '0'+ n/0x00000010;
    n %= 0x00000010;
    if(n/0x00000001>9) *p++ = 'A'+(n/0x00000001-10);
    else               *p++ = '0'+ n/0x00000001;
    n %= 0x10000000;
    *p = 0;
}
/*******************************************************************************
 Int_sqrt: unsigned int square root
*******************************************************************************/
u32 Int_sqrt(u32 n)

{ u32 k;
  if ( n == 0 ) return 0;
  k = 2*Int_sqrt(n/4)+1;
  if ( k*k > n ) return k-1;
  else return k;
}

// {
    // unsigned int c = 0x8000;
    // unsigned int g = 0x8000;

    // for(;;) {
        // if(g*g > n)
            // g ^= c;
        // c >>= 1;
        // if(c == 0)
            // return g;
        // g |= c;
    // }
// }

/*******************************************************************************
Read_Keys:    Key status process                                Return: KeyCode
*******************************************************************************/
u8 Read_Keys(void)
{
  u16 Key_Status, Key_Status_Now;
  u8  KeyCode=0;
  
  Key_Status_Now = ~__Get(KEY_STATUS);//~KeyScan();
  Key_Status = Key_Status_Now &(~Key_Status_Last);
  if(Key_Status){                               // New key push on
    Key_Wait_Cnt=30;                        // Close auto repeat 25=500mS
    if(Key_Status & K_ITEM_D_STATUS)   KeyCode = K_ITEM_DEC;    // K9
    if(Key_Status & K_ITEM_S_STATUS)   KeyCode = K_ITEM_S;      // K10
    if(Key_Status & KEY3_STATUS)       KeyCode = KEY3;          // K3 
    if(Key_Status & KEY4_STATUS)       KeyCode = KEY4;          // K4 
    if(Key_Status & K_INDEX_D_STATUS)  KeyCode = K_INDEX_DEC;   // K5 
    if(Key_Status & K_INDEX_I_STATUS)  KeyCode = K_INDEX_INC;   // K6  
    if(Key_Status & K_INDEX_S_STATUS)  KeyCode = K_INDEX_S;     // K7  
    if(Key_Status & KEY2_STATUS)       KeyCode = KEY2;          // K2
    if(Key_Status & KEY1_STATUS)       KeyCode = KEY1;         // K1
    if(Key_Status & K_ITEM_I_STATUS)   KeyCode = K_ITEM_INC;    // K8
  } else {
    if(Key_Status_Now & Key_Status_Last){       // Key push hold on
      if((Key_Wait_Cnt || Key_Repeat_Cnt)==0){
        if(Key_Status_Now & K_INDEX_D_STATUS)  KeyCode = K_INDEX_DEC;    // K5 
        if(Key_Status_Now & K_INDEX_I_STATUS)  KeyCode = K_INDEX_INC;    // K6
        if(Key_Status_Now & K_ITEM_D_STATUS)   KeyCode = K_ITEM_DEC;     // K9 
        if(Key_Status_Now & K_ITEM_I_STATUS)   KeyCode = K_ITEM_INC;     // K8 
        Key_Repeat_Cnt = 2;                 // Auto repeat per 5= 100mS
      }
    }
  }
  Key_Status_Last = Key_Status_Now;
  return KeyCode;
} 

char * long2str(long val)
{
	static char numbuf[32];
	static char vec[] = "0123456789ABCDEF";
	u8 base = 10;
	char *p = &numbuf[32];
	int sign = (base > 0);

	*--p = '\0';		/* null-terminate string	*/
	if (val) {
		if (base > 0) {
			if (val < 0L) {
				long v1 = -val;
				if (v1 == val)
					goto overflow;
				val = v1;
			}
			else
				sign = 0;
		}
		else
		if (base < 0) {			/* unsigned */
			base = -base;
			if (val < 0L) {	/* taken from Amoeba src */
				int mod, i;
			overflow:
				mod = 0;
				for (i = 0; i < 8 * sizeof val; i++) {
					mod <<= 1;
					if (val < 0)
						mod++;
					val <<= 1;
					if (mod >= base) {
						mod -= base;
						val++;
					}
				}
				*--p = vec[mod];
			}
		}
		do {
			*--p = vec[(int) (val % base)];
			val /= base;
		} while (val != 0L);
		if (sign)
			*--p = '-';	/* don't forget it !!	*/
	}
	else
		*--p = '0';		/* just a simple 0	*/
	return p;
}
/********************************* END OF FILE ********************************/
