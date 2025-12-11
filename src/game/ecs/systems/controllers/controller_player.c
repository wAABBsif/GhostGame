#include "controller_player.h"

#include "game/ecs/ecs_component.h"
#include "game/ecs/components/component_controller.h"
#include "input/input.h"

void controller_player(component_controller *controller, entity_id entity)
{
	controller->move = input_get_vector(INPUT_ACTION_MOVE_L, INPUT_ACTION_MOVE_R, INPUT_ACTION_MOVE_D, INPUT_ACTION_MOVE_U);
	controller->aim = input_get_vector(INPUT_ACTION_AIM_L, INPUT_ACTION_AIM_R, INPUT_ACTION_AIM_D, INPUT_ACTION_AIM_U);

	controller->shoot_down = input_is_action_down(INPUT_ACTION_SHOOT);
	controller->shoot_pressed = input_was_action_pressed(INPUT_ACTION_SHOOT);
	controller->shoot_released = input_was_action_released(INPUT_ACTION_SHOOT);

	controller->aux_down = input_is_action_down(INPUT_ACTION_AUX);
	controller->aux_pressed = input_was_action_pressed(INPUT_ACTION_AUX);
	controller->aux_released = input_was_action_released(INPUT_ACTION_AUX);
}
