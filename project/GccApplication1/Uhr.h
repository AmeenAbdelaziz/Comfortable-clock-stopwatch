

#ifndef __UHR_H__
#define __UHR_H__
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdbool.h>



// Defining Objects Here As Extern And Initialize Them later
extern ScrollMenu men1,men2,men3,men4;
extern LCD disp;
extern DigiPortRaw leds;
//extern uint8_t nowtime[2][3];  // 2-Dimensional Array. First Row For Watch And The Second For StopWtch
class Uhr
{
	
//variables

public:
bool distype = false;
bool clockdisable = false; 
bool stopwdisable = false;
bool start_menu = false;

bool set_hour = false;
bool set_minute = false;
bool set_second = false;

uint8_t hours;
uint8_t minute;
uint8_t second;



// Declaration For The Methods 
void start_time();
void start_time_stopwatch();
void setTime(uint8_t type,uint8_t timetype);
void set_format(bool which_format);
void showTime(bool type);

// Default Constructor
Uhr() : hours(0) , minute(0) , second(0) {}
~Uhr();

	Uhr( const Uhr &c );
	Uhr& operator=( const Uhr &c );

}; //Uhr

#endif //__UHR_H__
