#define F_CPU 16000000
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "LCD.h"
#include "Basics.h"
#include "DigiPort.h"
#include "QuadEncoder.h"
#include "Timer.h"
#include "ScrollMenu.h"
#include "Uhr.h"




Uhr my_clock; // Create An Object From The Class Uhr For Watch And Stopwatch
Uhr my_stopwatch;// Create An Object From The Class Uhr For Stopwatch
void timeUp();  // Callback Function For The Timer
void timeup_2();

Timer16 my_timer1(TC1, timeUp);  // Initialize the Timer Object For The Clock
Timer16 my_timer2(TC3, timeup_2);// Initialize the Timer Object For The Stopwatch
DigiPortRaw keys(PK, SET_IN_PORT);  // Set PortK As An Input
DigiPortRaw	 leds   (PA, SET_OUT_PORT); // Set PortA As An Output


int main(void)
{
	my_clock.clockdisable = false;
	sei ();  // Activate Interrupts	
	my_timer1.start_ms(1000); 
	my_timer2.start_ms(1000); 
	while (1)
	{	leds.off();
		if (!my_stopwatch.start_menu)
			{
				disp.set_pos(0,20);
				disp.write_SRAM_text("to Start Menu ");
				disp.set_pos(1,20);
				disp.write_SRAM_text("Press SW3");
			}
		if (keys.read_raw(0b10000000))
			{
				my_stopwatch.stopwdisable=false;
				my_stopwatch.start_menu=false;
				leds.on(0b10000000);
			    disp.clear();
			}
		my_clock.set_format(my_clock.distype);
		
		
		if (my_stopwatch.hours||my_stopwatch.minute||my_stopwatch.second) // Check, If Hours, Minutes Or Seconds were set.
			{															 // If True, Show Them On The Screen.
				my_stopwatch.showTime(1);
			
				if (my_stopwatch.stopwdisable)
					{
						my_stopwatch.start_menu=true; 	
						disp.set_pos(0,20);
						disp.write_SRAM_text("to start Stopwatch");
						disp.set_pos(1,20);
						disp.write_SRAM_text("press SW7");
					}			
			}	
			
		
		if (keys.read_raw(0b00001000))
			{
				leds.on(0b00001000);
				my_stopwatch.stopwdisable=false;
				uint8_t read=men1.select(); //read main inputs
				disp.set_pos(1,0);	
				switch (read) 
				{			
					case 0:
						my_clock.clockdisable=true;
						while(1)
							{
								read=men2.select();
								if (read!=3)
									{
										if (read==0) my_clock.set_hour=true;
										if (read==1) my_clock.set_minute=true;
										if (read==2) my_clock.set_second=true;
										 my_clock.setTime(0,read);
										 disp.clear();
										 my_clock.showTime(0);
									}

								else
									{
										disp.clear();
										break;
									}
					
							}
						my_clock.clockdisable=false;
						break;
							
					case 1:
						my_stopwatch.stopwdisable=true;
						while(1)
							{
								read=men3.select();
								if (read!=3)
									{
										if (read==0) my_stopwatch.set_hour=true;
										if (read==1) my_stopwatch.set_minute=true;
										if (read==2) my_stopwatch.set_second=true;
										my_stopwatch.setTime(1,read);
										disp.clear();
										my_stopwatch.showTime(1);
									}
								else
									{
										disp.clear();						
										break;
									}				
							}	
						break;
				
					case 2:				
						while(1)
							{			
								read=men4.select();
								if (read!=2)
									{
										switch(read)
											{
												case 0:my_clock.distype=0; my_clock.set_format(0);break;
												case 1:my_clock.distype=1; my_clock.set_format(1);break;																				
											}
										
									}
								else
									{
										disp.clear();	
																										
									}
								break;									
							}
								
				
					case 3:
						disp.clear();			
						break;
		
			}	

		}
												
	}
}


void timeUp()
{
	my_clock.start_time();
}

void timeup_2()
{
	my_stopwatch.start_time_stopwatch();

}



