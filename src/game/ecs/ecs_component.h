#pragma once
#include <stdbool.h>
#include <stdint.h>

#define COMPONENT_TYPE_TRANSFORM			0
#define COMPONENT_TYPE_KINEMATIC_BODY		1
#define COMPONENT_TYPE_SPRITE				2
#define COMPONENT_TYPE_LIGHT				3
#define COMPONENT_TYPE_COLLIDER				4
#define COMPONENT_TYPE_TILE					5
#define COMPONENT_TYPE_UPDATE				6
#define COMPONENT_TYPE_CONTROLLER			7
#define COMPONENT_TYPE_MOVEMENT_PROPERTIES	8

#define COMPONENT_TYPE_COUNT				9
#define COMPONENT_MASK(component)			(1 << (component))

typedef uint16_t component_type;
typedef uint32_t component_mask;
typedef int16_t entity_id;

void components_init(void);
void components_terminate(void);

void *component_add(component_type type, entity_id index);
void component_remove(component_type type, entity_id index);

bool component_exists(component_type type, entity_id index);
void *component_get(component_type type, entity_id index);
uint16_t component_get_size(component_type type);