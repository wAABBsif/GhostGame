#include "object_enemy_base.h"

uint16_t object_enemy_get_health(const object_enemy_base *base)
{
	return base->_health;
}

void object_enemy_set_health(object_enemy_base *base, int32_t value)
{
	if (base->set_health_callback != NULL)
		base->set_health_callback(base, &value);
	base->_health = value;
}