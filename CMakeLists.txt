cmake_minimum_required(VERSION 3.20)
project(GhostGame LANGUAGES C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DIS_DEBUG")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DIS_DEBUG")

set(IMGUI_FILES)

if (CMAKE_BUILD_TYPE MATCHES Debug)
	set(IMGUI_FILES
			lib/imgui/imconfig.h
			lib/imgui/imgui.cpp
			lib/imgui/imgui.h
			lib/imgui/imgui_demo.cpp
			lib/imgui/imgui_draw.cpp
			lib/imgui/imgui_internal.h
			lib/imgui/imgui_tables.cpp
			lib/imgui/imgui_widgets.cpp
			lib/imgui/imstb_rectpack.h
			lib/imgui/imstb_textedit.h
			lib/imgui/imstb_truetype.h
			lib/imgui/backends/imgui_impl_sdl3.cpp
			lib/imgui/backends/imgui_impl_sdl3.h
			lib/imgui/backends/imgui_impl_opengl3.cpp
			lib/imgui/backends/imgui_impl_opengl3.h
	)
endif ()

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
		src/gfx/camera.c
		src/gfx/camera.h
		src/gfx/texture.c
		src/gfx/texture.h
		src/gfx/color.h
		src/core/hash.h
		src/input/input.c
		src/input/input.h
		src/input/input_device.h
		src/input/device_kbm.c
		src/input/device_kbm.h
		src/input/device_gamepad.c
		src/input/device_gamepad.h
		src/input/input_binding.h
		src/game/ecs/ecs_entity.c
		src/game/ecs/ecs_entity.h
		src/game/ecs/ecs_component.c
		src/game/ecs/ecs_component.h
		src/game/ecs/ecs_system.c
		src/game/ecs/ecs_system.h
		src/game/ecs/components/component_transform.h
		src/game/ecs/components/component_kinematic_body.h
		src/game/ecs/components/component_sprite.h
		src/game/ecs/systems/system_kinematics.c
		src/game/ecs/systems/system_kinematics.h
		src/game/ecs/systems/system_draw_sprites.c
		src/game/ecs/systems/system_draw_sprites.h
		src/gfx/sprite.c
		src/gfx/sprite.h
		src/gfx/lighting.c
		src/gfx/lighting.h
		src/game/ecs/systems/system_draw_lighting.c
		src/game/ecs/systems/system_draw_lighting.h
		src/game/ecs/entities/entity_tile.c
		src/game/ecs/entities/entity_tile.h
		src/game/level/tile_atlas.c
		src/game/level/tile_atlas.h
		src/game/ecs/components/component_collider.h
		src/game/level/tile.h
		src/game/level/level.c
		src/game/level/level.h
		src/game/level/object.h
		src/game/level/object.c
		src/game/ecs/components/component_tile.h
		src/game/ecs/systems/system_tiles.c
		src/game/ecs/systems/system_tiles.h
		src/game/ecs/components/component_update.h
		src/game/ecs/systems/system_update.c
		src/game/ecs/systems/system_update.h
		src/game/ecs/systems/system_dynamics.c
		src/game/ecs/systems/system_dynamics.h
		src/game/ecs/ecs_deletion.c
		src/game/ecs/ecs_deletion.h
		src/core/collision.c
		src/core/collision.h
		src/game/ecs/components/component_controller.h
		src/game/ecs/systems/system_controller.c
		src/game/ecs/systems/system_controller.h
		src/game/ecs/systems/controllers/controller_player.c
		src/game/ecs/systems/controllers/controller_player.h
		src/game/ecs/components/component_movement_properties.h
		src/game/ecs/entities/entity_actor.c
		src/game/ecs/entities/entity_actor.h
		src/game/ecs/ecs_tag.c
		src/game/ecs/ecs_tag.h
		src/gfx/tiles.c
		src/gfx/tiles.h
		src/audio/audio.c
		src/audio/audio.h
		src/audio/sound.c
		src/audio/sound.h
		src/audio/audio_source.c
		src/audio/audio_source.h
		src/audio/music.c
		src/audio/music.h
		src/audio/vorbis_implementation.c
		src/audio/vorbis_implementation.h
		src/debug_ui/imgui_impl.cpp
		src/debug_ui/imgui_impl.h
		src/debug_ui/debug_ui.c
		src/debug_ui/debug_ui.h
		src/core/game_assert.h
		src/debug_ui/log_window.c
		src/debug_ui/log_window.h
		src/debug_ui/objects_window.c
		src/debug_ui/objects_window.h
		src/debug_ui/inspector_window.c
		src/debug_ui/inspector_window.h
		${IMGUI_FILES}
)

include_directories(src)
include_directories(lib)
include_directories(lib/imgui)

add_subdirectory(lib/SDL EXCLUDE_FROM_ALL)
include_directories(lib/SDL/include)
target_link_libraries(${PROJECT_NAME} SDL3::SDL3)

find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)

include_directories(lib/GLAD/include)

if (WIN32)
	add_library(OpenAL STATIC IMPORTED)
	set_target_properties(OpenAL PROPERTIES
			IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/AL/OpenAL32.lib"
	)
	target_link_libraries(${PROJECT_NAME} OpenAL)
else ()
	find_package(OpenAL CONFIG REQUIRED)
	target_link_libraries(${PROJECT_NAME} OpenAL::OpenAL)
endif ()

target_link_libraries(${PROJECT_NAME} m)

if (WIN32)
	set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/res)
execute_process(COMMAND python ${CMAKE_SOURCE_DIR}/scripts/build_res.py ${CMAKE_BINARY_DIR}/res)
