#include <REGX52.H>
#include <matrixled.h>
#include <delay.h>
#include <timer0.h>
#include <uart.h>

void key_detection(void);

struct snake{//������ͷ
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
	
	MatrixLED_Init();//led��������ʼ��
	Timer0_Init();//��ʱ����ʼ��
//	UART_Init();//����ͨ�ų�ʼ��,ʹ��ʱӦ����ע����ܻ���t0������ͻ��Ŀǰ���ֿ������ʹ���������������⡣��t0���ж���õ�
//	while(1){//���ڵ���
//		sendbyte(0x01);
//	}

	
	while(1){	//ѭ����ⰴ��
		key_detection();
		r++;
		if(r>timegapmax || r<timegapmin)r=((timegapmin+timegapmax)/2);//r�ķ�Χ������������һ�����ʵ�random��		
		
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

void Timer0_Routine(void) interrupt 1{ //��ʱ��ʵ��ÿ33ms����һ����Ļ
	static unsigned int i=0;
	cnt++;
	for(onei=0;onei<8;onei++){
		block_count[onei]++;
	}
	
	i++;
	TH0=64614/256;
	TL0=64614%256;
	if(i==random){//�������������while�������������
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
	
	if(cnt==33){//led��������ʾ����if�жϵ�Ŀ���Ǿ��������ж���led��Ļ��ʹ�ã���Ȼ�ж����ʱ��̫���ˡ�
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

//������������,������ÿ��80s�ͻ��Զ�����һ���ϰ���