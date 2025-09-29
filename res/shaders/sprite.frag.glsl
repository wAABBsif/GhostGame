#version 330 core

in float tex_index;
in vec2 tex_coords;
in vec4 color;

out vec4 FragColor;

uniform sampler2D textures[16];

void main()
{
    FragColor = texture(textures[int(tex_index)], tex_coords);
}