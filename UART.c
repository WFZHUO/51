#include <REGX52.H>

void UART_Init(void){
	PCON &= 0x7F;		
	SCON = 0x50;		
	TMOD &= 0x0F;		
	TMOD |= 0x20;		
	TL1 = 0xFD;		
	TH1 = 0xFD;		
	ET1 = 0;		
	TR1 = 1;
	EA=1;//
	ES=1;//
}

void sendbyte(unsigned char byte){
	SBUF=byte;
	while(TI=0);
	TI=0;
}


//串口中断函数模板
//void UART_Routine(void) interrupt 4{
//	if(RI==1){
//
//		P2=SBUF;
//
//		RI=0;
//	}
//}
