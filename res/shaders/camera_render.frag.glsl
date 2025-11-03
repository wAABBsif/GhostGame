in vec2 frag_tex_coords;

out vec4 FragColor;

uniform sampler2D main_texture;
uniform sampler2D lighting_texture;

void main()
{
    FragColor = texture(main_texture, frag_tex_coords) * texture(lighting_texture, frag_tex_coords);
}