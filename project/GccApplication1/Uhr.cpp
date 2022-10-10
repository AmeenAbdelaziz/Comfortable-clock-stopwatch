
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


 LCD disp(PC, LCD_Type_40x4, DISPLAY_ON|CURSOR_OFF|WRAPPING_OFF|BLINK_OFF);
static QuadEncoder  quenc(PJ);

 ////////////////////////// Menu 1 ///////////////////////
static const char cleartxt[] PROGMEM = "                   ";
static const char title1[] PROGMEM = "--CHOOSE FUNCTION--";
static const char entry11[] PROGMEM = " Set clock          ";
static const char entry12[] PROGMEM = " Set Stopwatch      ";
static const char entry13[] PROGMEM = "Change display type";
static const char exitmenu[] PROGMEM = " Exit menu          ";

static const char* const menu1[] PROGMEM ={cleartxt, title1, entry11, entry12, entry13, exitmenu};
ScrollMenu men1(menu1,(sizeof(menu1)/sizeof(char*)), &disp, &quenc,3, 20);

 ////////////////////////// Menu 2 and 3 ///////////////////////
static const char title2[] PROGMEM = "    --SET CLOCK--    ";
static const char title3[] PROGMEM = "--SET STOPWATCH--    ";
static const char entry21[] PROGMEM = " Set Hour           ";
static const char entry22[] PROGMEM = " Set Minute        ";
static const char entry23[] PROGMEM = " Set Second        ";


static const char* const menu2[] PROGMEM ={cleartxt, title2, entry21, entry22, entry23, exitmenu};
ScrollMenu men2(menu2,(sizeof(menu2)/sizeof(char*)), &disp, &quenc,3, 20);

static const char* const menu3[] PROGMEM ={cleartxt, title3, entry21, entry22, entry23, exitmenu};
ScrollMenu men3(menu3,(sizeof(menu3)/sizeof(char*)), &disp, &quenc,3, 20);


 ////////////////////////// Menu 4 ///////////////////////

static const char title4[] PROGMEM = "--SET TIME FORMAT--  ";
static const char entry41[] PROGMEM = " 24 hour           ";
static const char entry42[] PROGMEM = " am pm         ";

static const char* const menu4[] PROGMEM ={cleartxt, title4, entry41, entry42,exitmenu};
ScrollMenu men4(menu4,(sizeof(menu4)/sizeof(char*)), &disp, &quenc,3, 20);








// Callback Function For The Clock.
void Uhr::start_time()
{
	//my_clock.start_time();
	/*Watch*/
	if (!clockdisable)
	{
		if (second<59) second++;
		else
		{
			second=0;
			if (minute<59) minute++;
			else
			{
				minute=0;
				if (hours<23) hours++;
				else hours=0;
			}
			
		}
	}
	
	
}
// Callback Function For The Stopwatch.

void Uhr::start_time_stopwatch()
{
	/*Stopwatch*/
	if (!stopwdisable)
	{
		if (second>0) {second--;}
		else
		{
			if (minute>0)
			{
				minute--;
				second=59;
			}
			else
			{
				if (hours>0)
				{
					hours--;
					minute=59;
					second=59;
				}
				
			}
		}
	}
}

///////////////////////////////////  Set Time For Watch And Stopwatch //////////////////

void Uhr::setTime(uint8_t type,uint8_t timetype)
{
	disp.set_pos(3,0);
	switch(type)
		{
			case 0:disp.write_SRAM_text("clock   ");break;
			case 1:disp.write_SRAM_text("stopwatch");break;
		}
	disp.set_pos(2,2);
	disp.write_SRAM_text("00");
	switch(timetype)
		{
			case 0:  disp.write_char('H');break;//hour
			case 1:  disp.write_char('M');break;//minute
			case 2:  disp.write_char('S');break;//second
		}
	uint8_t val=0;
	
	switch(timetype)
		{
			case 0: quenc.start(0,23,0);break;//hour
			case 1: quenc.start(0,59,0);break;//minute
			case 2: quenc.start(0,59,0);break;//second
		}
	

	bool button=0;
	while(1)
	{
		disp.set_pos(0,20);
		disp.write_SRAM_text("Nach Eingabe Knopf");
		disp.set_pos(1,20);
		disp.write_SRAM_text("drueken und dann ");
		disp.set_pos(2,20);
		disp.write_SRAM_text("exit Menu        ");
		disp.set_pos(3,20);
		disp.write_SRAM_text("                 ");
		
		if (quenc.new_locked_value_available())
			 {
				val = (uint8_t)quenc.get_locked_value();
			 }
	    else if (quenc.new_value_available()) 
			{
				val = (uint8_t)quenc.get_unlocked_value();
				button=1;	
				disp.set_pos(2,2);
				disp.write_number(val, 2, '0');
				switch(timetype)
					{
					case 0:  disp.write_char('H');break;//hour
					case 1:  disp.write_char('M');break;//minute
					case 2:  disp.write_char('S');break;//second
					}			
			}
				
		if ((!(PINJ&(1<<3)))&&(button))
			{
				disp.set_pos(0,0);
				if (set_hour) hours = val; set_hour = false;
				if (set_minute) minute = val; set_minute = false;
				if (set_second) second = val; set_second = false;
				disp.cursor_off();
				disp.set_pos(2,2);
				break;
			}
	}
		quenc.stop();


}
// Set Time Format (12h Or 24h)
void Uhr::set_format(bool which_format)
{
	disp.set_pos(0,0);
	if (which_format)
		{
			if (hours>12)
				{
					disp.write_number(hours-12, 2, '0');
				}
			else
				{
					disp.write_number(hours, 2, '0');
				}
		
		
			disp.write_char(':');
			disp.write_number(minute, 2, '0');
			disp.write_char(':');
			disp.write_number(second, 2, '0');
			disp.set_pos(0,9);	
			if (hours>11)
				{
					disp.write_SRAM_text("pm");
				}
			else
				{
					disp.write_SRAM_text("am");
				}
		
		
		}
	else
		{		
			disp.write_number(hours, 2, '0');
			disp.write_char(':');
			disp.write_number(minute, 2, '0');
			disp.write_char(':');
			disp.write_number(second, 2, '0');
			disp.set_pos(0,9);
			
		}
	
}

// Display Time On The LCD (Watch And Stopwatch)
void Uhr::showTime(bool type)
{
	//Stopwatch
	if (type)
	{
		disp.set_pos(1,0);
		disp.write_number(hours, 2, '0');
		disp.write_char(':');
		disp.write_number(minute, 2, '0');
		disp.write_char(':');
		disp.write_number(second, 2, '0');
	} 
	else
	{
		disp.set_pos(0,0);
		disp.write_number(hours, 2, '0');
		disp.write_char(':');
		disp.write_number(minute, 2, '0');
		disp.write_char(':');
		disp.write_number(second, 2, '0');
				
	}

	
}


// default destructor
Uhr::~Uhr()
{
} //~Uhr
