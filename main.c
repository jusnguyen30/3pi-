

#include <pololu/3pi.h>
#include <avr/pgmspace.h>

const char welcome_line1[] PROGMEM = " Pololu";
const char welcome_line2[] PROGMEM = "3\xf7 Robot";
const char demo_name_line1[] PROGMEM = "Line";
const char demo_name_line2[] PROGMEM = "follower";

// A couple of simple tunes, stored in program space.
const char welcome[] PROGMEM = ">g32>>c32";
const char go[] PROGMEM = "L16 cdegreg4";

const char levels[] PROGMEM = {
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111
};


void load_custom_characters()
{
	lcd_load_custom_character(levels+0,0); // no offset, e.g. one bar
	lcd_load_custom_character(levels+1,1); // two bars
	lcd_load_custom_character(levels+2,2); // etc...
	lcd_load_custom_character(levels+3,3);
	lcd_load_custom_character(levels+4,4);
	lcd_load_custom_character(levels+5,5);
	lcd_load_custom_character(levels+6,6);
	clear(); // the LCD must be cleared for the characters to take effect
}

// This function displays the sensor readings using a bar graph.
void display_readings(const unsigned int *calibrated_values)
{
	unsigned char i;

	for(i=0;i<5;i++) {
		
		const char display_characters[10] = {' ',0,0,1,2,3,4,5,6,255};

		
		char c = display_characters[calibrated_values[i]/101];

		// Display the bar graph character.
		print_character(c);
	}
}

// Initializes the 3pi, displays a welcome message, calibrates, and
// plays the initial music.
void initialize()
{
	unsigned int counter; // used as a simple timer
	unsigned int sensors[5]; // an array to hold sensor values

	pololu_3pi_init(2000);
	load_custom_characters(); // load the custom characters
	
	// Play welcome music and display a message
	print_from_program_space(welcome_line1);
	lcd_goto_xy(0,1);
	print_from_program_space(welcome_line2);
	play_from_program_space(welcome);
	delay_ms(1000);

	clear();
	print_from_program_space(demo_name_line1);
	lcd_goto_xy(0,1);
	print_from_program_space(demo_name_line2);
	delay_ms(1000);

	// Display battery voltage and wait for button press
	while(!button_is_pressed(BUTTON_B))
	{
		int bat = read_battery_millivolts();

		clear();
		print_long(bat);
		print("mV");
		lcd_goto_xy(0,1);
		print("Press B");

		delay_ms(100);
	}

	// Always wait for the button to be released so that 3pi doesn't
	// start moving until your hand is away from it.
	wait_for_button_release(BUTTON_B);
	delay_ms(1000);

	// Auto-calibration: turn right and left while calibrating the
	// sensors.
	for(counter=0;counter<80;counter++)
	{
		if(counter < 20 || counter >= 60)
			set_motors(40,-40);
		else
			set_motors(-40,40);

		
		calibrate_line_sensors(IR_EMITTERS_ON);

		// Since our counter runs to 80, the total delay will be
		// 80*20 = 1600 ms.
		delay_ms(20);
	}
	set_motors(0,0);

	// Display calibrated values as a bar graph.
	while(!button_is_pressed(BUTTON_B))
	{
		// Read the sensor values and get the position measurement.
		unsigned int position = read_line(sensors,IR_EMITTERS_ON);

		clear();
		print_long(position);
		lcd_goto_xy(0,1);
		display_readings(sensors);

		delay_ms(100);
	}
	wait_for_button_release(BUTTON_B);

	clear();

	print("Go!");		

	// Play music and wait for it to finish before we start driving.
	play_from_program_space(go);
	while(is_playing());
}
	
// This is the main function, where the code starts.  All C programs
// must have a main() function defined somewhere.
int main()
{
	unsigned int sensors[5]; // an array to hold sensor values

	// set up the 3pi
	initialize();

	// This is the "main loop" - it will run forever.
	while(1)	{
		
		read_line(sensors,IR_EMITTERS_ON);
		//unsigned int postion = read_line(sensors,IR_EMITTERS_ON);
		unsigned int middleSens = sensors[1] + sensors[2] + sensors[3];
		//
		//if(postion < 1000){
			//set_motors(0,100);
		//}else if( postion > 3000){
			//set_motors(100,0);
		//}else if(middleSens < 1200){
			//while(middleSens < 1200){
				//set_motors(100,-100);
			//}
		//}
		//else{
			//set_motors(100,100);
		//}
		//
		
		
		
		
		//int leftSens = sensors[0]+ sensors[1];
		//int rightSen = sensors[3] + sensors[4];
		
		
		
		int leftSen = sensors[0] + sensors[1];
		int rightSen = sensors[3] + sensors[4];
		
		if(leftSen >=1100 && rightSen >=1100)
		{
			set_motors(100,0);
		}
		elif(sensors[2] == 900){
			
			set_motors(100,100);
		}
		else if(leftSen <= 600){
			
			set_motors(100,0);
			delay_ms(50);
		}
		else if(leftSen >= 1200){
			set_motors(0,100);
			delay_ms(50);
		}
		else if(rightSen <= 600){
			set_motors(0,100);
			delay_ms(50);
		}
		else if(rightSen >= 1200){
			set_motors(100,0);
			delay_ms(50);
			}else if(middleSens < 1200){
				while(middleSens < 1200){
					set_motors(100,-100);
			}
			
		}
		else{
			set_motors(100,100);
		}
		
		
    }
    
        /*
		if(position < 1000)
		{
			// We are far to the right of the line: turn left.

			// Set the right motor to 100 and the left motor to zero,
			// to do a sharp turn to the left.  Note that the maximum
			// value of either motor speed is 255, so we are driving
			// it at just about 40% of the max.
			set_motors(0,100);

			// Just for fun, indicate the direction we are turning on
			// the LEDs.
			left_led(1);
			right_led(0);
		}
		else if(position < 3000)
		{
			// We are somewhat close to being centered on the line:
			// drive straight.
			set_motors(100,100);
			left_led(1);
			right_led(1);
		}
		else
		{
			// We are far to the left of the line: turn right.
			set_motors(100,0);
			left_led(0);
			right_led(1);
		}
	}*/

	// This part of the code is never reached.  A robot should
	// never reach the end of its program, or unpredictable behavior
	// will result as random code starts getting executed.  If you
	// really want to stop all actions at some point, set your motors
	// to 0,0 and run the following command to loop forever:
	//
	// while(1);
}

// Local Variables: **
// mode: C **
// c-basic-offset: 4 **
// tab-width: 4 **
// indent-tabs-mode: t **
// end: **
