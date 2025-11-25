#pragma once
#include "game/ecs/components/component_kinematic_body.h"

void entity_actor_apply_acceleration(component_kinematic_body *kinematics, float acceleration, vec2 input);
void entity_actor_apply_ground_friction(component_kinematic_body *kinematics, float friction);
void entity_actor_apply_drag(component_kinematic_body *kinematics, float drag);
void entity_actor_apply_speed_cap(component_kinematic_body *kinematics, float speed_cap);