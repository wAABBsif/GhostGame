in vec2 tex_coords;
in vec4 color;
in float light_type;
in float max_z;

out vec4 FragColor;

uniform sampler2D radial_texture;
uniform sampler2D depth_texture;
uniform ivec2 render_size;

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
    float depth = texture(depth_texture, gl_FragCoord.xy / render_size).r;
    depth = (depth - 0.5) * 2;
    if (max_z > depth)
        discard;
    FragColor = get_color(int(light_type));
}