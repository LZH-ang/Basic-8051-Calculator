/*********************************������ѵ��***************************/
//2021��3��27�գ���Ե��ճ����޸ģ�ʵ�ֽ��������ʮ���ƽ��Ϊ65535������
//��ʾ�Ͷఴ�������������Ŀ�������õı���ַ���С���Ա�������ٱ�д���
#include<reg51.h>
#include<key.c>
code unsigned char bit_scan[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,};
code unsigned char seven_seg[] = 
{
	0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
	0xff,
};
sbit LE = P1^4;
/**********************************************************************/
void timer0_isr(void) interrupt 1
{
	TH0 = (65536 - 5000) / 256;
	TL0 = (65536 - 5000) % 256;
	P0 = 0xff;
	P0 = bit_scan[cp];LE = 1;LE = 0;P0 = seven_seg[num[cp]];
	cp++;
	if(cp >= 8)cp = 0;
}
void timer0_init(void)
{
	TMOD = TMOD | 0x01;
	TH0 = (65536 - 5000) / 256;
	TL0 = (65536 - 5000) % 256;
	EA = 1;
	ET0 = 1;
	TR0 = 1;
}
void main(void)
{
	timer0_init();
	while(1)
	{
		key_do();
		zero_ct();
	}
}