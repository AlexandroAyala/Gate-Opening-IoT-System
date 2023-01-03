#include <stdint.h>
#include <stdbool.h>

#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "SysTick.h"
#include "driverlib/pwm.h"
//#include "main.h" 
#include "driverlib/pin_map.h"

#define GPIO_PORTF_LOCK_R       (*((volatile uint32_t *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile uint32_t *)0x40025524))
#define GPIO_PORTF_PUR_R        (*((volatile uint32_t *)0x40025510))


static int32_t value12;
static int32_t value2;
static uint32_t i;


int main(void){
	SysCtlClockSet(SYSCTL_SYSDIV_12_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
	SysTick_Init(0xffffff, 0x05);	//enable systick with no interrupts
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); //our PIR sensors
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);	//our LEDs and motor
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);	//our LEDs 
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);	//our servo
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	value12=0;
	
	
	//only reading 2 input
	GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_5);//ir sensor read
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_3); // output to Rasberry PI
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5); 
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7); 
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_1 | GPIO_PIN_0);
	GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);	//read from Rasberry PI
	
	
	SysTick_Wait(16777215); //add delay about 1 second for each delay
	SysTick_Wait(16777215);
	SysTick_Wait(16777215); //add delay
	SysTick_Wait(16777215); //add delay

	
	while(1){
		value12=GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_6);
		i = 0;
		value2 = GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
		if (value12 == 0x20){
			GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0x08);
			
		}
		if (value2 == 0x40 ){
				while(i < 50){	//servo gate unlock
					GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0x02);
					SysTick_Wait(32000);
					GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0x00);
					SysTick_Wait(320000);
					i ++;
			}
				i = 0;
				GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0x10);//open door
				GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0x00);	

				SysTick_Wait(16777215); //add delay
				SysTick_Wait(16777215); //add delay
				GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0x00);
				GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0x00);
			
				SysTick_Wait(16777215);
				SysTick_Wait(16777215);
				SysTick_Wait(16777215); //add delay to stop reading PIR
				SysTick_Wait(16777215);

			
		}else if(value12 == 0x40){
				GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0x01);
		}
				if(value2 == 0x80){
					GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0x00);
					GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0x20);//close door
			
					SysTick_Wait(16777215); //add delay
					SysTick_Wait(16777215); //add delay
			
					GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0x00);
					GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0x00);
			
					SysTick_Wait(16777215); //add delay

				i = 0;
				while(i < 50){//servo lock 
					GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0x02);
					SysTick_Wait(16000);
					GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0x00);
					SysTick_Wait(320000);
					i ++;
				}
			
				SysTick_Wait(16777215); //add delay to stop reading PIR
				SysTick_Wait(16777215);
				SysTick_Wait(16777215); //add delay to stop reading PIR
				SysTick_Wait(16777215);

		}else{//this is my led matrix animation
			//will play when idle
			GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0x08);
			SysTick_Wait(2000000);
			GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0x00);
			SysTick_Wait(2000000);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0x10);
			SysTick_Wait(2000000);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0x00);
			SysTick_Wait(2000000);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0x20);
			SysTick_Wait(2000000);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0x00);
			SysTick_Wait(2000000);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0x40);
			SysTick_Wait(2000000);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0x00);
			SysTick_Wait(2000000);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0x80);
			SysTick_Wait(2000000);
			GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0x00);
			SysTick_Wait(2000000);
		}
	}
}

