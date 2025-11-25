#include "entity_actor.h"

#include <math.h>

#include "core/game_time.h"

void entity_actor_apply_acceleration(component_kinematic_body *kinematics, const float acceleration, const vec2 input)
{
	kinematics->velocity = vec2_add(kinematics->velocity, vec2_mul(input, game_time_get_delta() * acceleration));
}

void entity_actor_apply_ground_friction(component_kinematic_body *kinematics, const float friction)
{
	float speed = vec2_mag(kinematics->velocity);
	speed -= friction * game_time_get_delta();
	if (speed < 0)
		speed = 0;
	kinematics->velocity = vec2_set_mag(kinematics->velocity, speed);
}

void entity_actor_apply_drag(component_kinematic_body *kinematics, const float drag)
{
	float speed = vec2_mag(kinematics->velocity);
	speed *= powf(1 - drag, game_time_get_delta());
	kinematics->velocity = vec2_set_mag(kinematics->velocity, speed);
}

void entity_actor_apply_speed_cap(component_kinematic_body *kinematics, const float speed_cap)
{
	kinematics->velocity = vec2_clamp_mag(kinematics->velocity, speed_cap);
}