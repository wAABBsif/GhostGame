#include "clean_edge.glsl"

in vec2 tex_coords;

out vec4 FragColor;

uniform sampler2D tile_atlas;

void main()
{
	FragColor = clean_edge(tile_atlas, tex_coords);
    if (FragColor.a < 0.01)
        discard;
}