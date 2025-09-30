#version 330 core

layout(location = 0) in uint data;

out vec2 tex_coords;

uniform mat3 world_to_screen_matrix;

void main()
{
    vec3 tile_position = world_to_screen_matrix * vec3(((data >> 22) & uint(0x3FF)) * 16.0, ((data >> 12) & uint(0x3FF)) * 16.0, 1);
    gl_Position = vec4(tile_position.xy, 1 - float((data >> 8) & uint(0xF)) / 16.0, 1);
    tex_coords = vec2((data & uint(0xF)) / 16.0, ((data >> 4) & uint(0xF)) / 16.0);
}