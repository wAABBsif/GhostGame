layout(location = 0) in vec2 in_position;
layout(location = 1) in vec2 in_tex_coords;
layout(location = 2) in vec4 in_color;
layout(location = 3) in float in_z;
layout(location = 4) in int in_light_type;

out vec2 tex_coords;
out vec4 color;
out int light_type;

uniform mat3 world_to_screen_matrix;

void main()
{
    vec3 pos = world_to_screen_matrix * vec3(in_position.xy, 1);
    gl_Position = vec4(pos.xy, -in_z, 1);

    tex_coords = in_tex_coords;
    color = in_color;
    light_type = in_light_type;
}