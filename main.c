#include <REGX52.H>
#include <matrixled.h>
#include <delay.h>
#include <timer0.h>
#include <uart.h>

void key_detection(void);

struct snake{//定义蛇头
			unsigned char x;
			unsigned char column;
};
struct snake shead={5,0x02};

unsigned char block[8]={0};
unsigned int block_count[8]={0};
unsigned char ledcolumnshow[8]={0};

unsigned int who;
unsigned int random=1000;
unsigned int r=800;
unsigned int cnt=0;
unsigned int timegapmin=100;
unsigned int timegapmax=200;
unsigned int negv=200;
int onei,twoi,threei,fouri,fivei=0;
int timer0_flag=0;

typedef enum{
	KEY_RELEASED,
	KEY_PRESSED
} KeyState;

typedef struct{
	unsigned char pin;
	KeyState state;
}Key;

Key keys[4]={
	{1,	KEY_RELEASED},
	{1,	KEY_RELEASED},
	{1,	KEY_RELEASED},
	{1,	KEY_RELEASED},
};

int main(void){
	
	MatrixLED_Init();//led矩阵屏初始化
	Timer0_Init();//定时器初始化
//	UART_Init();//串口通信初始化,使用时应格外注意可能会与t0发生冲突，目前发现开启后会使独立按键出现问题。在t0的中断里好点
//	while(1){//串口调试
//		sendbyte(0x01);
//	}

	
	while(1){	//循环检测按键
		key_detection();
		r++;
		if(r>timegapmax || r<timegapmin)r=((timegapmin+timegapmax)/2);//r的范围被限制来产生一个合适的random。		
		
		if(timer0_flag){//update screen
			timer0_flag=0;
			
			for(fouri=0;fouri<8;fouri++){
				ledcolumnshow[fouri]=block[fouri];
			}

			ledcolumnshow[shead.x-1]|=shead.column;
			
			for(fivei=0;fivei<8;fivei++){
				 if(ledcolumnshow[fivei]!=0x00){
						led_lattice_screen2(fivei+1,ledcolumnshow[fivei]);
				 }
			}
		}

	}

	return 0;
}

void Timer0_Routine(void) interrupt 1{ //定时器实现每33ms更新一次屏幕
	static unsigned int i=0;
	cnt++;
	for(onei=0;onei<8;onei++){
		block_count[onei]++;
	}
	
	i++;
	TH0=64614/256;
	TL0=64614%256;
	if(i==random){//利用主函数里的while来生成随机数。
		who=random%9;
		random=r;
		i=0;
	}
	
	if(who>0 && who<9){
		block[who-1]|=0x80;
		block_count[who-1]=0;
		who=0;
	}	

		for(twoi=0;twoi<8;twoi++){
			if(block_count[twoi]==negv && block[twoi]!=0x00){
				block[twoi]>>=1;
				block_count[twoi]=0;
			}
		}

	for(threei=0;threei<8;threei++){
		if(shead.x==(threei+1) && (block[threei]&shead.column)!=0x00){
			EA=0;
			break;
		}
	}
	
	if(cnt==33){//led矩阵屏显示，加if判断的目的是尽量减少中断中led屏幕的使用，不然中断里的时间太长了。
		cnt=0;
		timer0_flag=1;

//		for(fouri=0;fouri<8;fouri++){
//			ledcolumnshow[fouri]=block[fouri];
//		}
	
//		ledcolumnshow[shead.x-1]|=shead.column;

//		for(fivei=0;fivei<8;fivei++){
//			 if(ledcolumnshow[fivei]!=0x00){
//					led_lattice_screen2(fivei+1,ledcolumnshow[fivei]);
//     	 }
//		}
	}
}


void key_detection(void){
	int i;
	for(i=0;i<4;i++){
		switch (i){
			case 0:
				keys[i].pin=P3_1;
			  break;
			case 1:
				keys[i].pin=P3_0;
				break;
			case 2:
				keys[i].pin=P3_2;
			  break;
			case 3:
				keys[i].pin=P3_3;
			  break;
		}
		if(keys[i].pin==0){
			if(keys[i].state==KEY_RELEASED){
				keys[i].state=KEY_PRESSED;
				delay(10);
				switch (i){
					case 0:
						if(shead.x!=1)shead.x--;
						break;
					case 1:
						if(shead.x!=8)shead.x++;
						break;
					case 2:
						if(shead.column!=0x01)shead.column>>=1;
						break;
					case 3:
						if(shead.column!=0x80)shead.column<<=1;
						break;
				}
			}
		}else{
			if(keys[i].state==KEY_PRESSED){
				keys[i].state=KEY_RELEASED;
				delay(10);
			}	
		}
	}
}

//如果不设随机数,第六列每隔80s就会自动掉落一个障碍物