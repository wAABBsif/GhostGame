#include "drawing.h"

#include "shader.h"
#include "sprite_renderer.h"
#include "tile_renderer.h"
#include "core/logging.h"
#include "glad/glad.h"

const draw_func DRAW_FUNCTIONS[] =
{
	NULL,
	tile_renderer_draw,
	sprite_renderer_draw,
};

static draw_command s_commands[DRAW_COMMAND_COUNT];

draw_command *create_draw_command(const uint16_t min_index)
{
	for (int i = min_index; i < DRAW_COMMAND_COUNT; i++)
	{
		if (s_commands[i].command == DRAW_COMMAND_NONE)
			return &s_commands[i];
	}

	log_error("Could not create draw command!");
	return NULL;
}

void release_draw_command(draw_command *command)
{
	command->command = DRAW_COMMAND_NONE;
}

void handle_draw_commands()
{
	for (int i = 0; i < DRAW_COMMAND_COUNT; i++)
	{
		if (s_commands[i].command == DRAW_COMMAND_NONE)
			break;

		shader_set(s_commands[i].shader);

		DRAW_FUNCTIONS[s_commands[i].command](s_commands[i].shader);

		glBindVertexArray(s_commands[i].vao);
		glDrawElements(GL_TRIANGLES, s_commands[i].element_count, GL_UNSIGNED_SHORT, 0);
	}
}