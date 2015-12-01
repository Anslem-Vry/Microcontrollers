#include <P18F252.h>

int i = 15;
unsigned char filterTotal = 0;				// The total of the filterBuffer array
unsigned char x[16]={0};

void interrupt timer2_ISR ( void )
{
	unsigned char average;				// Holds the value of the moving average
	unsigned char output;

	ADCON0bits.GO_DONE = 1;				//start conversion
	while(ADCON0bits.GO_DONE);			//wait for conversion to end

	x[i] = (ADRESH);

	i--;
	if (i == -1)
	{
		i = 15;							// If i is larger than the array, set i to be the lowest point in the array
	}
	filterTotal = (x[0]+x[1]+x[2]+x[3]+x[4]+x[5]+x[6]+x[7]+x[8]+x[9]+x[10]+x[11]+x[12]+x[13]+x[14]+x[15])/16; 
	average = filterTotal;
	PORTB = average;
	PIR1bits.TMR2IF = 0;         		//Check the interupt flag.
}

void main( void )
{
//ADC Setup
	ADCON0 = 0x81;				 
	ADCON1 = 0x0E;				 

	TRISA = 0xDF;					// Port A, all bits inputs.
	TRISB = 0x00;					// Port B, all bits, outputs.
    TRISC = 0x00;					// Port C, all bits inputs.
	PORTB = 0; 						// Initialise Port B.
	
	PORTAbits.RA5 = 0;

	RCONbits,IPEN = 1;				// Enable interrupt priority scheme.
	IPR1bits.TMR2IP = 1;			// Gives timer2 high priority.
	INTCONbits.GIEH = 1;			// Enable high priority interupts.
	PIE1bits.TMR2IE = 1;			// Enable timer2 interupt.
	PIR1bits.TMR2IF = 0;			// Clear interupt flag for start.
	PR2 = 0x31;						// Configure timer2.
	T2CON = 0x74;					// Configure timer2.
	
	ADCON1bits.ADFM  = 0;			//left justification
	ADCON0bits.ADON = 1;

	T2CONbits.TMR2ON = 1;			//Starts the timer.
			
	while(1)
	{

	}
}