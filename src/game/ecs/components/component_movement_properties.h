#pragma once

typedef struct component_movement_properties
{
	float acceleration;
	float ground_friction;
	float wall_friction;
	float deceleration;
	float drag;

	float top_speed;
	float max_speed;
} component_movement_properties;