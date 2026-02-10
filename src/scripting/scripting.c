#include "scripting.h"
#include "core/logging.h"

#include <assert.h>
#include <stdlib.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdlib.h>

static lua_State *s_lua_state;

void scripting_init()
{
    LOG_MESSAGE("Initializing scripting...");
    s_lua_state = luaL_newstate();
    if (s_lua_state == NULL)
    {
        LOG_ERROR("Failed to open lua");
        exit(EXIT_FAILURE);
    }

    luaL_openlibs(s_lua_state);
}

void scripting_terminate()
{
    LOG_MESSAGE("Terminating scripting...");
    lua_close(s_lua_state);
}
