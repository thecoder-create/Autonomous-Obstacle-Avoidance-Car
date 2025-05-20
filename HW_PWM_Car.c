/////////////////////////////////////////////////////////////////////////////
// Course Number: CECS 347
// Assignment: Example project for Hardware PWM autmated Car
// Description: 
/////////////////////////////////////////////////////////////////////////////

//////////////////////1. Pre-processor Directives Section////////////////////
#include "tm4c123gh6pm.h"
#include "PWM.h"
#include "GPIO.h"
#include "Sensors.h"
#include "LEDSW.h"
#include "PLL.h"
#include "stdint.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/////////////////////////////////////////////////////////////////////////////


////////// Local Global Variables //////////
int button_press;
double percentage;
double left;
double right;
int count;
bool R;
uint8_t sample=0;

volatile uint16_t diff;

// The digital number ADCvalue is a representation of the voltage on PE2 
// voltage  ADCvalue
// 0.00V     0
// 0.75V    1024
// 1.50V    2048
// 2.25V    3072
// 3.00V    4095

////////////////////// Declarations Section////////////////////////////////
#define START_SPEED TOTAL_PERIOD*0.7

// You use datasheet to calculate the following ADC values
// then test your sensors to adjust the values 
#define CRASH             IR15CM// if there is less than this distance ahead of the robot, it will immediately stop
#define IR15CM            2233  // ADC output for 15cm:1.8v -> (1.8/3.3)*4095=2233 
#define IR20CM            1724  // ADC output for 20cm:1.39v -> (1.39/3.3)*4095=1724
#define IR30CM            1116  // ADC output for 30cm:0.9v -> (0.9/3.3)*4095=1116
#define IR40CM            918   // ADC output for 40cm:0.74v -> (0.74/3.3)*4095=918
#define IR80CM            496   // ADC output for 80cm:0.4v -> (0.4/3.3)*4095=496
                                // with equal power to both motors (LeftH == RightH), the robot still may not drive straight
                                // due to mechanical differences in the motors, so bias the left wheel faster or slower than
                                // the constant right wheel
#define IR100CM           100   // with equal power to both motors (LeftH == RightH), the robot still may not drive straight
                                // due to mechanical differences in the motors, so bias the left wheel faster or slower than
                                // the constant right wheel
#define LEFTMINPCT        30    // minimum percent duty cycle of left wheel (10 to 90)
#define LEFTMAXPCT        50    // maximum percent duty cycle of left wheel (10 to 90)
#define RIGHTCONSTPCT     40    // constant percent duty cycle of right wheel (10 to 90)
//#define LEFTPOWER        	0.5*PERIOD   // duty cycle of left wheel 
//#define RIGHTPOWER        0.5*PERIOD    // duty cycle of left wheel 

////////// Function Prototypes //////////
void System_Init(void);
void steering(uint16_t ahead_dist,uint16_t right_dist, uint16_t left_dist);
void Delay(void);
void SysTick_Init(void);
extern void DisableInterrupts(void); // Disable interrupts
extern void EnableInterrupts(void);  // Enable interrupts
extern void WaitForInterrupt(void);  // low power mode


void GPIOPortF_Handler(void){
	if(GPIO_PORTF_RIS_R&0x01){
		//Software acknowledges the flag and clears the read-only RIS bit by setting ICR
		GPIO_PORTF_ICR_R = 0x01;
		button_press = 1;
	}
}

	
void SysTick_Handler(void){
  sample = 1;
}
//////////////////////3. Subroutines Section/////////////////////////////////
// MAIN: This main is meant for the command configuration of the hc05.


int main(void){
  uint16_t left, right, ahead;
  
  DisableInterrupts();  // disable interrupts while initializing
  System_Init();
  EnableInterrupts();   // enable after all initialization are done

	 

	PWM_PB45_Duty(START_SPEED, START_SPEED);
    
	// moving forward
	LED = Green; // yellow led for 2 seconds before start
	WHEEL_DIR = FORWARD;
	PWM0_ENABLE_R |= 0x0000000C; // enable both wheels
	Delay();
	
  while(1){
    if(sample) {
      sample = 0;
      ReadSensorsMedianFilter(&ahead, &right, &left);
      steering(ahead,right,left);
    }
  }
}

void System_Init(void) {
  PLL_Init();           // bus clock at 80 MHz
  Sensors_Init();        // initialize ADC to sample AIN2 (PE1), AIN9 (PE4), AIN8 (PE5)
	SysTick_Init();
  LEDSW_Init();         // configure onboard LEDs and push buttons	
  PortD_Init();
  Car_Dir_Init();
  PWM_PB45_Init();
}

void steering(uint16_t ahead_dist,uint16_t right_dist, uint16_t left_dist){
  // Suggest the following simple control as starting point:
  // 1. Take care of off center positions:
	//    If left off center track, turn right; 
	//    If right off center track, turn left
  // 2. Take care of turning track: 
	//    make a right turn if left opening detected;
	//    make a left turn if right opening detected;
  // 3. Optional: Take care of crach ahead: stop if crash ahead
	// 4.	go straight if moving in the center area
  // Feel free to add more controlls to fine tune your robot car.
  // Make sure to take care of both wheel movements and LED display.  
if (ahead_dist > IR20CM || left_dist > IR20CM || right_dist > IR20CM) // If any sensor detects an object within 20 cm
{
    // Stop both wheels
    PWM0_ENABLE_R &= ~0x0000000C;

    if (ahead_dist > IR20CM && left_dist > IR20CM && right_dist > IR20CM) // All sensors detect close object   reverse
    {
        LED = Red;
        PWM_PB45_Duty(START_SPEED, START_SPEED);
        WHEEL_DIR = BACKWARD;
        PWM0_ENABLE_R |= 0x0000000C; // Enable both wheels
    }
    else if (left_dist > IR20CM) // Obstacle on left side   pivot right
    {
        LED = Green;
        PWM_PB45_Duty(START_SPEED, START_SPEED);
        WHEEL_DIR = RIGHTPIVOT;
        PWM0_ENABLE_R &= ~0x00000008; // Disable right wheel
        PWM0_ENABLE_R |= 0x00000004;  // Enable left wheel
    }
    else if (right_dist > IR20CM) // Obstacle on right side   pivot left
    {
        LED = Blue;
        PWM_PB45_Duty(START_SPEED, START_SPEED);
        WHEEL_DIR = LEFTPIVOT;
        PWM0_ENABLE_R |= 0x00000008;  // Enable right wheel
        PWM0_ENABLE_R &= ~0x00000004; // Disable left wheel
    }
}
else if (ahead_dist < IR80CM && left_dist < IR80CM && right_dist < IR80CM) // If all sensors are clear beyond 80 cm   stop
{
    LED = Blue;
    PWM0_ENABLE_R &= ~0x0000000C; // Stop both wheels
}
else // Otherwise, path is clear   move forward
{
    LED = Dark;
    PWM_PB45_Duty(START_SPEED, START_SPEED);
    WHEEL_DIR = FORWARD;
    PWM0_ENABLE_R |= 0x0000000C; // Enable both wheels
}

}


		
// Subroutine to wait 0.25 sec
// Inputs: None
// Outputs: None
// Notes: ...
void Delay(void){
	unsigned long volatile time;
  time = 727240*500/91;  // 0.25sec
  while(time){
		time--;
  }
}

void SysTick_Init(void){
	NVIC_ST_CTRL_R = 0;                   // disable SysTick during setup
  NVIC_ST_RELOAD_R = NVIC_ST_RELOAD_M;  // maximum reload value 
  NVIC_ST_CURRENT_R = 0;                // any write to current clears it
                                        // enable SysTick with core clock
  
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF) | 0x20000000;
  NVIC_ST_CTRL_R = NVIC_ST_CTRL_ENABLE+NVIC_ST_CTRL_CLK_SRC+NVIC_ST_CTRL_INTEN;
}