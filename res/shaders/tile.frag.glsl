#version 330 core

in vec2 tex_coords;

out vec4 FragColor;

uniform sampler2D tile_atlas;

void main()
{
    FragColor = texture(tile_atlas, tex_coords);
}