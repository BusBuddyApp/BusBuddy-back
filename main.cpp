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
#include <stdio.h>
#include <string.h>
#include <unistd.h>

using namespace std;

void
print_version()
{
    fprintf(stdout, "Version %s on %s\n", mraa_get_version(), mraa_get_platform_name());
}

#define SHIELD_D2 2
#define SHIELD_D4 4
#define LED_ON 1
#define LED_OFF 0

static int ledstate;
int run_flag = 1;

void turnOnLED(void * args) {
	cout << "callback called" << endl;
	mraa_gpio_context led = (mraa_gpio_context) args;
	if (led != NULL) {
		mraa_gpio_write(led, LED_ON);
	}
}

int
main(int argc, char** argv)
{
	mraa_platform_t platform = mraa_get_platform_type();
    mraa_gpio_context led, button = NULL;
    const char* board_name = mraa_get_platform_name();
    ledstate = 0;

    print_version();
    fprintf(stdout, "Welcome to libmraa\n Version: %s\n Running on %s\n", mraa_get_version(), board_name);


	//set up LED for OUTPUT
    //MRAA_INTEL_EDISON_FAB_C
	led = mraa_gpio_init(SHIELD_D2);
    if (led == NULL) {
        fprintf(stdout, "Could not initilaize gpio\n");
        return 1;
    } else {
    	mraa_gpio_dir(led, MRAA_GPIO_OUT);
    }

    //set up button for INPUT
    button = mraa_gpio_init(SHIELD_D4);
    mraa_gpio_isr(button, MRAA_GPIO_EDGE_RISING, &turnOnLED, (void*)led);
    cout << "there" << endl;
//	if (gpio_in == NULL) {
//		fprintf(stdout, "Could not initilaize gpio\n");
//		return 1;
//	} else {
//		mraa_gpio_dir(gpio_in, MRAA_GPIO_IN);
//	}

	bool buttonDown = 0;
    while(run_flag == 1) {
//    	buttonDown = mraa_gpio_read(gpio_in);
//    	if (buttonDown == 1) {
//    		ledstate = !ledstate;
//    		mraa_gpio_write(gpio, ledstate);
//    	}
    	cout << "hello " << endl;
    	sleep(1);
    }
    mraa_gpio_close(led);
    mraa_gpio_close(button);

    return 0;
}
