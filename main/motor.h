#pragma once
#include <stdint.h>

void motor_hw_init(void);
void motor_start(uint8_t speed_percent);
void motor_stop(void);
void motor_set_speed(uint8_t speed_percent);
void motor_all_off(void);