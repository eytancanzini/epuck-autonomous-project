#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ch.h"
#include "hal.h"
#include "memory_protection.h"
#include <main.h>
#include "leds.h"
#include "spi_comm.h"
#include "motors.h"
#include "epuck1x/uart/e_uart_char.h"
#include "stdio.h"
#include "serial_comm.h"
#include "chprintf.h"
#include "usbcfg.h"


int main(void)
{

    halInit();
    chSysInit();
    mpu_init();
    clear_leds();
    spi_comm_start();
    motors_init();

    left_motor_set_speed(0);
    right_motor_set_speed(0);
    usb_start();


    /* Infinite loop. */
    while (1) {
    	set_led(LED1, 1);
    	//waits 1 second
    	if (SDU1.config->usbp->state == USB_ACTIVE) {
    		chprintf((BaseSequentialStream*)&SDU1, "%4d,\n", 5);
    	}
        chThdSleepMilliseconds(1000);
        set_led(LED1, 0);
        chThdSleepMilliseconds(1000);
    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
