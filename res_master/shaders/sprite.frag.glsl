in float tex_index;
in vec2 tex_coords;
in vec4 color;

out vec4 FragColor;

uniform sampler2D textures[16];

sampler2D get_tex(int index)
{
    //this is really stupid
    switch (int(tex_index))
    {
        case 0:
            return textures[0];
        case 1:
            return textures[1];
        case 2:
            return textures[2];
        case 3:
            return textures[3];
        case 4:
            return textures[4];
        case 5:
            return textures[5];
        case 6:
            return textures[6];
        case 7:
            return textures[7];
        case 8:
            return textures[8];
        case 9:
            return textures[9];
        case 10:
            return textures[10];
        case 11:
            return textures[11];
        case 12:
            return textures[12];
        case 13:
            return textures[13];
        case 14:
            return textures[14];
        case 15:
            return textures[15];
    }

    return textures[0];
}

void main()
{
    FragColor = texture(get_tex(int(tex_index)), tex_coords) * color;
    if (FragColor.a < 0.05)
        discard;
}