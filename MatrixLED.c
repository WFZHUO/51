#include <REGX52.H>
#include <intrins.h>

sbit RCK=P3^5;
sbit SCK=P3^6;
sbit SER=P3^4;

#define led_lattice_screen     P0

void _74HC595_wrintebyte(unsigned char byte);
void Delay1us(int us);

void led_lattice_screen2(int column,int date){ //choose column by column,less energy loss;( For date:the high information on the top,and 1 is on.)
	
		_74HC595_wrintebyte(date);
		led_lattice_screen=~(0x80>>(column-1));
		Delay1us(140);
		led_lattice_screen=0xff;
}

void _74HC595_wrintebyte(unsigned char byte){
	
	int i=0;
	for(i=0;i<8;i++){
		SER=byte&(0x80>>i);
		SCK=1;
		SCK=0;
	}
	RCK=1;
	RCK=0;
}

void Delay1us(int us)
{
	while(us--){
		_nop_();
		_nop_();
		_nop_();
	}
}

void MatrixLED_Init(void){
	SCK=0;
	RCK=0;
}

//int main(void){
//	int i;
//	int offset=0;
//	int cnt=0;
//	int a=11;//the value of a is equal to (animation-8).
//	int dtpf=4;//dwell time per frame.
//	int code animation[]={
//                        0xFF,0x7F,0x3F,0x0F,0x03,0x01,0x01,0x03,
//                        0x0F,0x3F,0x7F,0xFF,0xFF,0x7F,0x3F,0x0F,
//                        0x03,0x01,0x01
//                       };
//	
////if want the subtitle to be seamless each time ,add the first seven numbers to the end again.
//// like :                {
////		                    0xFF,0x00,0x9F,0x91,0x91,0x91,0xF1,0x00,
////		                    0x89,0x89,0x89,0x89,0xFF,0x00,0xF8,0x08,
////                        0x08,0xFF,0x08,0x08,0x00,0xF9,0x49,0x4f,
////                        0xFF,0x00,0x9F,0x91,0x91,0x91,0xF1,
////											 };										 
//	MatrixLED_Init();
//											 
//	while(1){

//  for(cnt=0;cnt<dtpf;cnt++){
//		for(i=1;i<9;i++){
//			led_lattice_screen2(i,animation[i-1+offset]);
//		}
//	}
//	
//	
////	offset+=8;//use this of animation
//	offset++;//use this when moving subtitles
//	if(offset>a)offset=0;

//	}

//	return 0;
//}


///////////////////////////////////////////////////////////////////////////////////////
//void led_lattice_screen1(int line,int column){ //in coordinate form,clear operation
//	
//		_74HC595_wrintebyte(0x80>>(line-1));
//		led_lattice_screen=~(0x80>>(column-1));
//		Delay1us(140);
//		led_lattice_screen=0xff;
//}

