/*
 * Copyright (c) 2016 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <iostream>
#include "gpio.hpp"
#include "buzzer.hpp"
#include "i2c.hpp"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "upm/jhd1313m1.hpp"

using namespace std;
using namespace upm;

void
print_version()
{
    fprintf(stdout, "Version %s on %s\n", mraa_get_version(), mraa_get_platform_name());
}

static const int SHIELD_D2 = 2;
static const int SHIELD_D3 = 3;
static const int SHIELD_D4 = 4;
static const int LED_ON = 1;
static const int LED_OFF = 0;
static const int SHIELD_I2C = 6;
static const int SOUND_SEC = 2;


static int ledstate;
int run_flag = 1;

string busNumbers[] = {"Bus #5 SW Main & 6th",
        "Bus #4 SE Hawthorne & 12th",
        "Bus #6 SE Hawthorne & Cesar Chavez Blvd",
        "Bus #9 SE Foster & Powell",
        "Bus #10 SE Foster & 82nd",
        "Bus #12 SE Foster & 94th (I-205 Overpass)"};
int busNum = 0;

struct state {
	string nextBus;
	mraa_gpio_context lcd;
	mraa_gpio_context button;
	mraa_i2c_context display;
	upm::Buzzer* buzzer;
};

//void playSound(state* s) {
//	s->buzzer->playSound(DO, 2000);
//}

void updateLCD(state* s) {
	upm::Jhd1313m1 *lcd = new upm::Jhd1313m1(0, 0x3E, 0x62);
	lcd->setCursor(0,0);
	busNum += 1;
	if (busNum > 5) {
		busNum = 0;
	}
	string str = busNumbers[busNum];
	lcd->write(str);
}

void buttonPressCallback(void * args) {
	state* state_ptr = (state*)args;
	cout << "CALLBACK CALLED" << endl;
	int sec = SOUND_SEC;
//	playSound(state_ptr);
	updateLCD(state_ptr);
	sleep(.5);
}

void turnOnLED(void * args) {
	cout << "callback called" << endl;
	mraa_gpio_context led = (mraa_gpio_context) args;
	if (led != NULL) {
		mraa_gpio_write(led, LED_ON);
	}
}


mraa_gpio_context init_button(int button_pin) {
	mraa_gpio_context button = mraa_gpio_init(button_pin);
	if (button == NULL) {
	        fprintf(stdout, "Could not initilaize button\n");
	    }
	mraa_gpio_isr(button, MRAA_GPIO_EDGE_RISING, &buttonPressCallback, NULL);

}

mraa_gpio_context init_lcd(int lcd_pin) {
    mraa_gpio_context lcd = mraa_gpio_init(lcd_pin);
    if (lcd == NULL) {
        fprintf(stdout, "Could not initilaize lcd\n");
    }
    mraa_gpio_dir(lcd, MRAA_GPIO_OUT); //change
    return lcd;
}

//void init_buzzer(state* s) {
//	upm::Buzzer* buzz = new Buzzer(6);
//
//	s->buzzer = buzz;
//}

int
main(int argc, char** argv)
{
	mraa_platform_t platform = mraa_get_platform_type();
    mraa_gpio_context button = NULL;
    const char* board_name = mraa_get_platform_name();
    ledstate = 0;

    print_version();
    fprintf(stdout, "Welcome to libmraa\n Version: %s\n Running on %s\n", mraa_get_version(), board_name);

	state* s = new state;

	//set up lcd for output and callback
    s->lcd = init_lcd(SHIELD_I2C);

    //set up button for INPUT
    s->button = init_button(SHIELD_D4);

//    init_buzzer(s);



    while(run_flag == 1) {
//    	buttonDown = mraa_gpio_read(gpio_in);
//    	if (buttonDown == 1) {
//    		ledstate = !ledstate;
//    		mraa_gpio_write(gpio, ledstate);
//    	}
    	sleep(1);
    }
    mraa_gpio_close(s->button);

    return 0;
}
