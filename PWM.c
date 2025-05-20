/////////////////////////////////////////////////////////////////////////////
// Course Number: CECS 347
// Assignment: Example project for Hardware PWM automated Car
// Description: 
/////////////////////////////////////////////////////////////////////////////

//////////////////////1. Pre-processor Directives Section////////////////////
#include "tm4c123gh6pm.h"
#include "PWM.h"

////////// Local Global Variables //////////
/////////////////////////////////////////////////////////////////////////////

//////////////////////3. Subroutines Section/////////////////////////////////
// Dependency: None
// Inputs: None
// Outputs: None
// Description: 
// Initializes the PWM module 0 generator 0 outputs A&B tied to PB45 to be used with the 
//		L298N motor driver allowing for a variable speed of robot car.
void PWM_PB45_Init(void){
	if ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)==0) {
		SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;	// Activate B clocks
		while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)==0){};
	}
	
	//
	GPIO_PORTB_AFSEL_R |= 0x30;	// enable alt funct: PB45 for PWM
	//SET
  GPIO_PORTB_PCTL_R &= ~0x00FF0000; // PWM to be used  
  GPIO_PORTB_PCTL_R |= 0x00440000; //        PWM to be used WHY 84 ?? DATASHEET REGISTER 17
  GPIO_PORTB_DEN_R |= 0x30;	// enable digital I/O 
//	GPIO_PORTB_PCTL_R |= 0x00840000;
	
	// Initializes PWM settings
	SYSCTL_RCGCPWM_R |= 0x01;	// activate PWM0. enable the clock source for PWM0
	//SYSCTL_RCGCPWM_R |= 0x02; //enable the clock source for PWM1
	SYSCTL_RCC_R &= ~0x001E0000; // Clear any previous PWM divider values
	
	// PWM0_0 output A&B Initialization for PB45
	//enable PWM module 0 generator 1
	
	//PWM0_0_CTL_R &= ~0x00000002; //COUNT DOWN MODE SETTING COUNT MODE TO BE DOWN
	PWM0_1_CTL_R = 0;	// re-loading down-counting mode
//PWM signal generator
	
	PWM0_1_GENA_R |= 0xC8;	// low on LOAD, high on CMPA down  OUTPUT A
	PWM0_1_GENB_R |= 0xC08;// low on LOAD, high on CMPB down   OUTPUT B
	
	
	PWM0_1_LOAD_R = TOTAL_PERIOD - 1;	// cycles needed to count down to 0
	
	
	//setting duty cycle CMPA/CMPB= require duty cycle - 1
	//OUTPUT A
  PWM0_1_CMPA_R = 0;	// count value when output rises
	//OUTPUT B
	PWM0_1_CMPB_R = 0;	// count value when output rises
	
	PWM0_1_CTL_R |= 0x00000001;	// Enable PWM0 Generator 1 in Countdown mode
	
	PWM0_ENABLE_R &= ~0x0000000C;	// Disable PB54:PWM0 output 2&3 on initialization
}
// Dependency: PWM_Init()
// Inputs: 
//	duty_L is the value corresponding to the duty cycle of the left wheel
//	duty_R is the value corresponding to the duty cycle of the right wheel
// Outputs: None 
// Description: Changes the duty cycles of PB45 by changing the CMP registers
void PWM_PB45_Duty(unsigned long duty_L, unsigned long duty_R){
	PWM0_1_CMPA_R = duty_L - 1;	// PB4 count value when output rises
  PWM0_1_CMPB_R = duty_R - 1;	// PB5 count value when output rises
}
/////////////////////////////////////////////////////////////////////////////
