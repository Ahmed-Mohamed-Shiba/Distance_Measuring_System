/*
 * ultrasonic sensor.c
 *
 *  Created on: Jul 6, 2022
 *      Author: Ahmed Shiba
 */

#include <avr/io.h>
#include "LCD.h"
#define  F_CPU 1000000UL
#include <util/delay.h>
int main(void)
{
	LCD_vInit();
	unsigned short a,b,high,distance;
	DIO_vsetPINDir('D',7,1);
    while(1)
    {	
		TCCR1A = 0;
		TIFR = (1<<ICF1);  	/* Clear ICF (Input Capture flag)  */
		DIO_write('D',7,1);
		_delay_us(50);
		DIO_write('D',7,0);
	
	    TCCR1B = 0xc1;  	/* Rising edge, no prescaler , noise canceler*/
	    while ((TIFR&(1<<ICF1)) == 0);
	    a = ICR1;  		/* Take value of capture register */
	    TIFR = (1<<ICF1);  	/* Clear ICF flag */
	    TCCR1B = 0x81;  	/* Falling edge, no prescaler ,noise canceler*/
	    while ((TIFR&(1<<ICF1)) == 0);
	    b = ICR1;  		/* Take value of capture register */
	    TIFR = (1<<ICF1);  	/* Clear ICF flag */
		TCNT1=0;
	    TCCR1B = 0;  		/* Stop the timer */
		high=b-a;
		distance=((high*34600)/(F_CPU*2)) ;
		if (distance>=80)
		{
			LCD_clearscreen();
			LCD_vSend_string("no object");
			_delay_ms(500);
		}
		else
		{
		LCD_clearscreen();
		LCD_vSend_string("distance=");
		LCD_vSend_char((distance/10)+48);
		LCD_vSend_char((distance%10)+48);
	    LCD_vSend_string("cm");
		_delay_ms(500);
		}		
	}
}

