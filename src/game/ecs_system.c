#include "ecs_system.h"

#include <tgmath.h>

#include "ecs_entity.h"
#include "components/component_light.h"
#include "components/component_position.h"
#include "components/component_rotation.h"
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
	for (int i = -1; i <= 1; i++)
	{
		entities_add();
		component_position *pos =  entity_add_component(COMPONENT_TYPE_POSITION);
		pos->value = (vec2){i * 32, 0};

		component_rotation *rot =  entity_add_component(COMPONENT_TYPE_ROTATION);
		rot->value = 0;

		component_sprite *spr =  entity_add_component(COMPONENT_TYPE_SPRITE);
		spr->size = (vec2){16.0, 16.0};
		spr->texture = texture_get("res/sprites/test.png");
		spr->texture_x = 0;
		spr->texture_y = 0;
		spr->texture_w = 16;
		spr->texture_h = 16;
		spr->color = (color){0xFF, 0xFF, 0xFF, 0xFF};
		spr->z = i;
		spr->use_camera_to_screen_matrix = false;
		spr->draw_sorted = false;

		component_velocity *vel =  entity_add_component(COMPONENT_TYPE_VELOCITY);
		vel->value = (vec2){0, 0};
	}
	{
		entities_add();
		component_position *pos =  entity_add_component(COMPONENT_TYPE_POSITION);
		pos->value = (vec2){0, 0};

		component_light *light =  entity_add_component(COMPONENT_TYPE_LIGHT);
		light->size = (vec2){1000, 1000};
		light->color = (color){0xFF, 0xFF, 0xFF, 0xFF};
		light->priority = -64;
		light->z = 2;
		light->type = LIGHT_TYPE_AREA;
	}
	{
		entities_add();
		component_position *pos =  entity_add_component(COMPONENT_TYPE_POSITION);
		pos->value = (vec2){0, 0};

		component_light *light =  entity_add_component(COMPONENT_TYPE_LIGHT);
		light->size = (vec2){100.0, 100.0};
		light->color = (color){0x40, 0x40, 0x40, 0xFF};
		light->priority = 0;
		light->z = 0;
		light->type = LIGHT_TYPE_RADIAL;
	}

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