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
#include "sensors/VL53L0X/VL53L0X.h"
#include "sensors/proximity.h"

messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);


int main(void)
{
	messagebus_init(&bus, &bus_lock, &bus_condvar);

    halInit();
    chSysInit();
    mpu_init();
    clear_leds();
    spi_comm_start();
    motors_init();

    left_motor_set_speed(0);
    right_motor_set_speed(0);
    usb_start();
    VL53L0X_start();
    proximity_start();
    calibrate_ir();


    uint16_t val;

    /* Infinite loop. */
    while (1) {
    	set_led(LED1, 1);
    	//waits 1 second
    	val = VL53L0X_get_dist_mm();
    	if (SDU1.config->usbp->state == USB_ACTIVE) {
    		chprintf((BaseSequentialStream*)&SDU1, "%4d,\n", val);
    	}
        chThdSleepMilliseconds(1000);
    	if (SDU1.config->usbp->state == USB_ACTIVE) {
    		chprintf((BaseSequentialStream*)&SDU1, "-----------\n");
    		chprintf((BaseSequentialStream*)&SDU1, "%4d\n", get_prox(0));
    		chprintf((BaseSequentialStream*)&SDU1, "%4d\n", get_prox(1));
    		chprintf((BaseSequentialStream*)&SDU1, "%4d\n", get_prox(2));
    		chprintf((BaseSequentialStream*)&SDU1, "%4d\n", get_prox(3));
    		chprintf((BaseSequentialStream*)&SDU1, "-----------\n");
    	}
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
