cmake_minimum_required(VERSION 3.20)
project(GhostGame C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DIS_DEBUG")

add_executable(GhostGame src/main.c
		src/game/game.c
		src/game/game.h
		src/core/logging.h
		src/core/logging.c
		src/game/sdl_interface.c
		src/game/sdl_interface.h
		src/gfx/gfx.c
		src/gfx/gfx.h
		src/gfx/window.c
		src/gfx/window.h
		lib/GLAD/src/glad.c
		src/core/game_time.h
		src/core/game_time.c
		src/gfx/shader.c
		src/gfx/shader.h
		src/core/hash_map.c
		src/core/hash_map.h
		src/core/vec2.c
		src/core/vec2.h
		src/core/mat3.c
		src/core/mat3.h
		src/game/camera.c
		src/game/camera.h
)

include_directories(src)

add_subdirectory(lib/SDL EXCLUDE_FROM_ALL)
include_directories(lib/SDL/include)
target_link_libraries(${PROJECT_NAME} SDL3::SDL3)

add_subdirectory(lib/SDL_image EXCLUDE_FROM_ALL)
include_directories(lib/SDL_image/include)
target_link_libraries(${PROJECT_NAME} SDL3_image::SDL3_image)

add_subdirectory(lib/SDL_mixer EXCLUDE_FROM_ALL)
include_directories(lib/SDL_mixer/include)
target_link_libraries(${PROJECT_NAME} SDL3_mixer::SDL3_mixer)

find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)

include_directories(lib/GLAD/include)

if (WIN32)
	set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/res
		${CMAKE_BINARY_DIR}/res)