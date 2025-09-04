#pragma once
#include <stdbool.h>

bool sdl_interface_init(void);
void sdl_interface_update(void);
void sdl_interface_terminate(void);
bool sdl_interface_ready_to_quit(void);