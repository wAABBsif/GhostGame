in vec2 tex_coords;
in vec4 color;
in float light_type;

out vec4 FragColor;

uniform sampler2D radial_texture;

vec4 get_color(int type)
{
    switch (type)
    {
        case 0:
            return color;
        case 1:
            return texture(radial_texture, tex_coords) * color;
    }

    return vec4(0);
}

void main()
{
    FragColor = get_color(int(light_type));
}