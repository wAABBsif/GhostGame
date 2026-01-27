#include "objects_window.h"

#include <stddef.h>
#include <stdio.h>

#include "imgui_impl.h"
#include "game/ecs/ecs_entity.h"

static entity_id selected_entity;

void objects_window_update()
{
	imgui_begin("Objects", NULL, IMGUI_WINDOW_FLAGS_NONE);
	imgui_begin_listbox("##Objects", imgui_get_content_region_avail());
	for (entity_id i = 0; i < ECS_MAX_ENTITIES; i++)
	{
		if (!entity_exists(i))
			continue;
		static char name[6];
		sprintf(name, "%i", i);
		bool selected = selected_entity == i;
		selected = imgui_selectable(name, selected, 0, VEC2_ZERO);
		if (selected)
			selected_entity = i;
	}
	imgui_end_listbox();
	imgui_end();
}
