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


void clear_leds(void);
void spi_comm_start(void);
void set_led(led_name_t led_number, unsigned int value);
void motors_init(void);
void left_motor_set_speed(int motor_speed);
void right_motor_set_speed(int motor_speed);
void serial_start(void);
void e_send_uart1_char(const char * buff, int buff_len);



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
    serial_start();

    char str[100];
    int str_length;
    str_length = sprintf(str, "Hello World!\n");

    /* Infinite loop. */
    while (1) {
    	set_led(LED1, 1);
    	//waits 1 second
    	e_send_uart1_char(str, str_length);
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
