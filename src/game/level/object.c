#include "object.h"
#include "level.h"

vec2 object_position_to_vec2(const vec2 chunk_offset, const int8_t x, const int8_t y)
{
	vec2 result;
	result.x = chunk_offset.x + x * (float)LEVEL_CHUNK_WIDTH_IN_PIXELS / 256.0f;
	result.y = chunk_offset.y + y * (float)LEVEL_CHUNK_HEIGHT_IN_PIXELS / 256.0f;
	return result;
}
