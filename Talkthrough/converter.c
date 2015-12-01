#include <P18F252.h>

int SampleButton = 0;
int PR2value = 0x00;
int T2CONvalue = 0x00;
int LEDdisplay = 0xFF;		// By default will not light up any LEDs.
int LEDdisplayA = 0xFF;

void interrupt timer2_ISR ( void )
{
	T2CONbits.TMR2ON = 0;			//Stop the timer.
	ADCON0bits.GO_DONE = 1;			//start conversion
	while(ADCON0bits.GO_DONE);		//wait for conversion to end
	PORTB = ADRESH;					//send to DAC
	PIR1bits.TMR2IF = 0;         	//Check the interupt flag.
	T2CONbits.TMR2ON = 1;			//Starts the timer.
}

void DebounceDelay( void )			// Debounce delay is required to stop one button press from being seen are multiple pressses by the system.
{
	long DelayTime;
	for (DelayTime = 0; DelayTime < 60000 ; DelayTime++)
	{		
	}
}

void One_KHz( void )
{
	PR2value = 0xC8;		// 1KHz sampling frequency achieved via these values.
	T2CONvalue = 0x75;
	LEDdisplay = 0xD7;		// Will display the number 1 on the seven segment display.
	LEDdisplayA = 0xFF;
}

void Two_KHz( void )
{
	PR2value = 0xC8;		// 2KHz sampling frequency achieved via these values.
	T2CONvalue = 0x74;
	LEDdisplay = 0xC9; 		// Will display the number 2 on the seven segment display.
	LEDdisplayA = 0xDF;
}

void Four_KHz( void )
{
	PR2value = 0x64;		// 4KHz sampling frequency achieved via these values.
	T2CONvalue = 0x74;
	LEDdisplay = 0xC6;		// Will display the number 4 on the seven segment display.
	LEDdisplayA = 0xFF;
}

void Eight_KHz( void )
{
	PR2value = 0x32;		// 8KHz sampling frequency achieved via these values.
	T2CONvalue = 0x74;
	LEDdisplay = 0xC0;		// Will display the number 8 on the seven segment display.
	LEDdisplayA = 0xDF;
}

void main( void )
{

//ADC Setup
	ADCON0 = 0x81;				 
	ADCON1 = 0x0E;				// pcfg = 1110

	TRISA = 0xDF;				// Port A, 1101 1111 input/output configuration.
	TRISB = 0x00;				// Port B, all bits outputs.
    TRISC = 0x00;				// Port C, all bits outputs.
	PORTB = 0; 					// Initialise Port B.
	
	RCONbits,IPEN = 1;			// Enable interrupt priority scheme.
	IPR1bits.TMR2IP = 1;		// Gives timer2 high priority.
	INTCONbits.GIEH = 1;		// Enable high priority interupts.
	PIE1bits.TMR2IE = 1;		// Enable timer2 interupt.
	PIR1bits.TMR2IF = 0;		// Clear interupt flag for start.
	PR2 = 0x32;					// Configure timer2.
	T2CON = 0x74;				// Configure timer2.
	
	ADCON1bits.ADFM  = 0;		//left justification
	ADCON0bits.ADON = 1;

	T2CONbits.TMR2ON = 1;		//Starts the timer.
			
	while(1)
	{

		if (PORTAbits.AN1 == 0)		// Looking for the button that switches between sampling frequencies to be pressed.
		{
			SampleButton ++;
			DebounceDelay();
			if (SampleButton == 4)
			{
				SampleButton = 0;
			}
		}
		else if (PORTAbits.AN1 == 1)
		{
			DebounceDelay();
			if (SampleButton == 4)
			{
				SampleButton = 0;
			}
		}
		if (SampleButton == 0) // 1KHz Switch positions.
		{
			One_KHz();
		}
		else if (SampleButton == 1) // 2KHz Switch positions.
		{
			Two_KHz();
		}
		else if (SampleButton == 2) // 4KHz Switch positions.
		{
			Four_KHz();
		}
		else if (SampleButton == 3) // 8KHz Switch positions.
		{
			Eight_KHz();
		} 
		PR2 = PR2value;		// Sets the correct PR2 value
		T2CON = T2CONvalue;	// Sets the correct T2CON value
		PORTC = LEDdisplay;	// Sets the outputs from PORTC for the 7 segement display
		PORTA = LEDdisplayA;// Sets the outputs from PORTA for the 7 segement display
	}	
}