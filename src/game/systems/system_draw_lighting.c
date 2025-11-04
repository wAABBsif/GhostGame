#include "system_draw_lighting.h"

#include <assert.h>
#include <math.h>

#include "core/game_time.h"
#include "core/mat3.h"
#include "game/ecs_component.h"
#include "game/ecs_entity.h"
#include "game/ecs_system.h"
#include "game/components/component_light.h"
#include "game/components/component_position.h"
#include "game/components/component_rotation.h"
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

void system_draw_lighting_init(void)
{

}

void system_draw_lighting_update(void)
{
	component_index position_index = 0;
	component_index rotation_index = 0;
	component_index light_index = 0;

	for (entity_index i = 0; i < entities_get_count(); i++)
	{
		const component_position *c_position = system_retrieve_component(i, COMPONENT_TYPE_POSITION, &position_index);
		const component_rotation *c_rotation = system_retrieve_component(i, COMPONENT_TYPE_ROTATION, &rotation_index);
		const component_light *c_light = system_retrieve_component(i, COMPONENT_TYPE_LIGHT, &light_index);

		if (!c_light)
			continue;

		assert(c_position);

		if (!sprite_simple_cull(c_position->value, c_light->size))
			continue;

		light_quad quad;
		const mat3 matrix = mat3_from_trs(c_position->value, c_rotation ? c_rotation->value : 0, c_light->size);

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
}