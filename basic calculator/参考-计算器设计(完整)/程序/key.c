unsigned int a_num,b_num,c_num;
unsigned char pk,key_dat,cp,num_d[8],num[8],num_a[8],num_b[8];
unsigned char  key(void)
{
	unsigned char i = 88;
	P2 = 0xef;
	if(P2 == 0xee)i = 1;
 	if(P2 == 0xed)i = 4;
	if(P2 == 0xeb)i = 7;
	if(P2 == 0xe7)i = 23;//清零
	P2 = 0xdf;
	if(P2 == 0xde)i = 2;
 	if(P2 == 0xdd)i = 5;
	if(P2 == 0xdb)i = 8;
	if(P2 == 0xd7)i = 0;//零	
	P2 = 0xbf;
	if(P2 == 0xbe)i = 3;
 	if(P2 == 0xbd)i = 6;
	if(P2 == 0xbb)i = 9;
	if(P2 == 0xb7)i = 24;//等号
	P2 = 0x7f;
	if(P2 == 0x7e)i = 12;//加
 	if(P2 == 0x7d)i = 13;//减
	if(P2 == 0x7b)i = 14;//乘
	if(P2 == 0x77)i = 15;//除
	return(i);
}
void dat_conv_a(void)	//数据a移位
{
	unsigned char i = 7;
	while(i){num_a[i] = num_a[i - 1];i--;};
}
void dat_conv_b(void)	//数据b移位
{
	unsigned char i = 7;
	while(i){num_b[i] = num_b[i - 1];i--;};
}
//void dat_conv(void)		//显示数据移位
//{
//	unsigned char i = 7;
//	while(i){num[i] = num[i - 1];i--;};
//}
void dat_clear(void)	//清零
{
	unsigned char i = 8;
	while(i){num_a[i - 1] = 0;num_b[i - 1] = 0;num[i - 1] = 0;i--;}
}
//void zero_ct(void)		//灭零控制
//{
//	unsigned char i = 7;
//	while(i)
//	{
//		if((num[i] == 0) || (num[i] == 10 )){num[i] = 10;i--;}
//		if((num[i] != 0) && (num[i] != 10))i = 0;		
//	}
//}
void key_do(void)
{
	unsigned char i;
	i = key();
	if(i <= 9)//数字按键按下
	{
		if(pk == 0)
		{
			dat_conv_a();
			num_a[0] = i;
//			dat_conv();
//			num[0] = num_a[0];		
			i = 0;
			a_num = num_a[3] * 1000 + num_a[2] * 100 + num_a[1] * 10 + num_a[0];
		}
		if(pk != 0)
		{
			dat_conv_b();			
			num_b[0] = i;
//			dat_conv();	
//			num[0] = num_b[0];		
			i = 0;
			b_num = num_b[3] * 1000 + num_b[2] * 100 + num_b[1] * 10 + num_b[0];
		}
			  	
	}
	if((i >=12) && (i <= 15))	//运算符按下
	{
		if(pk == 0)
		{
			dat_conv();
						
			if(i == 12)pk = 1;num[0] = 10;
			if(i == 13)pk = 2;num[0] = 10;
			if(i == 14)pk = 3;num[0] = 10;
			if(i == 15)pk = 4;num[0] = 10;
		}				
	}
	if(i == 23)dat_clear();		//清零操作
	if(i == 24)					//等号按键按下
	{
		dat_clear();		
		if(pk == 1)c_num = a_num + b_num;
		if(pk == 2)c_num = a_num - b_num;
		if(pk == 3)c_num = a_num * b_num;
		if(pk == 4)c_num = a_num / b_num;
		num[4] =  c_num / 10000;
		num[3] =  c_num / 1000 % 10;
		num[2] =  c_num % 1000 / 100;
		num[1] =  c_num % 100  / 10;
		num[0] =  c_num % 10;
		a_num = 0;
		b_num = 0;
		pk = 0;	
	}
	zero_ct();	
	while(i < 88)i = key();		
}