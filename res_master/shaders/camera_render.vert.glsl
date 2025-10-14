layout(location = 0) in vec2 vert_position;
layout(location = 1) in vec2 vert_tex_coords;

out vec2 frag_tex_coords;

void main()
{
    gl_Position = vec4(vert_position, 0, 1);
    frag_tex_coords = vert_tex_coords;
}