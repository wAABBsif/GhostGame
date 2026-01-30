#include "inspector_window.h"

#include <math.h>
#include <stddef.h>

#include "imgui_impl.h"
#include "objects_window.h"
#include "game/ecs/ecs_entity.h"
#include "game/ecs/components/component_collider.h"
#include "game/ecs/components/component_controller.h"
#include "game/ecs/components/component_kinematic_body.h"
#include "game/ecs/components/component_light.h"
#include "game/ecs/components/component_sprite.h"
#include "game/ecs/components/component_tile.h"
#include "game/ecs/components/component_transform.h"

typedef void (*inspector_func)(entity_id entity);

void inspector_component_transform(entity_id entity);
void inspector_component_kinematic_body(entity_id entity);
void inspector_component_sprite(entity_id entity);
void inspector_component_light(entity_id entity);
void inspector_component_collider(entity_id entity);
void inspector_component_tile(entity_id entity);
void inspector_component_update(entity_id entity);
void inspector_component_controller(entity_id entity);
void inspector_component_movement_properties(entity_id entity);

const inspector_func INSPECTOR_FUNCS[] =
{
	inspector_component_transform,
	inspector_component_kinematic_body,
	inspector_component_sprite,
	inspector_component_light,
	inspector_component_collider,
	inspector_component_tile,
	inspector_component_update,
	inspector_component_controller,
	inspector_component_movement_properties,
};

void inspector_window_update()
{
	imgui_begin("Inspector", NULL, 0);
	const entity_id selection = objects_window_get_selection();
	if (selection == ENTITY_ID_INVALID || !entity_exists(selection))
	{
		imgui_text("No selection");
		imgui_end();
		return;
	}

	for (int i = 0; i < sizeof(INSPECTOR_FUNCS) / sizeof(INSPECTOR_FUNCS[0]); i++)
	{
		if (component_exists(i, selection))
		{
			INSPECTOR_FUNCS[i](selection);
		}
	}

	imgui_end();
}

void inspector_component_transform(const entity_id entity)
{
	if (!imgui_collapsing_header("Transform"))
		return;

	component_transform *transform = component_get(COMPONENT_TYPE_TRANSFORM, entity);

	imgui_drag_vec2("Position", &transform->position, 0.1f, 0, 0, "%.1f", 0);
	imgui_drag_float("Rotation", &transform->rotation, 0.01f, -M_PI, M_PI, "%.2f", IMGUI_SLIDER_FLAGS_WRAP_AROUND);
}

void inspector_component_kinematic_body(const entity_id entity)
{
	if (!imgui_collapsing_header("Kinematic Body"))
		return;

	component_kinematic_body *kinematic_body = component_get(COMPONENT_TYPE_KINEMATIC_BODY, entity);

	imgui_drag_vec2("Velocity", &kinematic_body->velocity, 0.1f, 0, 0, "%.1f", 0);
}

void inspector_component_sprite(const entity_id entity)
{
	if (!imgui_collapsing_header("Sprite"))
		return;

	component_sprite *sprite = component_get(COMPONENT_TYPE_SPRITE, entity);

	imgui_drag_vec2("Size", &sprite->size, 0.01f, 0, 0, "%.2f", 0);
	imgui_text("Texture?");

	int max_ints[2];
	texture_get_size(sprite->texture, &max_ints[0], &max_ints[1]);
	const int max = max_ints[0] > max_ints[1] ? max_ints[0] : max_ints[1];

	imgui_drag_vec2u16("Texture Position", &sprite->texture_pos, 1, 0, max, "%d", IMGUI_SLIDER_FLAGS_WRAP_AROUND);
	imgui_drag_vec2u16("Texture Size", &sprite->texture_size, 1, 0, max, "%d", IMGUI_SLIDER_FLAGS_WRAP_AROUND);
	imgui_color_edit("Color", &sprite->color, 0);

	int z = sprite->z;
	imgui_slider_int("Sprite Z", &z, -128, 127, "%d", IMGUI_SLIDER_FLAGS_ALWAYS_CLAMP);
	sprite->z = z;

	imgui_checkbox("Use Camera to Screen Matrix", &sprite->use_camera_to_screen_matrix);
}

void inspector_component_light(const entity_id entity)
{
	if (!imgui_collapsing_header("Light"))
		return;

	component_light *light = component_get(COMPONENT_TYPE_LIGHT, entity);

	imgui_drag_vec2("Size", &light->size, 0.01f, 0, 0, "%.2f", 0);
	imgui_color_edit("Color", &light->color, 0);

	if (imgui_begin_combo("Light Type", light->type == LIGHT_TYPE_AREA ? "Area" : "Radial", 0))
	{
		if (imgui_selectable("Area", light->type == LIGHT_TYPE_AREA, 0, VEC2_ZERO))
			light->type = LIGHT_TYPE_AREA;

		if (imgui_selectable("Radial", light->type == LIGHT_TYPE_RADIAL, 0, VEC2_ZERO))
			light->type = LIGHT_TYPE_RADIAL;

		imgui_end_combo();
	}

	int temp = light->priority;
	imgui_slider_int("Priority", &temp, -128, 127, "%d", IMGUI_SLIDER_FLAGS_ALWAYS_CLAMP);
	light->priority = temp;

	temp = light->z;
	imgui_slider_int("Light Z", &temp, -128, 127, "%d", IMGUI_SLIDER_FLAGS_ALWAYS_CLAMP);
	light->z = temp;

	temp = light->intensity_multiplier;
	imgui_slider_int("Intensity Multiplier", &temp, 0, 255, "%d", IMGUI_SLIDER_FLAGS_ALWAYS_CLAMP);
	light->intensity_multiplier = temp;
}

void inspector_component_collider(const entity_id entity)
{
	if (!imgui_collapsing_header("Collider"))
		return;

	component_collider *collider = component_get(COMPONENT_TYPE_COLLIDER, entity);

	imgui_drag_vec2u8("Radius", &collider->radius, 1, 0, 255, "%d", IMGUI_SLIDER_FLAGS_WRAP_AROUND);
}

void inspector_component_tile(const entity_id entity)
{
	if (!imgui_collapsing_header("Tile"))
		return;

	component_tile *tile = component_get(COMPONENT_TYPE_TILE, entity);

	int chunk = tile->chunk_index;
	imgui_input_int("Chunk", &chunk, 1, 0, IMGUI_INPUT_TEXT_FLAGS_READ_ONLY);
}

void inspector_component_update(const entity_id entity)
{
	if (!imgui_collapsing_header("Update"))
		return;

	imgui_text("Planning on removing this component and replacing it with scripting...");
}

void inspector_component_controller(const entity_id entity)
{
	if (!imgui_collapsing_header("Controller"))
		return;

	component_controller *controller = component_get(COMPONENT_TYPE_CONTROLLER, entity);

	if (imgui_begin_combo("Type", "Player", 0))
	{
		if (imgui_selectable("Player", controller->type == CONTROLLER_TYPE_PLAYER, 0, VEC2_ZERO))
			controller->type = CONTROLLER_TYPE_PLAYER;

		imgui_end_combo();
	}

	bool temp = controller->shoot_down;
	imgui_checkbox("Shoot Down", &temp);
	controller->shoot_down = temp;

	temp = controller->shoot_pressed;
	imgui_checkbox("Shoot Pressed", &temp);
	controller->shoot_pressed = temp;

	temp = controller->shoot_released;
	imgui_checkbox("Shoot Released", &temp);
	controller->shoot_released = temp;

	temp = controller->aux_down;
	imgui_checkbox("Aux Down", &temp);
	controller->aux_down = temp;

	temp = controller->aux_pressed;
	imgui_checkbox("Aux Pressed", &temp);
	controller->aux_pressed = temp;

	temp = controller->aux_released;
	imgui_checkbox("Aux Released", &temp);
	controller->aux_released = temp;

	imgui_slider_vec2("Move", &controller->move, -1, 1, "%.2f", IMGUI_SLIDER_FLAGS_ALWAYS_CLAMP);
	imgui_slider_vec2("Aim", &controller->aim, -1, 1, "%.2f", IMGUI_SLIDER_FLAGS_ALWAYS_CLAMP);
}

void inspector_component_movement_properties(const entity_id entity)
{
	if (!imgui_collapsing_header("Movement Properties"))
		return;

	imgui_text("Planning on removing this component and replacing it with scripting...");
}