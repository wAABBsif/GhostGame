#include "system_draw_sprites.h"

#include <assert.h>
#include <math.h>

#include "core/game_time.h"
#include "core/mat3.h"
#include "../ecs_component.h"
#include "../ecs_entity.h"
#include "../ecs_system.h"
#include "../components/component_transform.h"
#include "../components/component_sprite.h"
#include "gfx/sprite.h"

const static vec2 S_VERTEX_QUADS[4] =
{
	{-0.5f, -0.5f},
	{+0.5f, -0.5f},
	{+0.5f, +0.5f},
	{-0.5f, +0.5f}
};

void system_draw_sprites_update(void)
{
	component_index transform_index = 0;
	component_index sprite_index = 0;

	for (entity_index i = 0; i < entities_get_count(); i++)
	{
		const component_transform *c_transform = system_retrieve_component(i, COMPONENT_TYPE_TRANSFORM, &transform_index);
		const component_sprite *c_sprite = system_retrieve_component(i, COMPONENT_TYPE_SPRITE, &sprite_index);

		if (!c_sprite)
			continue;

		assert(c_transform);

		if (!sprite_simple_cull(c_transform->position, c_sprite->size))
			continue;

		sprite_quad quad;
		const mat3 matrix = mat3_from_trs(c_transform->position, c_transform->rotation, c_sprite->size);

		const uint16_t tex_coords[8] =
		{
			c_sprite->texture_x, c_sprite->texture_y + c_sprite->texture_h,
			c_sprite->texture_x + c_sprite->texture_w, c_sprite->texture_y + c_sprite->texture_h,
			c_sprite->texture_x + c_sprite->texture_w, c_sprite->texture_y,
			c_sprite->texture_x, c_sprite->texture_y,
		};
		
		for (uint8_t v = 0; v < 4; v++)
		{
			const vec2 vec = vec2_transform(S_VERTEX_QUADS[v], matrix);
			quad.vertices[v].x = vec.x;
			quad.vertices[v].y = vec.y;

			int32_t w, h;
			texture_get_size(c_sprite->texture, &w, &h);

			quad.vertices[v].texture_x = (float)tex_coords[v * 2] / (float)w * 65535.0f;
			quad.vertices[v].texture_y = (float)tex_coords[v * 2 + 1] / (float)h * 65535.0f;

			quad.vertices[v].color = c_sprite->color;
			quad.vertices[v].z = c_sprite->z;
			quad.vertices[v].use_camera_to_screen_matrix = c_sprite->use_camera_to_screen_matrix;
			quad.vertices[v].texture_index = sprite_get_texture_num(c_sprite->texture);
		}

		add_sprite_quad(&quad, c_sprite->draw_sorted);
	}
}