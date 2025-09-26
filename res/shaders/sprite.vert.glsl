#version 330 core

layout(location = 0) in uvec3 data;

out uint tex_index;
out vec2 tex_coords;
out vec4 color;

uniform mat3 world_to_screen_matrix;

void main()
{
    vec3 pos = world_to_screen_matrix * vec3(data.x & uint(0xFFFF), (data.x >> 16) & uint(0xFFFF), 1);
    gl_Position = vec4(tile_position.xy, (data.z >> 8) & uint(0xFF), 1);
    tex_coords = vec2(data.y & uint(0xFFFF), (data.y >> 16) & uint(0xFFFF));
    tex_index = data.z & uint(0xFF);
}