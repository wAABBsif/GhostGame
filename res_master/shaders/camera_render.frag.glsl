in vec2 frag_tex_coords;

out vec4 FragColor;

uniform sampler2D camera_texture;

void main()
{
    FragColor = texture(camera_texture, frag_tex_coords);
}