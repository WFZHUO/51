#include <REGX52.H>

void Timer0_Init(){
	TMOD&=0xF0;
	TMOD|=0x01;
	TF0=0;
	TR0=1;
	TH0=64614/256;
	TL0=64614%256;
	ET0=1;
	EA=1;
	PT0=0;
}

//timer0 1 second templates
//void Timer0_Routine(void) interrupt 1{ 
//	static unsigned int cnt;
//	cnt++;
//	TH0=64614/256;
//	TL0=64614%256;
//	if(cnt==1000){
//		cnt=0;
//		//P2_0=~P2_0;//
//	}
//}