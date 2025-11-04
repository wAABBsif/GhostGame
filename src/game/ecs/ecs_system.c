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
#include "entities/entity_tile.h"
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