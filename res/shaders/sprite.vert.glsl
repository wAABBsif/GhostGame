layout(location = 0) in vec2 in_position;
layout(location = 1) in vec2 in_tex_coords;
layout(location = 2) in vec4 in_color;
layout(location = 3) in float in_z;
layout(location = 4) in int in_use_camera_to_screen_matrix;
layout(location = 5) in float in_texture_index;

out float tex_index;
out vec2 tex_coords;
out vec4 color;

uniform mat3 world_to_screen_matrix;
uniform mat3 camera_to_screen_matrix;

void main()
{
    mat3 m = in_use_camera_to_screen_matrix != 0 ? camera_to_screen_matrix : world_to_screen_matrix;

    vec3 pos = m * vec3(in_position.xy, 1);
    gl_Position = vec4(pos.xy, in_z, 1);

    tex_index = in_texture_index;
    tex_coords = in_tex_coords;
    color = in_color;
}