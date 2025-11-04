#pragma once
#include <stdbool.h>

#include "color.h"
#include "texture.h"

#define MAX_LIGHTS				32

typedef int8_t light_type;

#define LIGHT_TYPE_AREA			0
#define LIGHT_TYPE_RADIAL		1

typedef struct light_vertex
{
	int16_t x;
	int16_t y;

	uint16_t texture_x;
	uint16_t texture_y;

	color	color;

	light_type	type;
	int8_t priority;
	int8_t max_z;
} light_vertex;

typedef struct light_quad
{
	light_vertex vertices[4];
} light_quad;

void lighting_init(void);
void lighting_terminate(void);

void add_light_quad(const light_quad *quad);
void draw_lighting(void);