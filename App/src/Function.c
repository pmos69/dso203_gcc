/********************* (C) COPYRIGHT 2010 e-Design Co.,Ltd. ********************
File Name : Function.c  
Version   : DS203 APP Ver 2.5x                          Author : bure & Kewei
*******************************************************************************/
#include "Interrupt.h"
#include "Function.h"
#include "BIOS.h"
#include "Menu.h"
#include "Process.h"

// u32 TestCnt = 0;

// Since we only use 3/4 of N_WAVE, we define only
// this many samples, in order to conserve data space.
const short Sinewave[N_WAVE-N_WAVE/4] = {
	0,	402,	804,	1206,	1608,	2009,	2410,	2811,
	3212,	3612,	4011,	4410,	4808,	5205,	5602,	5998,
	6393,	6786,	7179,	7571,	7962,	8351,	8739,	9126,
	9512,	9896,	10278,	10659,	11039,	11417,	11793,	12167,
	12539,	12910,	13279,	13645,	14010,	14372,	14732,	15090,
	15446,	15800,	16151,	16499,	16846,	17189,	17530,	17869,
	18204,	18537,	18868,	19195,	19519,	19841,	20159,	20475,
	20787,	21096,	21403,	21705,	22005,	22301,	22594,	22884,
	23170,	23452,	23731,	24007,	24279,	24547,	24811,	25072,
	25329,	25582,	25832,	26077,	26319,	26556,	26790,	27019,
	27245,	27466,	27683,	27896,	28105,	28310,	28510,	28706,
	28898,	29085,	29268,	29447,	29621,	29791,	29956,	30117,
	30273,	30424,	30571,	30714,	30852,	30985,	31113,	31237,
	31356,	31470,	31580,	31685,	31785,	31880,	31971,	32057,
	32137,	32213,	32285,	32351,	32412,	32469,	32521,	32567,
	32609,	32646,	32678,	32705,	32728,	32745,	32757,	32765,
	32767,	32765,	32757,	32745,	32728,	32705,	32678,	32646,
	32609,	32567,	32521,	32469,	32412,	32351,	32285,	32213,
	32137,	32057,	31971,	31880,	31785,	31685,	31580,	31470,
	31356,	31237,	31113,	30985,	30852,	30714,	30571,	30424,
	30273,	30117,	29956,	29791,	29621,	29447,	29268,	29085,
	28898,	28706,	28510,	28310,	28105,	27896,	27683,	27466,
	27245,	27019,	26790,	26556,	26319,	26077,	25832,	25582,
	25329,	25072,	24811,	24547,	24279,	24007,	23731,	23452,
	23170,	22884,	22594,	22301,	22005,	21705,	21403,	21096,
	20787,	20475,	20159,	19841,	19519,	19195,	18868,	18537,
	18204,	17869,	17530,	17189,	16846,	16499,	16151,	15800,
	15446,	15090,	14732,	14372,	14010,	13645,	13279,	12910,
	12539,	12167,	11793,	11417,	11039,	10659,	10278,	9896,
	9512,	9126,	8739,	8351,	7962,	7571,	7179,	6786,
	6393,	5998,	5602,	5205,	4808,	4410,	4011,	3612,
	3212,	2811,	2410,	2009,	1608,	1206,	804,	402,
	0,	-402,	-804,	-1206,	-1608,	-2009,	-2410,	-2811,
	-3212,	-3612,	-4011,	-4410,	-4808,	-5205,	-5602,	-5998,
	-6393,	-6786,	-7179,	-7571,	-7962,	-8351,	-8739,	-9126,
	-9512,	-9896,	-10278,	-10659,	-11039,	-11417,	-11793,	-12167,
	-12539,	-12910,	-13279,	-13645,	-14010,	-14372,	-14732,	-15090,
	-15446,	-15800,	-16151,	-16499,	-16846,	-17189,	-17530,	-17869,
	-18204,	-18537,	-18868,	-19195,	-19519,	-19841,	-20159,	-20475,
	-20787,	-21096,	-21403,	-21705,	-22005,	-22301,	-22594,	-22884,
	-23170,	-23452,	-23731,	-24007,	-24279,	-24547,	-24811,	-25072,
	-25329,	-25582,	-25832,	-26077,	-26319,	-26556,	-26790,	-27019,
	-27245,	-27466,	-27683,	-27896,	-28105,	-28310,	-28510,	-28706,
	-28898,	-29085,	-29268,	-29447,	-29621,	-29791,	-29956,	-30117,
	-30273,	-30424,	-30571,	-30714,	-30852,	-30985,	-31113,	-31237,
	-31356,	-31470,	-31580,	-31685,	-31785,	-31880,	-31971,	-32057,
	-32137,	-32213,	-32285,	-32351,	-32412,	-32469,	-32521,	-32567,
	-32609,	-32646,	-32678,	-32705,	-32728,	-32745,	-32757,	-32765,
};

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
	u8 numdigits=e;
	char id=pUnit[0];
	s32 nn=n;

	if(n == 0x80000000) {               // This value is defined as an invalid value
		*p++ = ' ';
		*p++ = '.';
		while(--e)  *p++ = ' ';
		if ((id=='u')&&(numdigits==3))*p++=' '; 
		*p = 0;  return;
	}
	if(Mode == SIGN) {
		if(n>0) {
			*p++ = '+';
			v='+';
		}
		if(n<0) {
			*p++ = '-';
			n=-n;
			v='-';
		}
		if(n==0) {
			*p++ = '0';
			*p = '.';
		}
	}
	m = n;
	i = 0;
	c = 5;
	while(m >= 10) {                    // detection of n significant digits
		m /= 10;
		if(++i > e)    // number of significant digits than e calculate the rounded median
		c *= 10;
	}
	if(i >= e)                  // plus the rounded value
		n += c;
	m = n;
	i = 0;
	while(m >= 10) {                    // re-detection of n significant digits
		m /= 10;
		i++;
	}
	i++;   
	if(e > 3) {
		m = (i-1)/3;
		i %= 3;
		if(i == 0)
		i = 3;
	} else {
		m = i/3;
		i %= 3;
	}

	p += e;
	k = p+1;  				 					// pointer to where suffix will go
	while(n >= Power(10, e))	
		n /= 10;  									// interception of the highest e effective number of bits
	for(j=0; j<m; j++)								// move pointer m "slots" to nul in front of desired string
		while(*pUnit++);
	if ((_T_Range<3)&&(id=='u')&&(numdigits==3)) while(*pUnit++);	// special case for very large deltaT intervals, move to proper suffix
	do *k++ = *pUnit;								// copy desired string
	while(*pUnit++); // dimensional character string

	while(e--) {
		*p-- = '0'+(n%10);								// copy the digits of the value
		n /= 10;
		if((Mode != STD)&&(m > 0)&&(i == e)) {//&&(Mode != SIGN)
			*p-- = '.';
		}
	}
	if((Mode!=STD)&&(Mode!=SIGN)&&(m==0)) {
		p++; //*p++;
		*p-- = '.';
		*p= '0';
	}

	if((Mode==STD)&&(m==0))
		*p-- = ' ';

	if((Mode == SIGN)&&(p[1] == '.')&&(p[0] != '0')){
		if ((id=='u')&&(numdigits==3)){				// identify time vernier, no need for +, allow suffix to show properly
			p[0]='0';
		}else{
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
	if ((id=='u')&&(numdigits==3)){				// identify time vernier
		if (p[0]=='+') p[0]=' ';					// strip off "+" sign on rest of readings to match values that start with 0
		if (nn<100) {p--; p[0]='0'; p[1]='.';}			// special case for very small values
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
/*{ u32 k;					
if ( n == 0 ) return 0;
k = 2*Int_sqrt(n/4)+1;
if ( k*k > n ) return k-1;
else return k;
}*/

{						
	unsigned int c = 0x8000;
	unsigned int g = 0x8000;

	for(;;) {				
		if(g*g > n)                 
		g ^= c;                 
		c >>= 1;				
		if(c == 0)			
		return g;
		g |= c;				
	}
}

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
		if(Key_Status & K_ITEM_D_STATUS)   KeyCode = K_ITEM_DEC;    // K9    right toggle -
		if(Key_Status & K_ITEM_S_STATUS)   KeyCode = K_ITEM_S;      // K10
		if(Key_Status & KEY3_STATUS)       KeyCode = KEY3;          // K3 
		if(Key_Status & KEY4_STATUS)       KeyCode = KEY4;          // K4 
		if(Key_Status & K_INDEX_D_STATUS)  KeyCode = K_INDEX_DEC;   // K5   left toggle -
		if(Key_Status & K_INDEX_I_STATUS)  KeyCode = K_INDEX_INC;   // K6   left toggle +
		if(Key_Status & K_INDEX_S_STATUS)  KeyCode = K_INDEX_S;     // K7  
		if(Key_Status & KEY2_STATUS)       KeyCode = KEY2;          // K2
		if(Key_Status & KEY1_STATUS)       KeyCode = KEY1;         // K1
		if(Key_Status & K_ITEM_I_STATUS)   KeyCode = K_ITEM_INC;    // K8   right toggle +
	} else {
		if(Key_Status_Now & Key_Status_Last){       // Key push hold on
			if((Key_Wait_Cnt || Key_Repeat_Cnt)==0){
				if(Key_Status_Now & K_INDEX_D_STATUS)  KeyCode = K_INDEX_DEC;   // K5 
				if(Key_Status_Now & K_INDEX_I_STATUS)  KeyCode = K_INDEX_INC;   // K6

				if((Key_Status_Now & K_INDEX_D_STATUS)||(Key_Status_Now & K_INDEX_I_STATUS)){   
					// Auto repeat per 5= 100mS
					switch (Current){											//fast toggle repeat rate for:
					case T_VERNIE:							      			//time cursors
					case V_VERNIE:											//volt cursors
						Key_Repeat_Cnt = 2;
						break;
					case TRIGG:	
						if (Detail[Current]==2) Key_Repeat_Cnt = 2; else Key_Repeat_Cnt = 5;		//trigger cursors
						break;
					case OUTPUT:
						if ((_Det==2)||(_Det==3))Key_Repeat_Cnt = 2; else Key_Repeat_Cnt = 5;  		//PWM duty cycle or out level
						break;
					case T_BASE:
						if (Detail[Current]==2) Key_Repeat_Cnt = 2; else Key_Repeat_Cnt = 5;		//xpos in large buffer mode
						break;
					case TRACK1:
					case TRACK2:
					case TRACK3:
					case TRACK4:
						if (Detail[Current]==3) Key_Repeat_Cnt = 2; else Key_Repeat_Cnt = 5;		//ypos in track menus
						break;
					default:
						Key_Repeat_Cnt = 5;
					}
				}
				if(Key_Status_Now & K_ITEM_D_STATUS)   {KeyCode = K_ITEM_DEC;  Key_Repeat_Cnt = 5;}    // K9 
				if(Key_Status_Now & K_ITEM_I_STATUS)   {KeyCode = K_ITEM_INC;  Key_Repeat_Cnt = 5;}    // K8 
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

/************************************************************************

	FFT Audio Analysis

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

************************************************************************/

// Fixed-point in-place Fast Fourier Transform
// All data are fixed-point short integers, in which -32768
// to +32768 represent -1.0 to +1.0 respectively. Integer
// arithmetic is used for speed, instead of the more natural
// floating-point.
//
// For the forward FFT (time -> freq), fixed scaling is
// performed to prevent arithmetic overflow, and to map a 0dB
// sine/cosine wave (i.e. amplitude = 32767) to two -6dB freq
// coefficients.
//
// Written by:  Tom Roberts  11/8/89
// Made portable:  Malcolm Slaney 12/15/94 malcolm@interval.com
// Enhanced:  Dimitrios P. Bouras  14 Jun 2006 dbouras@ieee.org
// Ported to PIC18F:  Simon Inns 20110104
// Ported to stm32:  Pedro Simões 2012-03-10	pmos69@hotmail.com

/*
fix_fft() - perform forward fast Fourier transform.
fr[n],fi[n] are real and imaginary arrays, both INPUT AND
RESULT (in-place FFT), with 0 <= n < 2**m
*/
void fix_fft(short fr[], short fi[], short size)		// m = number of bits of the FFT size (ex: 1024 -> 10 bits)
{
	long int mr = 0, nn, i, j, l, k, istep, n;
	short qr, qi, tr, ti, wr, wi, m;
	
	// static const int log2helper[32] = {
	// 0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8, 
	// 31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9};

	// m = log2helper[(unsigned int)(size * 0x077CB531U) >> 27];
	m = LOG2_FFTSize;
	
	n = size;

	nn = n - 1;

	/* max FFT size = N_WAVE */
	//if (n > N_WAVE) return -1;

	/* decimation in time - re-order data */
	for (m=1; m<=nn; ++m)
	{
		l = n;
		do
		{
			l >>= 1;
		} while (mr+l > nn);
		
		mr = (mr & (l-1)) + l;
		if (mr <= m) continue;
		
		tr = fr[m];
		fr[m] = fr[mr];
		fr[mr] = tr;
		ti = fi[m];
		fi[m] = fi[mr];
		fi[mr] = ti;
	}

	l = 1;
	k = LOG2_N_WAVE-1;
	
	while (l < n)
	{
		/*
		fixed scaling, for proper normalization --
		there will be log2(n) passes, so this results
		in an overall factor of 1/n, distributed to
		maximize arithmetic accuracy.

		It may not be obvious, but the shift will be
		performed on each data point exactly once,
		during this pass.
		*/
		
		// Variables for multiplication code
		long int c;
		short b;
		
		istep = l << 1;
		for (m=0; m<l; ++m)
		{
			j = m << k;
			/* 0 <= j < N_WAVE/2 */
			wr =  Sinewave[j+N_WAVE/4];
			wi = -Sinewave[j];

			wr >>= 1;
			wi >>= 1;
			
			for (i=m; i<n; i+=istep)
			{
				j = i + l;
				
				// Multiplications unrolled to prevent overhead
				// for procedural calls (the stm32 has an ALU with
				// H/W divide and single cycle multiply):
				
				// tr = FIX_MPY(wr,fr[j]) - FIX_MPY(wi,fi[j]);
				c = ((long int)wr * (long int)fr[j]);
				c = c >> 14;
				b = c & 0x01;
				tr = (c >> 1) + b;
				
				c = ((long int)wi * (long int)fi[j]);
				c = c >> 14;
				b = c & 0x01;
				tr = tr - ((c >> 1) + b);
				
				// ti = FIX_MPY(wr,fi[j]) + FIX_MPY(wi,fr[j]);
				c = ((long int)wr * (long int)fi[j]);
				c = c >> 14;
				b = c & 0x01;
				ti = (c >> 1) + b;
				
				c = ((long int)wi * (long int)fr[j]);
				c = c >> 14;
				b = c & 0x01;
				ti = ti + ((c >> 1) + b);
				
				qr = fr[i];
				qi = fi[i];
				qr >>= 1;
				qi >>= 1;

				fr[j] = qr - tr;
				fi[j] = qi - ti;
				fr[i] = qr + tr;
				fi[i] = qi + ti;
			}
		}
		
		--k;
		l = istep;
	}
}



/********************************* END OF FILE ********************************/
