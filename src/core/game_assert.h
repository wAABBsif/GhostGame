#pragma once
#include <stdlib.h>
#include "logging.h"

#if defined(IS_DEBUG)
#define GAME_ASSERT(statement) ({if (!(statement)) { LOG_ERROR("Assertion failed: " #statement); exit(1); } })
#else
#define GAME_ASSERT(statement)
#endif