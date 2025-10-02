#version 330 core

in float tex_index;
in vec2 tex_coords;
in vec4 color;

out vec4 FragColor;

uniform sampler2D textures[16];

vec4 get_tex_color(int index)
{
    //this is really stupid
    switch (int(tex_index))
    {
        case 0:
            return texture(textures[0], tex_coords);
        case 1:
            return texture(textures[1], tex_coords);
        case 2:
            return texture(textures[2], tex_coords);
        case 3:
            return texture(textures[3], tex_coords);
        case 4:
            return texture(textures[4], tex_coords);
        case 5:
            return texture(textures[5], tex_coords);
        case 6:
            return texture(textures[6], tex_coords);
        case 7:
            return texture(textures[7], tex_coords);
        case 8:
            return texture(textures[8], tex_coords);
        case 9:
            return texture(textures[9], tex_coords);
        case 10:
            return texture(textures[10], tex_coords);
        case 11:
            return texture(textures[11], tex_coords);
        case 12:
            return texture(textures[12], tex_coords);
        case 13:
            return texture(textures[13], tex_coords);
        case 14:
            return texture(textures[14], tex_coords);
        case 15:
            return texture(textures[15], tex_coords);
    }
}

void main()
{
    FragColor = get_tex_color(int(tex_index)) * color;
    if (FragColor.a < 0.01)
        discard;
}