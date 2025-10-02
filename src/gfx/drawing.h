#pragma once

#include <stdint.h>
#include "core/hash.h"

#define QUAD_INDICES {0, 1, 2, 2, 3, 0}

typedef struct draw_command draw_command;

typedef uint16_t command_type;
typedef uint32_t vertex_array_object;
typedef uint32_t vertex_buffer_object;
typedef uint32_t element_buffer_object;
typedef hash shader_h;
typedef void (*draw_func)(draw_command *cmd);

#define DRAW_COMMAND_NONE   0
#define DRAW_COMMAND_TILE   1
#define DRAW_COMMAND_SPRITE_UNSORTED 2
#define DRAW_COMMAND_SPRITE_SORTED 3

#define DRAW_COMMAND_COUNT  16

struct draw_command
{
	command_type command;
	vertex_array_object vao;
	vertex_buffer_object vbo;
	element_buffer_object ebo;
	shader_h shader;
	uint16_t element_count;
};

draw_command *create_draw_command(uint16_t min_index);
void release_draw_command(draw_command *command);
void handle_draw_commands();