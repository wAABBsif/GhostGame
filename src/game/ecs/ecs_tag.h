#pragma once
#include <stdint.h>

#define TAG_DELETION			0

#define TAG_COUNT				1
#define TAG_GET_MASK(tag)		(1 << (tag))

typedef uint16_t ecs_tag;
