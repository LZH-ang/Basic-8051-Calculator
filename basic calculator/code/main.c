
#include <STC89C5xRC.H>
/*****************************************CONFIG****************************************/
/*  
**@SEG:P0
**@BIT:P1.0-4
**@KEY:P2
 */
#define SEG P0
#define KEY P2
#define BIT P1
#define KEYDE 123
data const unsigned char seg[]  = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
data unsigned int  result[] = {0,0,0};
data const unsigned char key_num[] = {8,5,9,13,6,10,14,7,11,15};//键盘编码数组
unsigned char keynum = 0,numbit = 0;														//读取键盘键值,每次最多读四位
bit keystate = 0;																								//检测按键状态,控制数据只处理一次每次按下
unsigned char OPER;																							//+ - * /
sbit DIG1 = P1^3;
sbit DIG2 = P1^2;
sbit DIG3 = P1^1;
sbit DIG4 = P1^0;

/***************************************FUNCTIONS*****************************************/

void Display(unsigned int num);
unsigned char Key(void);
unsigned int CALC(void);
void Delay(unsigned int x);

/****************************************MAIN*********************************************/

void main(void)
{
  SEG = 0xff;
  BIT = 0xff;
  while(1)
  {
		keynum = Key();
    Display(CALC());
  }
}

/*****************************************************************************************/
/****************************************CODE*********************************************/
/*****************************************************************************************/

void Display(unsigned int num)
{
  static unsigned int numstate=10000;//检测数据变化,控制计算
  static unsigned char num1,num2,num3,num4;
  static bit flag = 1;
  if(numstate == num)flag = 0;else flag = 1;
  if(flag)
  {
    num1 = num%10;
    num2 = num%100/10;
    num3 = num/100%10;
    num4 = num/1000;
  }
  numstate = num;
  //以上程序为了减少单片机运算量
  SEG = 0xff;BIT = 0xff;
	if(num4 < 10)
	{
		if(num4 != 0)
		{
			DIG1 = 0;SEG = seg[num1];DIG1 = 1;SEG = 0xff;
			DIG2 = 0;SEG = seg[num2];DIG2 = 1;SEG = 0xff;
			DIG3 = 0;SEG = seg[num3];DIG3 = 1;SEG = 0xff;
			DIG4 = 0;SEG = seg[num4];DIG4 = 1;SEG = 0xff;
		}
		else if(num3 != 0)
		{
			DIG1 = 0;SEG = seg[num1];DIG1 = 1;SEG = 0xff;
			DIG2 = 0;SEG = seg[num2];DIG2 = 1;SEG = 0xff;
			DIG3 = 0;SEG = seg[num3];DIG3 = 1;SEG = 0xff;	
		}
		else if(num2 != 0)
		{
			DIG1 = 0;SEG = seg[num1];DIG1 = 1;SEG = 0xff;
			DIG2 = 0;SEG = seg[num2];DIG2 = 1;SEG = 0xff;
		}
		else
		{
			DIG1 = 0;SEG = seg[num1];DIG1 = 1;SEG = 0xff;
		}
	}
	else//OVER VALUE
	{
		DIG1 = 0;SEG = 0xce;DIG1 = 1;SEG = 0xff;
		DIG2 = 0;SEG = 0x86;DIG2 = 1;SEG = 0xff;
		DIG3 = 0;SEG = 0xc1;DIG3 = 1;SEG = 0xff;
		DIG4 = 0;SEG = 0xc0;DIG4 = 1;SEG = 0xff;		
	}
}

unsigned char Key(void)//矩阵键盘扫描
{
	static unsigned char i=0,m=0,n=0;
	//KEY = 0x0f;//0000 1111 触发后：~0xf1,~0xf2,~0xf4,~0xf8
	KEY = 0x0f;
	keystate = 0;
	if(KEY != 0x0f)
	{
		m = KEY;
		if(KEY == 0x0f)return KEYDE;//消除误触发，消抖
		KEY = 0xf0;//1111 0000 触发后：~0x1f,~0x2f,~0x4f,~0x8f
		if(KEY != 0xf0)n = KEY;
		keystate = 1;
	}	
	KEY = 0x0f;
	while(KEY != 0x0f);
	
	if(m == 14)i = 0;				//0000 1110 ~0xf1
	else if(m == 13)i = 1;	//0000 1101 ~0xf2
	else if(m == 11)i = 2;	//0000 1011 ~0xf4
	else if(m == 7)i = 3;		//0000 0111 ~0xf8
	
	if(n == 224)return i+ 0;				//1110 0000	~0x1f				
	else if(n == 208)return i + 4;	//1101 0000	~0x2f
	else if(n == 176)return i + 8;	//1011 0000	~0x4f
	else if(n == 112)return i + 12; //0111 0000	~0x8f	
	
	return KEYDE;
}

/******************************************************************************************
**@计算器数据处理核心代码
**@键盘布局
** | 15  11  7  3 |					| 9  8  7  + |
** | 14  10  6  2 |  ---->	| 6  5  4  - |
** | 13  9   5  1 |  ---->	| 3  2  1  * |
** | 12  8   4  0 |					| C  0  =  / |
*/
unsigned int CALC(void)
{
	static unsigned int num = 0;
#if 0	//OLD CODE
	unsigned char i;
	if(keystate)//检测到按下
	{	
		if(keynum == 12){result[0] = 0;result[1] = 0;result[2] = 0;num = 0;numbit = 0;OPER = 0;keystate = 0;}	// 'CLEAR'
		else if(keynum == 3){OPER = 1;result[0] = num;num = 0;numbit = 0;keystate = 0;}												// '+'
		else if(keynum == 2){OPER = 2;result[0] = num;num = 0;numbit = 0;keystate = 0;}												// '-'
		else if(keynum == 1){OPER = 3;result[0] = num;num = 0;numbit = 0;keystate = 0;}												// '*'
		else if(keynum == 0){OPER = 4;result[0] = num;num = 0;numbit = 0;keystate = 0;}												// '/'
		else if(keynum == 4)			// '='
		{		
			result[1] = num;
			if(OPER == 1){result[2] = result[0] + result[1];}
			else if(OPER == 2){result[2] = result[0] - result[1];}
			else if(OPER == 3){result[2] = result[0] * result[1];}
			else if(OPER == 4){result[2] = result[0] / result[1];}
			OPER = 0;numbit = 0;keystate = 0;
			num = result[2];
		}
		else for(i=0;i<10;i++)		// '0-9'
		{
			if(key_num[i] == keynum)//查询键值,得到数字
			{				
				if(numbit <= 3){num = num*10+i;numbit++;}
				keystate = 0;//复位
				break;				
			}		
		}
	}
#else
	if(keystate)//检测到按下
	{	
		switch(keynum)
		{
		/*'0'*/case  8:{if((numbit > 0)&&(numbit <= 3)){num = num*10+0;numbit++;}keystate = 0;}break;
		/*'1'*/case  5:{if(numbit <= 3){num = num*10+1;numbit++;}keystate = 0;}break;
		/*'2'*/case  9:{if(numbit <= 3){num = num*10+2;numbit++;}keystate = 0;}break;
		/*'3'*/case 13:{if(numbit <= 3){num = num*10+3;numbit++;}keystate = 0;}break;
		/*'4'*/case  6:{if(numbit <= 3){num = num*10+4;numbit++;}keystate = 0;}break;
		/*'5'*/case 10:{if(numbit <= 3){num = num*10+5;numbit++;}keystate = 0;}break;
		/*'6'*/case 14:{if(numbit <= 3){num = num*10+6;numbit++;}keystate = 0;}break;
		/*'7'*/case  7:{if(numbit <= 3){num = num*10+7;numbit++;}keystate = 0;}break;
		/*'8'*/case 11:{if(numbit <= 3){num = num*10+8;numbit++;}keystate = 0;}break;
		/*'9'*/case 15:{if(numbit <= 3){num = num*10+9;numbit++;}keystate = 0;}break;
		/*'C'*/case 12:{result[0] = 0;result[1] = 0;result[2] = 0;num = 0;numbit = 0;OPER = 0;keystate = 0;}break;
		/*'+'*/case 3:{OPER = 1;result[0] = num;num = 0;numbit = 0;keystate = 0;}break;
		/*'-'*/case 2:{OPER = 2;result[0] = num;num = 0;numbit = 0;keystate = 0;}break;
		/*'*'*/case 1:{OPER = 3;result[0] = num;num = 0;numbit = 0;keystate = 0;}break;
		/*'/'*/case 0:{OPER = 4;result[0] = num;num = 0;numbit = 0;keystate = 0;}break;
		/*'='*/case 4:{
								result[1] = num;
								if(OPER == 1){result[2] = result[0] + result[1];}
								else if(OPER == 2){result[2] = result[0] - result[1];}
								else if(OPER == 3){result[2] = result[0] * result[1];}
								else if(OPER == 4){result[2] = result[0] / result[1];}
								OPER = 0;numbit = 4;keystate = 0;
								num = result[2];
								}break;
		}
	}
#endif
	return num;
}

/*****************************************************************************************/

#if 0
void Delay(unsigned int x)
{
  while(x--);
}
#endif