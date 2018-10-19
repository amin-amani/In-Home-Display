#include <stm32f10x_lib.h>
#include <RTL.h>
#include "gpio.h"

 

void porta_set_direction(U8 direction,int bit){
	if(bit<8){
	GPIOA->CRL&=~(0x0f<<(4*bit));
	GPIOA->CRL|=direction<<(4*bit);
	return;
		}
		bit-=8;
	GPIOA->CRH&=~(0x0f<<(4*bit));
	GPIOA->CRH|=direction<<(4*bit);
		
	
	}
	void portb_set_direction(U8 direction,int bit){
	if(bit<8){
	GPIOB->CRL&=~(0x0f<<(4*bit));
	GPIOB->CRL|=direction<<(4*bit);
	return;
		}
		bit-=8;
	GPIOB->CRH&=~(0x0f<<(4*bit));
	GPIOB->CRH|=direction<<(4*bit);
	
	}
	void portc_set_direction(U8 direction,int bit){
	if(bit<8){
	GPIOC->CRL&=~(0x0f<<(4*bit));
	GPIOC->CRL|=direction<<(4*bit);
	return;
		}
		bit-=8;
	GPIOC->CRH&=~(0x0f<<(4*bit));
	GPIOC->CRH|=direction<<(4*bit);
	
	}
	void portd_set_direction(U8 direction,int bit){
	if(bit<8){
	GPIOD->CRL&=~(0x0f<<(4*bit));
	GPIOD->CRL|=direction<<(4*bit);
	return;
		}
		bit-=8;
	GPIOD->CRH&=~(0x0f<<(4*bit));
	GPIOD->CRH|=direction<<(4*bit);
	
	}
