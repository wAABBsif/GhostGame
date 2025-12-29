layout(location = 0) in vec2 in_position;
layout(location = 1) in vec2 in_tex_coords;
layout(location = 2) in float in_z;

out vec2 tex_coords;

uniform mat3 world_to_screen_matrix;
uniform vec2 origin;

void main()
{
    vec3 pos = world_to_screen_matrix * vec3(in_position.xy * 16 + origin, 1);
    gl_Position = vec4(pos.xy, -in_z, 1);

    tex_coords = in_tex_coords;
}