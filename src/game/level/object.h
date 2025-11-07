#pragma once
#include <stdint.h>

#define OBJECT_TYPE_NONE		0

#define OBJECT_TYPE_COUNT		1

typedef struct object
{
	uint16_t type;
	int8_t x;
	int8_t y;
	char data[12];
} object;
