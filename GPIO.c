/////////////////////////////////////////////////////////////////////////////
// Course Number: CECS 347
// Assignment: Example project for Hardware PWM automated Car
// Description: 
/////////////////////////////////////////////////////////////////////////////

//////////////////////1. Pre-processor Directives Section////////////////////
#include "tm4c123gh6pm.h"
#include "GPIO.h"
#include "PLL.h"
/////////////////////////////////////////////////////////////////////////////

//////////////////////2. Declarations Section////////////////////////////////
////////// Constants //////////

////////// Local Global Variables //////////
/////////////////////////////////////////////////////////////////////////////

//////////////////////3. Subroutines Section/////////////////////////////////
// Dependency: None
// Inputs: None
// Outputs: None
// Description: Initializes PB5432 for use with L298N motor driver direction
// interrupt functions defined in startup.s
extern void DisableInterrupts(void); // Disable interrupts
extern void EnableInterrupts(void);  // Enable interrupts
extern void WaitForInterrupt(void);  // low power mode




void Car_Dir_Init(void){
	if ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)==0) {
		SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;	// Activate B clocks
		while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)==0){};
	}
		
  GPIO_PORTB_AMSEL_R &= ~0xCC;	// disable analog function
	GPIO_PORTB_AFSEL_R &= ~0xCC;	// no alternate function
  GPIO_PORTB_PCTL_R &= ~0xFF00FF00;	// GPIO clear bit PCTL 
	GPIO_PORTB_DIR_R |= 0xCC; // output on pin(s)
  GPIO_PORTB_DEN_R |= 0xCC;	// enable digital I/O on pin(s)
}


// Port D Initialization
void PortD_Init(void){ volatile unsigned long delay;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD;	// Activate D clocks
	while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOD)==0){};
		
  GPIO_PORTD_AMSEL_R &= ~0x0F;      // 3) disable analog function
  GPIO_PORTD_PCTL_R &= ~0x0000FFFF; // 4) GPIO clear bit PCTL  
  GPIO_PORTD_DIR_R |= 0x0F;         // 6) PD0-PAD output
  GPIO_PORTD_AFSEL_R &= ~0x0F;      // 7) no alternate function     
  GPIO_PORTD_DEN_R |= 0x0F;         // 8) enable digital pins PA3-PA0
  LED = Dark;                       // Turn off all LEDs.
}


void SW2_Init(void) {
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;	// Activate F clocks
	while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOF)==0){};
		
	GPIO_PORTF_LOCK_R = 0X4C4F434B;
	GPIO_PORTF_CR_R = 0x01; //Allow changes to PF0
	GPIO_PORTF_AMSEL_R &= ~0x01;     // 3) disable analog function
	GPIO_PORTF_PUR_R |= 0x01;     //     enable weak pull-up on PF4
  GPIO_PORTF_PCTL_R &= ~0x0000000F; //  configure PF4,0 as GPIO
  GPIO_PORTF_DIR_R &= ~0x01;         // 6) PF1-PF3 output
  GPIO_PORTF_AFSEL_R &= ~0x01;      // 7) no alternate function     
  GPIO_PORTF_DEN_R |= 0x01;         // 8) enable digital pins PF3-PF1
	GPIO_PORTF_IS_R &= ~0x01; // Edge Sensitive
	GPIO_PORTF_IBE_R &= ~0x01; //Not both edges
	GPIO_PORTF_IEV_R &= ~0x01; // Falling Edge event
	GPIO_PORTF_ICR_R |= 0x01; //clear flag
	GPIO_PORTF_IM_R |= 0x01;      // (f) arm interrupt on PF0
	NVIC_PRI7_R = (NVIC_PRI7_R&0xFF1FFFFF)|0x00400000; // priorty 4
	NVIC_EN0_R |= 0x08;
}

/*
unsigned long H,L;
void Motors_Init(void){
  SYSCTL_RCGC2_R |= 0x00000002; // activate clock for port 
  H = L = 40000;                // 50%
  GPIO_PORTA_AMSEL_R &= ~0xCC;      // disable analog functionality on PB2, PB3, PB6, PB7
  GPIO_PORTA_PCTL_R &= ~0x0FF000CC; // Configure PB2, PB3, PB6, PB7 as GPIO
  GPIO_PORTA_DIR_R |= 0xCC;     // Make PB2, PB3, PB6, PB7 output (direction pins)
  GPIO_PORTA_DR8R_R |= 0xCC;    // Enable 8 mA drive on PB2, PB3, PB6, PB7
  GPIO_PORTA_AFSEL_R &= ~0xCC;  // Disable alternate function on PB2, PB3, PB6, PB7
  GPIO_PORTA_DEN_R |= 0xCC;     // Enable digital I/O on PB2, PB3, PB6, PB7
  GPIO_PORTA_DATA_R &= ~0xCC;   // Set PB2, PB3, PB6, PB7 low (initially forward direction)
  NVIC_ST_CTRL_R = 0;           // disable SysTick during setup
  NVIC_ST_RELOAD_R = L-1;       // reload value for 500us
  NVIC_ST_CURRENT_R = 0;        // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000; // priority 2
  NVIC_ST_CTRL_R = 0x00000007;  // enable with core clock and interrupts
}
void SysTick_Handler(void){
  if(GPIO_PORTA_DATA_R&0xCC){   // Toggle PB2, PB3, PB6, PB7 (direction pins)
    GPIO_PORTA_DATA_R &= ~0xCC; // Make PB2, PB3, PB6, PB7 low
    NVIC_ST_RELOAD_R = L-1;     // reload value for low phase
  } else{
    GPIO_PORTA_DATA_R |= 0xCC;  // Make PB2, PB3, PB6, PB7 high
    NVIC_ST_RELOAD_R = H-1;     // reload value for high phase
  }
}


void Switch_Init(void){  unsigned long volatile delay;
  SYSCTL_RCGC2_R |= 0x00000020; // (a) activate clock for port F
  delay = SYSCTL_RCGC2_R;
  GPIO_PORTF_LOCK_R = 0x4C4F434B; // unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x11;         // allow changes to PF4,0
  GPIO_PORTF_DIR_R &= ~0x11;    // (c) make PF4,0 in (built-in button)
  GPIO_PORTF_AFSEL_R &= ~0x11;  //     disable alt funct on PF4,0
  GPIO_PORTF_DEN_R |= 0x11;     //     enable digital I/O on PF4,0
  GPIO_PORTF_PCTL_R &= ~0x000F000F; //  configure PF4,0 as GPIO
  GPIO_PORTF_AMSEL_R &= ~0x11;  //     disable analog functionality on PF4,0
  GPIO_PORTF_PUR_R |= 0x11;     //     enable weak pull-up on PF4,0
  GPIO_PORTF_IS_R &= ~0x11;     // (d) PF4,PF0 is edge-sensitive
  GPIO_PORTF_IBE_R &= ~0x11;    //     PF4,PF0 is not both edges
  GPIO_PORTF_IEV_R &= ~0x11;    //     PF4,PF0 falling edge event
  GPIO_PORTF_ICR_R = 0x11;      // (e) clear flags 4,0
  GPIO_PORTF_IM_R |= 0x11;      // (f) arm interrupt on PF4,PF0
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00400000; // (g) priority 2
  NVIC_EN0_R = 0x40000000;      // (h) enable interrupt 30 in NVIC
}
// L range: 8000,16000,24000,32000,40000,48000,56000,64000,72000
// power:   10%    20%   30%   40%   50%   60%   70%   80%   90%
void GPIOPortF_Handler(void){ // called on touch of either SW1 or SW2
  if(GPIO_PORTF_RIS_R&0x01){  // SW2 touch
    GPIO_PORTF_ICR_R = 0x01;  // acknowledge flag0
    if(L>8000) L = L-8000;    // slow down
  }
  if(GPIO_PORTF_RIS_R&0x10){  // SW1 touch
    GPIO_PORTF_ICR_R = 0x10;  // acknowledge flag4
    if(L<72000) L = L+8000;   // speed up
  }
  H = 80000-L; // constant period of 1ms, variable duty cycle
}
int main(void){
  DisableInterrupts();  // disable interrupts while initializing
  PLL_Init();           // bus clock at 80 MHz
  Motors_Init();         // output from PA5, SysTick interrupts
  Switch_Init();        // arm PF4, PF0 for falling edge interrupts
  EnableInterrupts();   // enable after all initialization are done
  while(1){
    // main program is free to perform other tasks
    WaitForInterrupt(); // low power mode
  }
}
*/
/////////////////////////////////////////////////////////////////////////////
