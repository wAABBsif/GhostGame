#pragma once
#include "input_device.h"

typedef struct input_device_kbm
{
	input_device base;
	const bool *keys;
} input_device_kbm;

input_device_kbm *device_kbm_init();