in vec2 tex_coords;

out vec4 FragColor;

uniform sampler2D atlas_texture;

void main()
{
    FragColor = texture(atlas_texture, tex_coords);
    if (FragColor.a < 0.05)
        discard;
}