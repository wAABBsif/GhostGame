#include "system_draw_lighting.h"

#include <math.h>

#include "core/game_time.h"
#include "core/mat3.h"
#include "../ecs_component.h"
#include "../ecs_entity.h"
#include "../ecs_system.h"
#include "../components/component_light.h"
#include "../components/component_transform.h"
#include "gfx/lighting.h"
#include "gfx/sprite.h"

const static vec2 S_VERTEX_QUADS[4] =
{
	{-0.5f, -0.5f},
	{+0.5f, -0.5f},
	{+0.5f, +0.5f},
	{-0.5f, +0.5f}
};

const static uint16_t S_TEX_COORDS[8] =
{
	0, 1,
	1, 1,
	1, 0,
	0, 0
};

void system_draw_lighting_update(entity_id entity)
{
	const component_transform *c_transform = component_get(COMPONENT_TYPE_TRANSFORM, entity);
	const component_light *c_light = component_get(COMPONENT_TYPE_LIGHT, entity);

	const vec2 rounded_position = vec2_truncate(c_transform->position);

	if (!sprite_simple_cull(rounded_position, c_light->size))
		return;

	light_quad quad;
	const mat3 matrix = mat3_from_trs(rounded_position, c_transform->rotation, c_light->size);

	for (uint8_t v = 0; v < 4; v++)
	{
		const vec2 vec = vec2_transform(S_VERTEX_QUADS[v], matrix);
		quad.vertices[v].x = lroundf(vec.x);
		quad.vertices[v].y = lroundf(vec.y);

		quad.vertices[v].texture_x = S_TEX_COORDS[v * 2];
		quad.vertices[v].texture_y = S_TEX_COORDS[v * 2 + 1];

		quad.vertices[v].color = c_light->color;
		quad.vertices[v].intensity_multiplier = c_light->intensity_multiplier;
		quad.vertices[v].priority = c_light->priority;
		quad.vertices[v].z = c_light->z;
		quad.vertices[v].type = c_light->type;
	}

	add_light_quad(&quad);
}