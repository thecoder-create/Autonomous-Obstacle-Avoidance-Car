/////////////////////////////////////////////////////////////////////////////
// Course Number: CECS 347
// Assignment: Example project for Hardware PWM autmated Car
// Description: 
/////////////////////////////////////////////////////////////////////////////


// Constant definitions based on the following hardware interface:
// System clock is 16MHz.
#define TOTAL_PERIOD 16000 //  16MHz/1000=16000

// PB5432 are used for direction control on L298.
// Motor 1 is connected to the left wheel, Motor 2 is connected to the right wheel.

//////////////////////1. Declarations Section////////////////////////////////
////////// Function Prototypes //////////
// Dependency: None
// Inputs: None
// Outputs: None
// Description: 
//	Initializes the PWM module 1 signals tied to PF321 on the Tiva Launchpad 
//		to allow for changing brightness of LEDs based on vehicle speed.
//	Initializes the PWM module 0 signals tied to PB76 to be used with the 
//		L298N motor driver allowing for a variable speed of robot car.
void PWM_PB45_Init(void);

// Dependency: PWM_PB76_Init()
// Inputs: 
//	duty_L is the value corresponding to the duty cycle of the left wheel
//	duty_R is the value corresponding to the duty cycle of the right wheel
// Outputs: None 
// Description: Changes the duty cycles of PB76 by changing the CMP registers
void PWM_PB45_Duty(unsigned long duty_L, unsigned long duty_R);
/////////////////////////////////////////////////////////////////////////////
