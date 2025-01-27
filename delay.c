#include <intrins.h>

void delay(int ms)		
{
	unsigned char i, j;
	while(ms--){
	_nop_();
	_nop_();
	_nop_();
	i = 11;
	j = 190;
	do
	{
		while (--j);
	} while (--i);
	}
}
