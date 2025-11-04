layout(location = 0) in vec2 in_position;
layout(location = 1) in vec2 in_tex_coords;
layout(location = 2) in vec4 in_color;
layout(location = 3) in int in_light_type;
layout(location = 4) in float in_priority;
layout(location = 5) in float in_max_z;

out vec2 tex_coords;
out vec4 color;
out float light_type;
out float max_z;

uniform mat3 world_to_screen_matrix;

void main()
{
    vec3 pos = world_to_screen_matrix * vec3(in_position.xy, 1);
    gl_Position = vec4(pos.xy, -in_priority, 1);

    tex_coords = in_tex_coords;
    color = in_color;
    light_type = float(in_light_type);
    max_z = -in_max_z;
}