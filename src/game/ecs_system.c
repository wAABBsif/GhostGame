#include "ecs_system.h"

#include <tgmath.h>

#include "ecs_entity.h"
#include "components/component_light.h"
#include "components/component_position.h"
#include "components/component_rotation.h"
#include "components/component_size.h"
#include "components/component_sprite.h"
#include "components/component_velocity.h"
#include "core/game_time.h"
#include "core/logging.h"
#include "systems/system_deletion.h"
#include "systems/system_draw_lighting.h"
#include "systems/system_draw_sprites.h"
#include "systems/system_kinematics.h"

const ecs_system SYSTEMS[] =
{
	(ecs_system){system_deletion_init, system_deletion_update},
	(ecs_system){system_kinematics_init, system_kinematics_update},
	(ecs_system){system_draw_sprites_init, system_draw_sprites_update},
	(ecs_system){system_draw_lighting_init, system_draw_lighting_update}
};

void *system_retrieve_component(const entity_index entity, const component_type type, component_index *index)
{
	if (!entity_has_component(entity, type))
		return NULL;

	void *result = components_get_index(type, *index);
	(*index)++;
	return result;
}

void systems_init(void)
{
	for (int i = 0; i <= 2; i++)
	{
		entities_add();
		component_position *pos =  entity_add_component(COMPONENT_TYPE_POSITION);
		pos->value = (vec2){i * 32 + 32, 0};

		component_rotation *rot =  entity_add_component(COMPONENT_TYPE_ROTATION);
		rot->value = 0;

		component_size *size =  entity_add_component(COMPONENT_TYPE_SIZE);
		size->value = (vec2){16.0, 16.0};

		component_sprite *spr =  entity_add_component(COMPONENT_TYPE_SPRITE);
		if (i == 0)
			spr->texture = texture_get("res/sprites/test.png");
		else
			spr->texture = texture_get("res/sprites/test2.png");
		spr->texture_x = 0;
		spr->texture_y = 0;
		spr->texture_w = 16;
		spr->texture_h = 16;
		spr->color = (color){i == 0 ? 0xFF : 0x00, i == 1 ? 0xFF : 0x00, i == 2 ? 0xFF : 0x00, i == 1 ? 0x80 : 0xFF};
		spr->z = i;
		spr->use_camera_to_screen_matrix = false;
		spr->draw_sorted = true;

		component_velocity *vel =  entity_add_component(COMPONENT_TYPE_VELOCITY);
		vel->value = (vec2){48 - i * 32, 0};
	}

	entities_add();
	component_position *pos =  entity_add_component(COMPONENT_TYPE_POSITION);
	pos->value = (vec2){0, 0};

	component_size *size =  entity_add_component(COMPONENT_TYPE_SIZE);
	size->value = (vec2){1000.0, 1000.0};

	component_light *light =  entity_add_component(COMPONENT_TYPE_LIGHT);
	light->color = COLOR_WHITE;
	light->z = 0;
	light->type = LIGHT_TYPE_AREA;

	for (int i = 0; i < sizeof(SYSTEMS) / sizeof(ecs_system); i++)
	{
		SYSTEMS[i].init();
	}
}

void systems_update(void)
{
	for (int i = 0; i < sizeof(SYSTEMS) / sizeof(ecs_system); i++)
	{
		SYSTEMS[i].update();
	}
}