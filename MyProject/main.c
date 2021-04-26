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


	uint16_t TOF;
	uint16_t val;
	uint16_t LHS;
	uint16_t RHS;
	uint16_t L_side;
	uint16_t r_side;

	set_led(LED1, 1);
	chThdSleepMilliseconds(750);
	set_led(LED2, 1);
	chThdSleepMilliseconds(750);
	set_led(LED3, 1);
	chThdSleepMilliseconds(750);
	set_led(LED4, 1);
	chThdSleepMilliseconds(750);
	set_led(LED5, 1);
	chThdSleepMilliseconds(750);
	set_led(LED6, 1);
	chThdSleepMilliseconds(750);
	set_led(LED7, 1);
	chThdSleepMilliseconds(750);
	set_led(LED8, 1);
	chThdSleepMilliseconds(1250);
	clear_leds();


	/* Infinite loop. */
	while (1) {
		chThdSleepMilliseconds(100);
		// Obstacle Avoidance Implementation
		TOF = VL53L0X_get_dist_mm();
		LHS = get_prox(6);
		RHS = get_prox(1);
		L_side = get_prox(2);
		r_side = get_prox(5);
		if (SDU1.config->usbp->state == USB_ACTIVE) {
			set_led(LED2, 1);
			chprintf((BaseSequentialStream*)&SDU1, "%4d,\n", TOF);
			chprintf((BaseSequentialStream*)&SDU1, "-----------\n");
			chprintf((BaseSequentialStream*)&SDU1, "%4d\n", LHS);
			chprintf((BaseSequentialStream*)&SDU1, "%4d\n", RHS);
			chprintf((BaseSequentialStream*)&SDU1, "-----------\n");
		}
		if (TOF < 45) {
			LHS = get_prox(6) + get_prox(7);
			RHS = get_prox(0) + get_prox(1);
			if (LHS > RHS) {
				val = TOF;
				while (val < (1.4*TOF)) {
					val = VL53L0X_get_dist_mm();
					left_motor_set_speed(250);
					right_motor_set_speed(-250);
				}
			}
			else {
				val = TOF;
				while (val < (1.4*TOF)) {
					val = VL53L0X_get_dist_mm();
					left_motor_set_speed(-250);
					right_motor_set_speed(250);
				}
			}
		}
		// VFF - Virtual Force Field Implementation
		else if (LHS > 600 || RHS > 600) {
			if (LHS > RHS) {
				left_motor_set_speed(250);
				right_motor_set_speed(-250);
			}
			else {
				left_motor_set_speed(-250);
				right_motor_set_speed(250);
			}
			chThdSleepMilliseconds(500);
		}
		else if (L_side > 500 || r_side > 500) {
			if ((L_side - r_side) > 0) {
				left_motor_set_speed(-200);
				right_motor_set_speed(350);
			}
			else if ((L_side - r_side) < 0) {
				left_motor_set_speed(350);
				right_motor_set_speed(-200);
			}
			chThdSleepMilliseconds(500);
		}
		else {
			left_motor_set_speed(400);
			right_motor_set_speed(400);
		}
	}
//	messagebus_init(&bus, &bus_lock, &bus_condvar);
//	halInit();
//	chSysInit();
//	mpu_init();
//	clear_leds();
//	spi_comm_start();
//	motors_init();
//
//	left_motor_set_speed(0);
//	right_motor_set_speed(0);
//	usb_start();
//	VL53L0X_start();
//	proximity_start();
//	calibrate_ir();
//
//	uint16_t TOF;
//	uint16_t prox_0;
//	uint16_t prox_1;
//	uint16_t prox_2;
//	uint16_t prox_3;
//	uint16_t prox_4;
//	uint16_t prox_5;
//	uint16_t prox_6;
//	uint16_t prox_7;
//	uint16_t LHS;
//	uint16_t RHS;
//
//
//	if (SDU1.config->usbp->state == USB_ACTIVE) {
//		set_led(LED2, 1);
//		chprintf((BaseSequentialStream*)&SDU1, "%4d,\n", VL53L0X_get_dist_mm());
//		chprintf((BaseSequentialStream*)&SDU1, "=================\n");
//	}
//
//	set_led(LED1, 1);
//	chThdSleepMilliseconds(750);
//	set_led(LED2, 1);
//	chThdSleepMilliseconds(750);
//	set_led(LED3, 1);
//	chThdSleepMilliseconds(750);
//	set_led(LED4, 1);
//	chThdSleepMilliseconds(750);
//	set_led(LED5, 1);
//	chThdSleepMilliseconds(750);
//	set_led(LED6, 1);
//	chThdSleepMilliseconds(750);
//	set_led(LED7, 1);
//	chThdSleepMilliseconds(750);
//	set_led(LED8, 1);
//	chThdSleepMilliseconds(750);
//	clear_leds();
//
//	while(1) {
//		chThdSleepMilliseconds(100);
//		clear_leds();
//		TOF = VL53L0X_get_dist_mm();
//		if (SDU1.config->usbp->state == USB_ACTIVE) {
//			set_led(LED2, 1);
//			chprintf((BaseSequentialStream*)&SDU1, "%4d,\n", TOF);
//			chprintf((BaseSequentialStream*)&SDU1, "-----------\n");
//		}
//		if (TOF > 50 && TOF < 300) {
//			left_motor_set_speed(TOF*0.1*100);
//			right_motor_set_speed(TOF*0.1*100);
//		}
//		else if (TOF >=300) {
//			prox_0 = get_prox(0);
//			prox_1 = get_prox(1);
//			prox_2 = get_prox(2);
//			prox_3 = get_prox(3);
//			prox_4 = get_prox(4);
//			prox_5 = get_prox(5);
//			prox_6 = get_prox(6);
//			prox_7 = get_prox(7);
//
//			RHS = prox_0 + prox_1 + prox_2 + prox_3;
//			LHS = prox_4 + prox_5 + prox_6 + prox_7;
//
//			if (LHS > RHS) {
//				left_motor_set_speed((-250));
//				right_motor_set_speed((250));
//			}
//			else {
//				left_motor_set_speed((250));
//				right_motor_set_speed((-250));
//			}
//		}
//		else if (TOF <=50 && TOF > 25) {
//			left_motor_set_speed(0);
//			right_motor_set_speed(0);
//		}
//		else if (TOF <= 25) {
//			left_motor_set_speed(-400);
//			right_motor_set_speed(-400);
//		}
//		else {
//			left_motor_set_speed(300);
//			right_motor_set_speed(-300);
//		}
//	}
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
