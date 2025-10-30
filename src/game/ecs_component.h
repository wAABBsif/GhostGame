#pragma once
#include <stdint.h>

#define COMPONENT_TYPE_DELETION_FLAG		0
#define COMPONENT_TYPE_POSITION				1
#define COMPONENT_TYPE_ROTATION				2
#define COMPONENT_TYPE_SCALE				3
#define COMPONENT_TYPE_VELOCITY				4

#define COMPONENT_TYPE_COUNT				5
#define COMPONENT_GET_MASK(component)		(1 << (component))

typedef uint16_t component_type;
typedef uint16_t component_index;

typedef struct register_component_entry
{
	uint16_t size;
	uint16_t count;
} register_component_entry;

void components_init(void);
void components_terminate(void);

component_index components_add(component_type type);
void components_remove(component_type type, component_index index);

void *components_get(component_type type);
uint16_t components_get_size(component_type type);
component_index components_get_count(component_type type);