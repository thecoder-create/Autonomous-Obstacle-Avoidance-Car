/////////////////////////////////////////////////////////////////////////////
// Course Number: CECS 347
// Assignment: Example project for Hardware PWM automated Car
// Description: 
/////////////////////////////////////////////////////////////////////////////
#define LED (*((volatile unsigned long *)0x4000703C))  // use onboard three LEDs: PD3210

#define WHEEL_DIR (*((volatile unsigned long *)0x40005330)) // PB5432 are the four direction pins for L298
                                                            //PB 6732

////////// Constants //////////  
// Color    LED(s) PortA
// dark     ---    0
// red      R--    0x02
// blue     --B    0x04
// green    -G-    0x08
// yellow   RG-    0x0A
// white    RGB    0x0E
// pink     R-B    0x06
// Cran     -GB    0x0C

#define Dark    	0x00
#define Red     	0x04
#define Blue    	0x02
#define Green   	0x08
#define Yellow  	0x0A
#define Cran      0x0C
#define White   	0x0E
#define Purple  	0x06


// Constant definitions based on the following hardware interface:
// PB5432 are used for direction control on L298.
// Motor 1 is connected to the left wheel, Motor 2 is connected to the right wheel.

#define FORWARD 0x88
#define BACKWARD 0x44
#define LEFTPIVOT 0x48  
#define RIGHTPIVOT 0x84   


//////////////////////1. Declarations Section////////////////////////////////
////////// Function Prototypes //////////
// Dependency: None
// Inputs: None
// Outputs: None
// Description: Initializes PB5432 for use with L298N motor driver direction
void Car_Dir_Init(void);
void PortD_Init(void);
void SW2_Init(void) ;
/////////////////////////////////////////////////////////////////////////////
