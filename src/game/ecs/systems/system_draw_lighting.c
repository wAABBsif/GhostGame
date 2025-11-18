#include "system_draw_lighting.h"

#include <assert.h>
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

void system_draw_lighting_update(void **components, entity_index entity)
{
	if (!entity_has_component(entity, COMPONENT_TYPE_LIGHT))
		return;

	assert(entity_has_component(entity, COMPONENT_TYPE_TRANSFORM));

	const component_transform *c_transform = components[COMPONENT_TYPE_TRANSFORM];
	const component_light *c_light = components[COMPONENT_TYPE_LIGHT];

	if (!sprite_simple_cull(c_transform->position, c_light->size))
		return;

	light_quad quad;
	const mat3 matrix = mat3_from_trs(c_transform->position, c_transform->rotation, c_light->size);

	for (uint8_t v = 0; v < 4; v++)
	{
		const vec2 vec = vec2_transform(S_VERTEX_QUADS[v], matrix);
		quad.vertices[v].x = lroundf(vec.x);
		quad.vertices[v].y = lroundf(vec.y);

		quad.vertices[v].texture_x = S_TEX_COORDS[v * 2];
		quad.vertices[v].texture_y = S_TEX_COORDS[v * 2 + 1];

		quad.vertices[v].color = c_light->color;
		quad.vertices[v].priority = c_light->priority;
		quad.vertices[v].z = c_light->z;
		quad.vertices[v].type = c_light->type;
	}

	add_light_quad(&quad);
}